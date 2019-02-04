#pragma once
#include <EnginePch.h>
#include <System/IRunnable.h>
#include <thread>
#include <mutex>
#include <queue>

class API ThreadHandler
{
	friend class Application;

public:
	static void RequestExecution(IRunnable* runable) noexcept;

private:
	static std::queue<IRunnable*> executionQueue;
	static bool exit;
	static std::thread thread;
	static std::mutex mutex;

	static void Run() noexcept;
	static void Init() noexcept;
	static void Exit() noexcept;
};