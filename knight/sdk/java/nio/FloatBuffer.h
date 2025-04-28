#pragma once

#include "../IClass.h"
#include "../../../engine/util/math/geometry.h"
#include "../../../engine/java/java.h"

struct FloatBuffer : IClass
{
	FloatBuffer(jobject obj);

	jclass GetClass();
	jobject GetInstance();

	Matrix GetMatrix();
};

