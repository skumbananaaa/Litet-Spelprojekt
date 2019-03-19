#pragma once
#include <EnginePch.h>
#include <System/IRunnable.h>
#include <System/IMultiUpdater.h>

class API ThreadHandler
{
	friend class Application;

public:
	static void RequestExecution(IRunnable* pRunable) noexcept;
	static void AddMultiUpdater(IMultiUpdater* pUpdater);
	static void RemoveMultiUpdater(IMultiUpdater* pUpdater);
	static void SetExecutionPaused(bool pause);

private:
	static std::queue<IRunnable*> s_ExecutionQueue;
	static std::vector<IMultiUpdater*> s_MultiUpdaters;
	static bool s_Exit;
	static bool s_PauseExecution;
	static std::thread s_Thread1;
	static std::thread s_Thread2;
	static std::mutex s_Mutex1;
	static std::mutex s_Mutex2;

	static void Run1() noexcept;
	static void Run2() noexcept;
	static void Init() noexcept;
	static void Exit() noexcept;
};