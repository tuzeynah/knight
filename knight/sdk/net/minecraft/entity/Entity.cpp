#include "Entity.h"

#include "../../../java/lang/String.h"
#include "../../../sdk.h"
 
CEntity::CEntity()
{
	if (!StrayCache::initialized) StrayCache::Initialize();
	this->Class = StrayCache::entity_class;
}

CEntity::CEntity(jobject instance) : CEntity()
{
	this->Instance = instance;
}

jclass CEntity::GetClass()
{
	return this->Class;
}

jobject CEntity::GetInstance()
{
	return this->Instance;
}

std::string CEntity::GetName()
{
	String str = String(Java::Env->CallObjectMethod(this->GetInstance(), StrayCache::entity_getName));
	return str.ToString();
}

/*

https://github.com/h1meji
shoutout to him for porting fusion to 1.7.10
i could do it myself but it saved me a lot of time

*/
Vector3 CEntity::GetPos()
{
	if ((Java::Version == minecraft_version::MAJOR_1_7_10) && this->GetName() == SDK::Minecraft->thePlayer->GetName())
	{
		// In 1.7.10, the entity position is the "eye" position, so we need to subtract the eye height to get the actual position. But only for the main player entity
		return Vector3{
			(float)(double)Java::Env->GetDoubleField(this->GetInstance(), StrayCache::entity_posX),
			(float)(double)Java::Env->GetDoubleField(this->GetInstance(), StrayCache::entity_posY) - (this->GetHeight() * 0.85f),
			(float)(double)Java::Env->GetDoubleField(this->GetInstance(), StrayCache::entity_posZ)
		};
	}
	else
	{
		return Vector3{
			(float)(double)Java::Env->GetDoubleField(this->GetInstance(), StrayCache::entity_posX),
			(float)(double)Java::Env->GetDoubleField(this->GetInstance(), StrayCache::entity_posY),
			(float)(double)Java::Env->GetDoubleField(this->GetInstance(), StrayCache::entity_posZ)
		};
	}
}

Vector3 CEntity::GetEyePos()
{
	Vector3 pos = GetPos();
	return Vector3{
		pos.x,
		(float)(double)(pos.y + (this->GetHeight() * 0.85f)),
		pos.z
	};
}

Vector3 CEntity::GetLastTickPos()
{
	if ((Java::Version == minecraft_version::MAJOR_1_7_10) && this->GetName() == SDK::Minecraft->thePlayer->GetName())
	{
		// In 1.7.10, the entity position is the "eye" position, so we need to subtract the eye height to get the actual position. But only for the main player entity
		return Vector3{
			(float)(double)Java::Env->GetDoubleField(this->GetInstance(), StrayCache::entity_lastTickPosX),
			(float)(double)Java::Env->GetDoubleField(this->GetInstance(), StrayCache::entity_lastTickPosY) - (this->GetHeight() * 0.85f),
			(float)(double)Java::Env->GetDoubleField(this->GetInstance(), StrayCache::entity_lastTickPosZ)
		};
	}
	else
	{
		return Vector3{
			(float)(double)Java::Env->GetDoubleField(this->GetInstance(), StrayCache::entity_lastTickPosX),
			(float)(double)Java::Env->GetDoubleField(this->GetInstance(), StrayCache::entity_lastTickPosY),
			(float)(double)Java::Env->GetDoubleField(this->GetInstance(), StrayCache::entity_lastTickPosZ)
		};
	}
}
 
bool CEntity::isOnGround()
{
	return Java::Env->GetBooleanField(this->GetInstance(), StrayCache::entity_isOnGround);
}
 
void CEntity::forceGround()
{
	Java::Env->SetBooleanField(this->GetInstance(), StrayCache::entity_isOnGround, true);
}

bool CEntity::IsSneaking()
{
	return Java::Env->CallBooleanMethod(this->GetInstance(), StrayCache::entity_isSneaking);
}

bool CEntity::IsSprinting()
{
	return Java::Env->CallBooleanMethod(this->GetInstance(), StrayCache::entity_isSprinting);
}

void CEntity::setSprinting(bool value)
{
	Java::Env->CallVoidMethod(this->GetInstance(), StrayCache::entity_setSprinting, value);
}

bool CEntity::IsInvisibleToPlayer(jobject player)
{
	return Java::Env->CallBooleanMethod(this->GetInstance(), StrayCache::entity_isInvisibleToPlayer, player);
}

float CEntity::GetHeight()
{
	return Java::Env->GetFloatField(this->GetInstance(), StrayCache::entity_height);
}

float CEntity::GetWidth()
{
	return Java::Env->GetFloatField(this->GetInstance(), StrayCache::entity_width);
}

float CEntity::GetDistanceWalkedModified()
{
	return Java::Env->GetFloatField(this->GetInstance(), StrayCache::entity_distanceWalkedModified);
}

float CEntity::GetPrevDistanceWalkedModified()
{
	return Java::Env->GetFloatField(this->GetInstance(), StrayCache::entity_prevDistanceWalkedModified);
}

float CEntity::GetRotationYaw()
{
	return Java::Env->GetFloatField(this->GetInstance(), StrayCache::entity_rotationYaw);
}

float CEntity::GetRotationPitch()
{
	return Java::Env->GetFloatField(this->GetInstance(), StrayCache::entity_rotationPitch);
}

float CEntity::GetPrevRotationYaw()
{
	return Java::Env->GetFloatField(this->GetInstance(), StrayCache::entity_prevRotationYaw);
}

float CEntity::GetPrevRotationPitch()
{
	return Java::Env->GetFloatField(this->GetInstance(), StrayCache::entity_prevRotationPitch);
}

void CEntity::SetRotationYaw(float value)
{
	Java::Env->SetFloatField(this->GetInstance(), StrayCache::entity_rotationYaw, value);
}

void CEntity::SetRotationPitch(float value)
{
	Java::Env->SetFloatField(this->GetInstance(), StrayCache::entity_rotationPitch, value);
}

void CEntity::SetPrevRotationYaw(float value)
{
	Java::Env->SetFloatField(this->GetInstance(), StrayCache::entity_prevRotationYaw, value);
}

void CEntity::SetPrevRotationPitch(float value)
{
	Java::Env->SetFloatField(this->GetInstance(), StrayCache::entity_prevRotationPitch, value);
}

Vector2 CEntity::GetAngles()
{
	return Vector2(CEntity::GetRotationYaw(), CEntity::GetRotationPitch());
}

Vector2 CEntity::GetPrevAngles()
{
	return Vector2(CEntity::GetPrevRotationYaw(), CEntity::GetPrevRotationPitch());
}

void CEntity::setMotionX(double motion) 
{
	Java::Env->SetDoubleField(this->GetInstance(), StrayCache::entity_motionX, motion);
};

void CEntity::setMotionY(double motion)
{
	Java::Env->SetDoubleField(this->GetInstance(), StrayCache::entity_motionY, motion);
};

void CEntity::setMotionZ(double motion)
{
	Java::Env->SetDoubleField(this->GetInstance(), StrayCache::entity_motionZ, motion);
};

double CEntity::getMotionX()
{
	return Java::Env->GetDoubleField(this->GetInstance(), StrayCache::entity_motionX);
};

double CEntity::getMotionY()
{
	return Java::Env->GetDoubleField(this->GetInstance(), StrayCache::entity_motionY);
};

double CEntity::getMotionZ()
{
	return Java::Env->GetDoubleField(this->GetInstance(), StrayCache::entity_motionZ);
};

void CEntity::SetAngles(Vector2 angles)
{
	Java::Env->SetFloatField(this->GetInstance(), StrayCache::entity_rotationYaw, angles.x);
	Java::Env->SetFloatField(this->GetInstance(), StrayCache::entity_rotationPitch, angles.y);
};

CAxisAlignedBB CEntity::GetBB()
{
	return CAxisAlignedBB(
		Java::Env->GetObjectField(this->GetInstance(), StrayCache::entity_boundingBox)
	);
}

void CEntity::SetBB(BoundingBox bb)
{
	this->GetBB().SetBoundingBox(bb);
}
