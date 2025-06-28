#pragma once

#include "../IClass.h"
#include "../../../engine/java/java.h"

struct List : IClass
{
	List(jobject jlist);

	jclass GetClass();
	jobject GetInstance();

	jobjectArray toArray();
};

