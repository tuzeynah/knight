#include "sdk.h"
#include "../engine/util/logger.hpp"
#include "../engine/java/java.h"
#include "strayCache.h"

void SDK::Init()
{
	StrayCache::Initialize();
	SDK::Minecraft = new CMinecraft();
}
