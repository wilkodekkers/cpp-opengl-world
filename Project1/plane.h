#pragma once
#include "Object.h"

class plane : public object
{
public:
	plane();
	plane(const char* texture_path);
	~plane();
};

