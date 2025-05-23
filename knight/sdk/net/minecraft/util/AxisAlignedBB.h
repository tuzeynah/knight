#pragma once

#include "../../../java/IClass.h"
#include "../../../strayCache.h"

struct CAxisAlignedBB : IClass
{
	CAxisAlignedBB();
	CAxisAlignedBB(jobject instance);

	jclass GetClass();
	jobject GetInstance();

	BoundingBox GetNativeBoundingBox();
	void SetBoundingBox(BoundingBox newBoundingBox);
};

