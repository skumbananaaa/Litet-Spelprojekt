#include <EnginePch.h>
#include <System\ThreadHandler.h>

std::queue<IRunnable*> ThreadHandler::executionQueue;
bool ThreadHandler::exit = true;
std::thread ThreadHandler::thread;
std::mutex ThreadHandler::mutex;

void ThreadHandler::RequestExecution(IRunnable* runable) noexcept
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
	thread.join();
}

void ThreadHandler::Run() noexcept
{
	std::cout << "New Thread Started! ID: " << thread.get_id() << std::endl;
	while (!exit)
	{
		IRunnable* runnable = nullptr;

		if (!executionQueue.empty())
		{
			mutex.lock();
			runnable = executionQueue.front();
			executionQueue.pop();
			mutex.unlock();
		}

		if (runnable)
		{
			runnable->RunParallel();
		}
		else
		{
			std::cout << "Vafan Bengan" << std::endl;
		}
	}
	std::cout << "Thread Terminated! ID: " << thread.get_id() << std::endl;
}