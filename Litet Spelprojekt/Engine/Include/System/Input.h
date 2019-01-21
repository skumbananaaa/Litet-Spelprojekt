#pragma once
#include <EnginePch.h>

class Input
{
public:
	//This is a static class, no use for these
	Input() = delete;
	~Input() = delete;
	Input(Input&& other) = delete;
	Input(const Input& other) = delete;
	Input& operator=(Input&& other) = delete;
	Input& operator=(const Input& other) = delete;

	static bool IsKeyDown();
	static bool IsKeyUp();
};