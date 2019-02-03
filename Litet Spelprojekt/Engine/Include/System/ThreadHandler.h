#pragma once
#include <EnginePch.h>
#include <System/IRunable.h>
#include <thread>
#include <mutex>
#include <queue>

class API ThreadHandler
{
	friend class Application;

public:
	static void RequestExecution(IRunable* runable) noexcept;

private:
	static std::queue<IRunable*> executionQueue;
	static bool exit;
	static std::thread thread;
	static std::mutex mutex;

	static void Run() noexcept;
	static void Init() noexcept;
	static void Exit() noexcept;
};