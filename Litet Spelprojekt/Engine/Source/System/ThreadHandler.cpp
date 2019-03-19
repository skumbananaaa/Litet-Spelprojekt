#include <EnginePch.h>
#include <System\ThreadHandler.h>
#include <System/CPUProfiler.h>

std::queue<IRunnable*> ThreadHandler::s_ExecutionQueue;
std::vector<IMultiUpdater*> ThreadHandler::s_MultiUpdaters;
bool ThreadHandler::s_Exit = true;
std::thread ThreadHandler::s_Thread1;
std::thread ThreadHandler::s_Thread2;
std::mutex ThreadHandler::s_Mutex1;
std::mutex ThreadHandler::s_Mutex2;

void ThreadHandler::RequestExecution(IRunnable* runable) noexcept
{
	s_Mutex1.lock();
	s_ExecutionQueue.push(runable);
	s_Mutex1.unlock();
}

void ThreadHandler::AddMultiUpdater(IMultiUpdater* pUpdater)
{
	s_Mutex2.lock();
	s_MultiUpdaters.push_back(pUpdater);
	s_Mutex2.unlock();
}

void ThreadHandler::RemoveMultiUpdater(IMultiUpdater* pUpdater)
{
	s_Mutex2.lock();
	for (int i = 0; i < s_MultiUpdaters.size(); i++)
	{
		if (s_MultiUpdaters[i] == pUpdater)
		{
			s_MultiUpdaters.erase(s_MultiUpdaters.begin() + i);
			break;
		}
	}
	s_Mutex2.unlock();
}

void ThreadHandler::Init() noexcept
{
	if (s_Exit)
	{
		s_Exit = false;
		s_Thread1 = std::thread(Run1);
		s_Thread2 = std::thread(Run2);
	}
}

void ThreadHandler::Exit() noexcept
{
	s_Exit = true;
	s_Thread1.join();
	s_Thread2.join();
}

void ThreadHandler::Run1() noexcept
{
	std::cout << "Thread_1 Started" << std::endl;
	while (!s_Exit)
	{
		IRunnable* runnable = nullptr;

		if (!s_ExecutionQueue.empty())
		{
			s_Mutex1.lock();
			runnable = s_ExecutionQueue.front();
			s_ExecutionQueue.pop();
			s_Mutex1.unlock();
		}

		if (runnable)
		{
			runnable->RunParallel();
		}
	}
	std::cout << "Thread_1 Terminated" << std::endl;
}

void ThreadHandler::Run2() noexcept
{
	std::cout << "Thread_2 Started" << std::endl;

	using clock = std::chrono::high_resolution_clock;
	using duration = std::chrono::duration<float>;

	auto currentTime = clock::now();
	auto m_PrevTime = currentTime;
	float deltaTime = 0.0f;

	while (!s_Exit)
	{
		#if defined(PRINT_CPU_DEBUG_DATA)
			CPUProfiler::StartTimer(CPU_PROFILER_SLOT_7);
		#endif
		currentTime = clock::now();
		deltaTime = std::chrono::duration_cast<duration>(currentTime - m_PrevTime).count();
		m_PrevTime = currentTime;

		s_Mutex2.lock();
		for (int i = 0; i < s_MultiUpdaters.size(); i++)
		{
			s_MultiUpdaters[i]->UpdateParallel(deltaTime);
		}
		s_Mutex2.unlock();

		#if defined(PRINT_CPU_DEBUG_DATA)
			CPUProfiler::EndTimer("Animation Bone Update took %.3f ms", CPU_PROFILER_SLOT_7);
		#endif
	}
	std::cout << "Thread_2 Terminated" << std::endl;
}
