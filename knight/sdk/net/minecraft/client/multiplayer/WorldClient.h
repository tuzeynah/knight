#pragma once

#include "../../world/World.h"
#include "../../../../strayCache.h"

struct CWorldClient : CWorld
{
	CWorldClient();

	jclass EntityPlayer;

	jclass GetClass();
	jobject GetInstance();

	std::vector<CEntity> GetEntityList();
};

