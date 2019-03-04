#pragma once
#include <EnginePch.h>

class API IGLObject
{
public:
	IGLObject(IGLObject&& other) = delete;
	IGLObject(const IGLObject& other) = delete;
	IGLObject& operator=(IGLObject&& other) = delete;
	IGLObject& operator=(const IGLObject& other) = delete;

	IGLObject() {}
	virtual ~IGLObject() {}

	virtual void SetDebugName(const char* pName) = 0;
};