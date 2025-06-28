#pragma once

#include "../../../java/IClass.h"
#include "../../../strayCache.h"

struct CItemStack : IClass
{
	CItemStack();

	CItemStack(jobject instance);

	jclass GetClass();
	jobject GetInstance();

	jobject GetItem();
};