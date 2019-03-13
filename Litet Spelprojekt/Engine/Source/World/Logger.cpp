#include <EnginePch.h>
#include <World/Logger.h>

std::vector<std::string> Logger::s_Logs;
ILogListener* Logger::s_Listener = nullptr;
bool Logger::debugUIOverride = false;

void Logger::LogEvent(const std::string& text, bool showInUI) noexcept
{
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%X - ", &tstruct);

	std::string line = buf;
	line += text;

	std::cout << line << std::endl;
	s_Logs.push_back(line);
	if (showInUI || debugUIOverride)
	{
		if (s_Listener)
		{
			s_Listener->OnLogged(line);
		}
	}
}

void Logger::SetListener(ILogListener* listener)
{
	s_Listener = listener;
}

void Logger::Save() noexcept
{
	std::string finalText;
	for (int i = 0; i < s_Logs.size(); i++)
	{
		finalText += s_Logs[i] + "\n";
	}
	FILE* file = fopen("Game_Session_Log.txt", "w");
	fputs(finalText.c_str(), file);
	fclose(file);

	s_Logs.clear();
}

uint32 Logger::GetNumEntries()
{
	return (uint32)s_Logs.size();
}

const std::string& Logger::GetEntry(uint32 index)
{
	return s_Logs[index];
}
