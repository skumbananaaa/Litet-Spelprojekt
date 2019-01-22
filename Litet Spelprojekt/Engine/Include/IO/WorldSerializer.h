#pragma once
#include "../EnginePch.h"
#include <vector>
#include <sstream>
#include <fstream>
#include <functional>

class API WorldSerializer
{
public:
	static bool Read(const char* filename, std::function<void(int)> callback);
	//static bool Write(const std::string filename, const std::vector<std::string> data);
};