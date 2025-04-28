#pragma once

#include "../../../strayCache.h"
#include "../../../java/IClass.h"
#include "../util/AxisAlignedBB.h"

struct CEntity : IClass
{
	CEntity();

	CEntity(jobject instance);

	jclass GetClass();
	jobject GetInstance();

	std::string GetName();

	Vector3 GetPos();
	Vector3 GetEyePos();
	Vector3 GetLastTickPos();

	bool isOnGround();
	void forceGround();

	bool IsSneaking();
	bool IsSprinting();
	void setSprinting(bool value);

	bool IsInvisibleToPlayer(jobject player);

	float GetHeight();
	float GetWidth(); 

	float GetDistanceWalkedModified();
	float GetPrevDistanceWalkedModified();

	double getMotionX();
	double getMotionY();
	double getMotionZ();

	float GetRotationYaw();
	float GetRotationPitch();

	float GetPrevRotationYaw();
	float GetPrevRotationPitch();
	 
	Vector2 GetAngles();
	Vector2 GetPrevAngles();

	void SetRotationYaw(float value);
	void SetRotationPitch(float value);

	void SetPrevRotationYaw(float value);
	void SetPrevRotationPitch(float value);


	void SetAngles(Vector2 angles);

	void setMotionX(double motion);
	void setMotionY(double motion);
	void setMotionZ(double motion);

	CAxisAlignedBB GetBB();
	void SetBB(BoundingBox bb);
};