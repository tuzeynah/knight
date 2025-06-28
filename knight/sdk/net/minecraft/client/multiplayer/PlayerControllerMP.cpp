#include "PlayerControllerMP.h"

#include "../../../../sdk.h"

CPlayerControllerMP::CPlayerControllerMP()
{
	this->Class = StrayCache::playerControllerMP_class;
	this->FieldIDs["curBlockDamageMP"] = StrayCache::playerControllerMP_curBlockDamageMP;
	this->FieldIDs["blockHitDelay"] = StrayCache::playerControllerMP_blockHitDelay;
	this->FieldIDs["isHittingBlock"] = StrayCache::playerControllerMP_isHittingBlock;
}

CPlayerControllerMP::CPlayerControllerMP(jobject instance) : CPlayerControllerMP()
{
	this->Instance = instance;
}

jobject CPlayerControllerMP::GetInstance()
{
	return Java::Env->GetObjectField(SDK::Minecraft->GetInstance(), SDK::Minecraft->FieldIDs["playerController"]);
}

jclass CPlayerControllerMP::GetClass()
{
	return this->Class;
}

float CPlayerControllerMP::getBlockDamage()
{
	return Java::Env->GetFloatField(this->GetInstance(), this->FieldIDs["curBlockDamageMP"]);
}

void CPlayerControllerMP::setBlockDamage(float value)
{
	Java::Env->SetFloatField(this->GetInstance(), this->FieldIDs["curBlockDamageMP"], value);
}

int CPlayerControllerMP::getBlockHitDelay()
{
	return Java::Env->GetIntField(this->GetInstance(), this->FieldIDs["blockHitDelay"]);
}

void CPlayerControllerMP::setBlockHitDelay(int value)
{
	Java::Env->SetIntField(this->GetInstance(), this->FieldIDs["blockHitDelay"], value);
}

bool CPlayerControllerMP::isHittingBlock()
{
	return Java::Env->GetBooleanField(this->GetInstance(), this->FieldIDs["isHittingBlock"]);
}

