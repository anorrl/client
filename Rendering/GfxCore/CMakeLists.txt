include(Rendering)

include_project_files(GfxCore "*")

include_directories(include)
include_directories(glad/include)
include_directories(GL)

if(ANDROID)
  list(APPEND SOURCES GL/ContextGLAndroid.cpp)
elseif(WIN32)
  list(APPEND SOURCES GL/ContextGLWin32.cpp)
elseif(APPLE)
  list(APPEND SOURCES GL/ContextGLEAGL.mm)
  list(APPEND SOURCES GL/ContextGLNSGL.mm)
elseif(LINUX)
  list(APPEND SOURCES 
    glad/include/EGL/eglplatform.h
    glad/include/glad/egl.h
    glad/src/egl.c
    GL/ContextEGL.cpp
  )
endif(ANDROID)

list(APPEND HEADERS GL/ContextGL.h)
list(APPEND HEADERS GL/DeviceGL.h)
list(APPEND HEADERS GL/FramebufferGL.h)
list(APPEND HEADERS GL/GeometryGL.h)
list(APPEND HEADERS GL/HeadersGL.h)
list(APPEND HEADERS GL/ShaderGL.h)
list(APPEND HEADERS GL/TextureGL.h)
list(APPEND HEADERS include/GfxCore/Device.h)
list(APPEND HEADERS include/GfxCore/Framebuffer.h)
list(APPEND HEADERS include/GfxCore/Geometry.h)
list(APPEND HEADERS include/GfxCore/Resource.h)
list(APPEND HEADERS include/GfxCore/Shader.h)
list(APPEND HEADERS include/GfxCore/States.h)
list(APPEND HEADERS include/GfxCore/Texture.h)
list(APPEND HEADERS include/GfxCore/pix.h)
list(APPEND HEADERS glad/include/glad/gl.h)

list(APPEND SOURCES Device.cpp)
list(APPEND SOURCES DeviceCreate.cpp)
list(APPEND SOURCES Framebuffer.cpp)
list(APPEND SOURCES GL/CardboardVRGL.cpp)
list(APPEND SOURCES GL/DeviceContextGL.cpp)
list(APPEND SOURCES GL/DeviceGL.cpp)
list(APPEND SOURCES GL/FramebufferGL.cpp)
list(APPEND SOURCES GL/GeometryGL.cpp)
list(APPEND SOURCES GL/ShaderGL.cpp)
list(APPEND SOURCES GL/TextureGL.cpp)
list(APPEND SOURCES Geometry.cpp)
list(APPEND SOURCES Resource.cpp)
list(APPEND SOURCES Shader.cpp)
list(APPEND SOURCES States.cpp)
list(APPEND SOURCES Texture.cpp)
list(APPEND SOURCES pix.cpp)
list(APPEND SOURCES glad/src/gl.c)

add_library(GfxCore OBJECT ${SOURCES} ${HEADERS})

if(STUDIO_BUILD)
  target_link_libraries(GfxCore PRIVATE Qt5::OpenGL)
endif()