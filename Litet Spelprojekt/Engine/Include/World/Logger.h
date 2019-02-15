#pragma once
#include <EnginePch.h>

class API ILogListener
{
public:
	virtual void OnLogged(const std::string& text) noexcept = 0;
};

class API Logger
{
public:
	static void LogEvent(const std::string& text, bool showInUI = false) noexcept;
	static void SetListener(ILogListener* listener);
	static void Save() noexcept;

private:
	static std::vector<std::string> s_Logs;
	static ILogListener* s_Listener;
};