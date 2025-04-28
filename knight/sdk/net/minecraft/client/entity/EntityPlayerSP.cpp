#include "EntityPlayerSP.h"
#include "../../../../sdk.h"
#include "../../../../../engine/java/java.h"

#include "../../../../../engine/util/logger.hpp"
#include "../../../../../engine/game_data.h"

CEntityPlayerSP::CEntityPlayerSP() : CEntityPlayer()
{
	Java::AssignClass(Java::Version == minecraft_version::MAJOR_1_7_10 ? "Lnet/minecraft/client/entity/EntityClientPlayerMP;" : "Lnet/minecraft/client/entity/EntityPlayerSP;", this->Class);
}

jclass CEntityPlayerSP::GetClass()
{
	return this->Class;
}

jobject CEntityPlayerSP::GetInstance()
{
	return Java::Env->GetObjectField(SDK::Minecraft->GetInstance(), SDK::Minecraft->FieldIDs["thePlayer"]);
}
 