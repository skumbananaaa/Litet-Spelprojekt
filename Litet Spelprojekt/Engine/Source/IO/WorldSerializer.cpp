#include <EnginePch.h>
#include "../../Include/IO/WorldSerializer.h"

bool WorldSerializer::Read(const char* filename, std::function<void(int)> callback)
{
	std::ifstream file(filename, std::ios::in);
	if (file.is_open())
	{
		/*std::string line;
		while (std::getline(file, line))
		{
			data->push_back(line);
		}
		file.close();*/
		return true;
	}
	return false;
}
/*
bool WorldSerializer::Write(const std::string filename, const std::vector<std::string> data)
{
	std::ofstream file(filename, std::ios::out | std::ios::trunc);
	if (file.is_open())
	{
		for (auto const& value : data)
		{
			file << value;
		}
		file.close();
		return true;
	}
	return false;
}*/