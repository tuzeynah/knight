#pragma once

#include "../IClass.h"
#include "../../../engine/java/java.h"

struct String : IClass
{
	String(jobject instance);

	String(std::string text);

	jclass GetClass();
	jobject GetInstance();


	std::string ToString();
};

