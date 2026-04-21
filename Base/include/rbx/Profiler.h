#pragma once

#include <stdint.h>

#include "RbxFormat.h" // for ARL_PRINTF_ATTR

#if defined(_WIN32) || defined(__APPLE__) || defined(__ANDROID__)
#define ARLPROFILER
#endif

namespace ARL
{
	namespace Profiler
	{
		typedef uint64_t Token;

		Token getToken(const char* group, const char* name, int color = -1);
		Token getLabelToken(const char* group);
		Token getCounterToken(const char* name);

		uint64_t enterRegion(Token token);
		void leaveRegion(Token token, uint64_t enterTimestamp);

		void addLabel(Token token, const char* name);
        ARL_PRINTF_ATTR(2, 3) void addLabelFormat(Token token, const char* name, ...);
        
        void counterAdd(Token token, long long count);
        void counterSet(Token token, long long count);

		void onThreadCreate(const char* name);
		void onThreadExit();

		void onFrame();

		enum Flags
		{
			Flag_MouseMove = 1 << 0,
			Flag_MouseWheel = 1 << 1,
			Flag_MouseDown = 1 << 2,
			Flag_MouseUp = 1 << 3,
		};

		void gpuInit(void* context);
		void gpuShutdown();

		bool isCapturingMouseInput();
		bool handleMouse(unsigned int flags, int mouseX, int mouseY, int mouseWheel, int mouseButton);

		bool toggleVisible();
		bool togglePause();

		struct Renderer
		{
			virtual ~Renderer() {}

			virtual void drawText(int x, int y, unsigned int color, const char* text, unsigned int length, unsigned int textWidth, unsigned int textHeight) = 0;
			virtual void drawBox(int x0, int y0, int x1, int y1, unsigned int color0, unsigned int color1) = 0;
			virtual void drawLine(unsigned int vertexCount, const float* vertexData, unsigned int color) = 0;
		};

		bool isVisible();
		void render(Renderer* renderer, unsigned int width, unsigned int height);

		struct Scope
		{
			Token token;
			uint64_t timestamp;

			Scope(Token token): token(token)
			{
				timestamp = enterRegion(token);
			}

			~Scope()
			{
				leaveRegion(token, timestamp);
			}
		};
	}
}

#define ARLPROFILER_TOKEN_PASTE0(a, b) a ## b
#define ARLPROFILER_TOKEN_PASTE(a, b) ARLPROFILER_TOKEN_PASTE0(a,b)

#ifdef ARLPROFILER
	#define ARLPROFILER_SCOPE(group, name, ...) static ::ARL::Profiler::Token ARLPROFILER_TOKEN_PASTE(proftoken, __LINE__) = ::ARL::Profiler::getToken(group "", name "", ## __VA_ARGS__); ::ARL::Profiler::Scope ARLPROFILER_TOKEN_PASTE(profscope, __LINE__)(ARLPROFILER_TOKEN_PASTE(proftoken, __LINE__))
	#define ARLPROFILER_LABEL(group, label) static ::ARL::Profiler::Token ARLPROFILER_TOKEN_PASTE(proftoken, __LINE__) = ::ARL::Profiler::getLabelToken(group ""); ::ARL::Profiler::addLabel(ARLPROFILER_TOKEN_PASTE(proftoken, __LINE__), label)
	#define ARLPROFILER_LABELF(group, label, ...) static ::ARL::Profiler::Token ARLPROFILER_TOKEN_PASTE(proftoken, __LINE__) = ::ARL::Profiler::getLabelToken(group ""); ::ARL::Profiler::addLabelFormat(ARLPROFILER_TOKEN_PASTE(proftoken, __LINE__), label, ## __VA_ARGS__)
	#define ARLPROFILER_COUNTER_ADD(name, count) static ::ARL::Profiler::Token ARLPROFILER_TOKEN_PASTE(proftoken, __LINE__) = ::ARL::Profiler::getCounterToken(name ""); ::ARL::Profiler::counterAdd(ARLPROFILER_TOKEN_PASTE(proftoken, __LINE__), static_cast<long long>(count))
	#define ARLPROFILER_COUNTER_SUB(name, count) static ::ARL::Profiler::Token ARLPROFILER_TOKEN_PASTE(proftoken, __LINE__) = ::ARL::Profiler::getCounterToken(name ""); ::ARL::Profiler::counterAdd(ARLPROFILER_TOKEN_PASTE(proftoken, __LINE__), -static_cast<long long>(count))
	#define ARLPROFILER_COUNTER_SET(name, count) static ::ARL::Profiler::Token ARLPROFILER_TOKEN_PASTE(proftoken, __LINE__) = ::ARL::Profiler::getCounterToken(name ""); ::ARL::Profiler::counterSet(ARLPROFILER_TOKEN_PASTE(proftoken, __LINE__), count)
#else
	#define ARLPROFILER_SCOPE(group, name, ...) (void)0
	#define ARLPROFILER_LABEL(group, label) (void)0
	#define ARLPROFILER_LABELF(group, label, ...) (void)sizeof(0, __VA_ARGS__)
	#define ARLPROFILER_COUNTER_ADD(name, count) (void)0
	#define ARLPROFILER_COUNTER_SUB(name, count) (void)0
	#define ARLPROFILER_COUNTER_SET(name, count) (void)0
#endif
