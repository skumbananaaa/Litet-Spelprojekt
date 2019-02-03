#include <EnginePch.h>
#include <System\ThreadHandler.h>

std::queue<IRunable*> ThreadHandler::executionQueue;
bool ThreadHandler::exit = true;
std::thread ThreadHandler::thread;
std::mutex ThreadHandler::mutex;

void ThreadHandler::RequestExecution(IRunable* runable) noexcept
{
	mutex.lock();
	executionQueue.push(runable);
	mutex.unlock();
}

void ThreadHandler::Init() noexcept
{
	if (exit)
	{
		exit = false;
		thread = std::thread(Run);
	}
}

void ThreadHandler::Exit() noexcept
{
	exit = true;
}

void ThreadHandler::Run() noexcept
{
	std::cout << "New Thread Started! ID: " << thread.get_id() << std::endl;
	while (!exit)
	{
		IRunable* runable = nullptr;

		if (!executionQueue.empty())
		{
			mutex.lock();
			runable = executionQueue.front();
			executionQueue.pop();
			mutex.unlock();
		}

		if (runable)
		{
			runable->RunParallel();
		}
	}
	std::cout << "Thread Terminated! ID: " << thread.get_id() << std::endl;
}