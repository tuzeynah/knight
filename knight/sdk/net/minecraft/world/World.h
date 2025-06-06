#pragma once

#include "../../../strayCache.h"
#include "../../../java/IClass.h"
#include "../entity/player/EntityPlayer.h"
#include "../../../../engine/util/math/geometry.h"

#include <vector>

struct CWorld : IClass
{
	CWorld();

	jclass GetClass();
	jobject GetInstance();

	std::vector<CEntityPlayer> GetPlayerList();

	Vector3 rayTraceBlocks(Vector3 from, Vector3 to, bool stopOnLiquid, bool ignoreBlockWithoutBoundingBox, bool returnLastUncollidableBlock);
};

