#pragma once
#include <EnginePch.h>
#include <chrono>

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::duration<float> Duration;
typedef std::chrono::time_point<std::chrono::steady_clock> TimePoint;

#define CPU_PROFILER_SLOT_0 0
#define CPU_PROFILER_SLOT_1 1
#define CPU_PROFILER_SLOT_2 2
#define CPU_PROFILER_SLOT_3 3
#define CPU_PROFILER_SLOT_4 4
#define CPU_PROFILER_SLOT_5 5
#define CPU_PROFILER_SLOT_6 6
#define CPU_PROFILER_SLOT_7 7
#define CPU_PROFILER_SLOT_8 8
#define CPU_PROFILER_SLOT_9 9
#define CPU_PROFILER_SLOT_10 10
#define CPU_PROFILER_SLOT_11 11
#define CPU_PROFILER_SLOT_12 12
#define CPU_PROFILER_SLOT_13 13
#define CPU_PROFILER_SLOT_14 14
#define CPU_PROFILER_SLOT_15 15
#define CPU_PROFILER_NUM_SLOTS 16

class API CPUProfiler
{
public:
	static void Init() noexcept;
	static void StartTimer(uint32 slot) noexcept;
	static void EndTimer(const char* message, uint32 slot) noexcept;
	static void PrintTime() noexcept;
	static void Update(float dtS) noexcept;

private:
	static TimePoint s_PrevTime[CPU_PROFILER_NUM_SLOTS];
	static const char* s_Messages[CPU_PROFILER_NUM_SLOTS];
	static float s_LastDeltaTime[CPU_PROFILER_NUM_SLOTS];
	static float s_NumSums[CPU_PROFILER_NUM_SLOTS];
	static float s_PrintTimer;
};