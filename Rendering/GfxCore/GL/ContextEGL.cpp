#include "ContextGL.h"
#include "HeadersGL.h"
#include "arl/Debug.h"

#include <glad/egl.h>

#include <wayland-client.h>
#include <wayland-egl.h>
#include <X11/Xlib.h>
#include <EGL/eglext.h>
#include <cstdlib>

LOGGROUP(Graphics)

namespace ARL
{
namespace Graphics
{

enum class DisplayBackend
{
    Wayland,
    X11,
    Unknown
};

static DisplayBackend detectDisplayBackend()
{
    const char* waylandDisplay = std::getenv("WAYLAND_DISPLAY");
    const char* x11Display = std::getenv("DISPLAY");

    if (waylandDisplay && waylandDisplay[0])
        return DisplayBackend::Wayland;
    if (x11Display && x11Display[0])
        return DisplayBackend::X11;
    return DisplayBackend::Unknown;
}

class ContextEGL : public ContextGL
{
public:
    ContextEGL(void* windowHandle)
    {
        DisplayBackend backend = detectDisplayBackend();

        // Load core EGL API
        gladLoaderLoadEGL(nullptr);

        // Load required EGL extensions
        auto eglGetPlatformDisplayEXT = (PFNEGLGETPLATFORMDISPLAYEXTPROC)eglGetProcAddress("eglGetPlatformDisplayEXT");
        auto eglCreatePlatformWindowSurfaceEXT = (PFNEGLCREATEPLATFORMWINDOWSURFACEEXTPROC)eglGetProcAddress("eglCreatePlatformWindowSurfaceEXT");

        if (!eglGetPlatformDisplayEXT || !eglCreatePlatformWindowSurfaceEXT)
            throw ARL::runtime_error("Missing required EGL extension functions");

		
		display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

        // Create EGL display based on backend
        /*if (backend == DisplayBackend::Wayland)
        {
            wl_display* wlDisplay = static_cast<wl_display*>(windowScreen);
            display = eglGetPlatformDisplayEXT(EGL_PLATFORM_WAYLAND_EXT, wlDisplay, nullptr);
        }
        else if (backend == DisplayBackend::X11)
        {
            ::Display* x11Display = static_cast<::Display*>(windowScreen);
            display = eglGetPlatformDisplayEXT(EGL_PLATFORM_X11_EXT, x11Display, nullptr);
        }
        else
        {
            throw ARL::runtime_error("Unknown or unsupported display backend");
        }*/

        if (display == EGL_NO_DISPLAY)
            throw ARL::runtime_error("eglGetPlatformDisplayEXT failed: %x", eglGetError());

        if (!eglInitialize(display, nullptr, nullptr))
            throw ARL::runtime_error("eglInitialize failed: %x", eglGetError());

        if (!gladLoaderLoadEGL(display))
            throw ARL::runtime_error("gladLoaderLoadEGL failed");

        // EGL Config
        static const EGLint configAttribs[] = {EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT, EGL_RED_SIZE, 8, EGL_GREEN_SIZE,
            8, EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 8, EGL_DEPTH_SIZE, 24, EGL_STENCIL_SIZE, 8, EGL_NONE};

        EGLint numConfigs;
        if (!eglChooseConfig(display, configAttribs, &config, 1, &numConfigs) || numConfigs == 0)
            throw ARL::runtime_error("eglChooseConfig failed: %x", eglGetError());

        // Surface creation
        if (windowHandle)
        {
            /*if (backend == DisplayBackend::Wayland)
            {

                surface = eglCreatePlatformWindowSurfaceEXT(display, config, windowHandle, nullptr);
            }
            else // X11
            {
                surface = eglCreatePlatformWindowSurfaceEXT(display, config, windowHandle, nullptr);
            }*/

			surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)windowHandle, nullptr);
        }
        else
        {
            // Create pbuffer
            static const EGLint pbufferAttribs[] = {EGL_WIDTH, 1, EGL_HEIGHT, 1, EGL_NONE};
            surface = eglCreatePbufferSurface(display, config, pbufferAttribs);
        }

        if (surface == EGL_NO_SURFACE)
            throw ARL::runtime_error("Failed to create EGL surface: %x", eglGetError());

        // Bind OpenGL API
        eglBindAPI(EGL_OPENGL_API);

        static const EGLint contextAttribs[] = {EGL_CONTEXT_MAJOR_VERSION, 3, EGL_CONTEXT_MINOR_VERSION, 3, EGL_CONTEXT_OPENGL_PROFILE_MASK,
            EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT,
#ifndef NDEBUG
            EGL_CONTEXT_OPENGL_DEBUG, EGL_TRUE,
#endif
            EGL_NONE};

        context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
        if (context == EGL_NO_CONTEXT)
            throw ARL::runtime_error("eglCreateContext failed: %x", eglGetError());

        if (!eglMakeCurrent(display, surface, surface, context))
            throw ARL::runtime_error("eglMakeCurrent failed: %x", eglGetError());

        gladLoaderLoadGL();
    }

    virtual void setCurrent() override
    {
        eglMakeCurrent(display, surface, surface, context);
    }

    virtual void swapBuffers() override
    {
        eglSwapBuffers(display, surface);
    }

    virtual unsigned int getMainFramebufferId() override
    {
        return 0;
    }

    virtual bool isMainFramebufferRetina() override
    {
        return false;
    }

    void resizeWindow(int w, int h)
    {
        if (egl_window)
        {
            wl_egl_window_resize(egl_window, w, h, 0, 0);
        }
    }

    virtual std::pair<unsigned int, unsigned int> updateMainFramebuffer(unsigned int width, unsigned int height) override
    {
        EGLint w, h;
        eglQuerySurface(display, surface, EGL_WIDTH, &w);
        eglQuerySurface(display, surface, EGL_HEIGHT, &h);
        return std::make_pair((unsigned int)w, (unsigned int)h);
    }

    virtual ~ContextEGL()
    {
        if (display != EGL_NO_DISPLAY)
        {
            eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
            if (context != EGL_NO_CONTEXT)
                eglDestroyContext(display, context);
            if (surface != EGL_NO_SURFACE)
                eglDestroySurface(display, surface);
            eglTerminate(display);
        }

        if (egl_window)
        {
            wl_egl_window_destroy(egl_window);
            egl_window = nullptr;
        }
    }

private:
    EGLDisplay display = EGL_NO_DISPLAY;
    EGLContext context = EGL_NO_CONTEXT;
    EGLSurface surface = EGL_NO_SURFACE;
    EGLConfig config = nullptr;
    wl_egl_window* egl_window = nullptr;
};

ContextGL* ContextGL::create(void* windowHandle)
{
    return new ContextEGL(windowHandle);
}

} // namespace Graphics
} // namespace ARL
