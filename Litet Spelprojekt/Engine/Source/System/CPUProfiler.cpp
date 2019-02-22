#include <EnginePch.h>
#include <System/CPUProfiler.h>

TimePoint CPUProfiler::s_PrevTime[CPU_PROFILER_NUM_SLOTS] = { };
const char* CPUProfiler::s_Messages[CPU_PROFILER_NUM_SLOTS] = { };
float CPUProfiler::s_LastDeltaTime[CPU_PROFILER_NUM_SLOTS] = { };
float CPUProfiler::s_NumSums[CPU_PROFILER_NUM_SLOTS] = { };
float CPUProfiler::s_PrintTimer = 0.0f;

void CPUProfiler::Init() noexcept
{
	for (uint32 slot = 0; slot < CPU_PROFILER_NUM_SLOTS; slot++)
	{
		s_LastDeltaTime[slot] = 0.0f;
		s_NumSums[slot] = 0.0f;
	}
}

void CPUProfiler::StartTimer(uint32 slot) noexcept
{
	assert(slot < CPU_PROFILER_NUM_SLOTS);
	s_PrevTime[slot] = Clock::now();
}

void CPUProfiler::EndTimer(const char* message, uint32 slot) noexcept
{
	assert(slot < CPU_PROFILER_NUM_SLOTS);
	s_LastDeltaTime[slot] += 1000.0f * std::chrono::duration_cast<Duration>(Clock::now() - s_PrevTime[slot]).count();
	s_Messages[slot] = message;
	s_NumSums[slot] += 1.0f;
}

void CPUProfiler::PrintTime() noexcept
{
	if (s_PrintTimer > 1.0f)
	{
		s_PrintTimer = 0.0f;


		for (uint32 slot = 0; slot < CPU_PROFILER_NUM_SLOTS; slot++)
		{
			if (s_Messages[slot] != nullptr)
			{
				float averageTime = s_NumSums[slot] > 0.0f ? s_LastDeltaTime[slot] / s_NumSums[slot] : 0.0f;

				printf(s_Messages[slot], averageTime);
				printf("\n");
			}

			s_LastDeltaTime[slot] = 0.0f;
			s_NumSums[slot] = 0.0f;
		}
		printf("-----------\n");
	}
}

void CPUProfiler::Update(float dtS) noexcept
{
	s_PrintTimer += dtS;
}
