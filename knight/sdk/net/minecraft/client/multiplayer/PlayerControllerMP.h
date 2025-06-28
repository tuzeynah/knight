#pragma once

#include "../../../../java/IClass.h"

#include "../../../../strayCache.h"
struct CPlayerControllerMP : IClass
{
	CPlayerControllerMP();
	CPlayerControllerMP(jobject instance);

	void setBlockHitDelay(int value);
	void setBlockDamage(float value);

	int getBlockHitDelay();
	float getBlockDamage();
	bool isHittingBlock();

	jclass GetClass();
	jobject GetInstance();
};