#pragma once

#include "net/minecraft/client/Minecraft.h"
#include "../engine/java/java.h"

struct SDK
{
	static inline CMinecraft* Minecraft;

	static void Init();
};

