#pragma once

#include "Entity.h"
#include "../../../strayCache.h"

struct CEntityLivingBase : CEntity
{
	CEntityLivingBase();

	jclass GetClass();
	jobject GetInstance();

	int getHurtTime();
	int getMaxHurtTime();

	float GetHealth();
	float GetMaxHealth();
	bool CanEntityBeSeen(jobject entity);
	void jump();
};

