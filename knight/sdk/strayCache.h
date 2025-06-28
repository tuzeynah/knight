#pragma once

#include "../engine/util/math/geometry.h"
#include "../engine/util/logger.hpp"
#include "../engine/java/java.h"

struct StrayCache {

	inline static bool initialized;

	// ENTITY CLASS
	inline static jclass entity_class;
	inline static jmethodID entity_getName;
	inline static jmethodID entity_isSneaking;
	inline static jmethodID entity_isSprinting;
	inline static jmethodID entity_setSprinting;
	inline static jmethodID entity_isInvisibleToPlayer;

	inline static jfieldID entity_isOnGround;
	inline static jfieldID entity_posX;
	inline static jfieldID entity_posY;
	inline static jfieldID entity_posZ;

	inline static jfieldID entity_motionX;
	inline static jfieldID entity_motionY;
	inline static jfieldID entity_motionZ;
	 
	inline static jfieldID entity_lastTickPosX;
	inline static jfieldID entity_lastTickPosY;
	inline static jfieldID entity_lastTickPosZ;
	inline static jfieldID entity_width;
	inline static jfieldID entity_height;
	inline static jfieldID entity_distanceWalkedModified;
	inline static jfieldID entity_prevDistanceWalkedModified;
	inline static jfieldID entity_rotationYaw;
	inline static jfieldID entity_rotationPitch;
	inline static jfieldID entity_prevRotationYaw;
	inline static jfieldID entity_prevRotationPitch;
	inline static jfieldID entity_boundingBox;

	// ENTITY LIVING BASE CLASS
	inline static jclass entityLivingBase_class;

	inline static jfieldID entityLivingBase_maxHurtTime;
	inline static jfieldID entityLivingBase_hurtTime;

	inline static jmethodID entityLivingBase_jump;
	inline static jmethodID entityLivingBase_getHealth;
	inline static jmethodID entityLivingBase_getMaxHealth;
	inline static jmethodID entityLivingBase_canEntityBeSeen;

	// PLAYER CONTROLLER M P CLASS
	inline static jclass playerControllerMP_class;
	inline static jfieldID playerControllerMP_blockHitDelay;
	inline static jfieldID playerControllerMP_curBlockDamageMP;
	inline static jfieldID playerControllerMP_isHittingBlock;

	// RENDER MANAGER CLASS
	inline static jclass renderManager_class;
	inline static jfieldID renderManager_instance;
	inline static jfieldID renderManager_renderPosX;
	inline static jfieldID renderManager_renderPosY;
	inline static jfieldID renderManager_renderPosZ;
	inline static jfieldID renderManager_viewerPosX;
	inline static jfieldID renderManager_viewerPosY;
	inline static jfieldID renderManager_viewerPosZ;

	// ACTIVE RENDER INFO CLASS
	inline static jclass activeRenderInfo_class;
	inline static jfieldID activeRenderInfo_Projection;
	inline static jfieldID activeRenderInfo_ModelView;

	// AXIS ALIGNED BB CLASS
	inline static jclass axisAlignedBB_class;
	inline static jfieldID axisAlignedBB_minX;
	inline static jfieldID axisAlignedBB_minY;
	inline static jfieldID axisAlignedBB_minZ;
	inline static jfieldID axisAlignedBB_maxX;
	inline static jfieldID axisAlignedBB_maxY;
	inline static jfieldID axisAlignedBB_maxZ;

	// MOVING OBJECT POSITION CLASS
	inline static jclass movingObjectPosition_class;
	inline static jfieldID movingObjectPosition_hitVec;
	inline static jfieldID movingObjectPosition_typeOfHit;

	// VECTOR
	inline static jclass vec3_class;
	inline static jfieldID vec3_xCoord;
	inline static jfieldID vec3_yCoord;
	inline static jfieldID vec3_zCoord;

	// INVENTORY AND ITEM_STACK
	inline static jclass inventoryPlayer_class;
	inline static jmethodID inventoryPlayer_getCurrentItem;

	inline static jclass itemStack_class;
	inline static jmethodID itemStack_getItem;

	static void Initialize() {


		// -------- Entity --------
		Java::AssignClass("net.minecraft.entity.Entity", entity_class);
		if (Java::Version == MAJOR_1_7_10) {
			entity_getName = Java::Env->GetMethodID(entity_class, "getCommandSenderName", "()Ljava/lang/String;");
		}
		else {
			entity_getName = Java::Env->GetMethodID(entity_class, "getName", "()Ljava/lang/String;");
		}

		entity_isSneaking = Java::Env->GetMethodID(entity_class, "isSneaking", "()Z");
		entity_isSprinting = Java::Env->GetMethodID(entity_class, "isSprinting", "()Z");
		entity_setSprinting = Java::Env->GetMethodID(entity_class, "setSprinting", "(Z)V");

		entity_isInvisibleToPlayer = Java::Env->GetMethodID(entity_class, "isInvisibleToPlayer", "(Lnet/minecraft/entity/player/EntityPlayer;)Z");

		entity_isOnGround = Java::Env->GetFieldID(entity_class, "onGround", "B");
		 
		entity_posX = Java::Env->GetFieldID(entity_class, "posX", "D");
		entity_posY = Java::Env->GetFieldID(entity_class, "posY", "D");
		entity_posZ = Java::Env->GetFieldID(entity_class, "posZ", "D");

		entity_motionX = Java::Env->GetFieldID(entity_class, "motionX", "D");
		entity_motionY = Java::Env->GetFieldID(entity_class, "motionY", "D");
		entity_motionZ = Java::Env->GetFieldID(entity_class, "motionZ", "D");
		
		entity_lastTickPosX = Java::Env->GetFieldID(entity_class, "lastTickPosX", "D");
		entity_lastTickPosY = Java::Env->GetFieldID(entity_class, "lastTickPosY", "D");
		entity_lastTickPosZ = Java::Env->GetFieldID(entity_class, "lastTickPosZ", "D");

		entity_width = Java::Env->GetFieldID(entity_class, "width", "F");
		entity_height = Java::Env->GetFieldID(entity_class, "height", "F");
		entity_distanceWalkedModified = Java::Env->GetFieldID(entity_class, "distanceWalkedModified", "F");
		entity_prevDistanceWalkedModified = Java::Env->GetFieldID(entity_class, "prevDistanceWalkedModified", "F");
		entity_rotationYaw = Java::Env->GetFieldID(entity_class, "rotationYaw", "F");
		entity_rotationPitch = Java::Env->GetFieldID(entity_class, "rotationPitch", "F");
		entity_prevRotationYaw = Java::Env->GetFieldID(entity_class, "prevRotationYaw", "F");
		entity_prevRotationPitch = Java::Env->GetFieldID(entity_class, "prevRotationPitch", "F");
		entity_boundingBox = Java::Env->GetFieldID(entity_class, "boundingBox", "Lnet/minecraft/util/AxisAlignedBB;");

		// -------- EntityLivingBase --------
		Java::AssignClass("net.minecraft.entity.EntityLivingBase", entityLivingBase_class);
		entityLivingBase_hurtTime = Java::Env->GetFieldID(entityLivingBase_class, "hurtTime", "I");
		entityLivingBase_maxHurtTime = Java::Env->GetFieldID(entityLivingBase_class, "maxHurtTime", "I");
		entityLivingBase_jump = Java::Env->GetMethodID(entityLivingBase_class, "jump", "()V");
		entityLivingBase_getHealth = Java::Env->GetMethodID(entityLivingBase_class, "getHealth", "()F");
		entityLivingBase_getMaxHealth = Java::Env->GetMethodID(entityLivingBase_class, "getMaxHealth", "()F");
		entityLivingBase_canEntityBeSeen = Java::Env->GetMethodID(entityLivingBase_class, "canEntityBeSeen", "(Lnet/minecraft/entity/Entity;)Z");

		// -------- Player Controller --------
		Java::AssignClass("net.minecraft.client.multiplayer.PlayerControllerMP", playerControllerMP_class);
		playerControllerMP_blockHitDelay = Java::Env->GetFieldID(playerControllerMP_class, "blockHitDelay", "I");
		playerControllerMP_curBlockDamageMP = Java::Env->GetFieldID(playerControllerMP_class, "curBlockDamageMP", "F");
		playerControllerMP_isHittingBlock = Java::Env->GetFieldID(playerControllerMP_class, "isHittingBlock", "Z");

		// -------- ActiveRenderInfo --------
		Java::AssignClass("net.minecraft.client.renderer.ActiveRenderInfo", activeRenderInfo_class);
		if (Java::Version == MAJOR_1_7_10) {
			activeRenderInfo_Projection = Java::Env->GetStaticFieldID(activeRenderInfo_class, "projection", "Ljava/nio/FloatBuffer;");
			activeRenderInfo_ModelView = Java::Env->GetStaticFieldID(activeRenderInfo_class, "modelview", "Ljava/nio/FloatBuffer;");
		}
		else {
			activeRenderInfo_Projection = Java::Env->GetStaticFieldID(activeRenderInfo_class, "PROJECTION", "Ljava/nio/FloatBuffer;");
			activeRenderInfo_ModelView = Java::Env->GetStaticFieldID(activeRenderInfo_class, "MODELVIEW", "Ljava/nio/FloatBuffer;");
		}

		// -------- RenderManager --------
		Java::AssignClass("net.minecraft.client.renderer.entity.RenderManager", renderManager_class);
		if (Java::Version == MAJOR_1_7_10) {
			renderManager_instance = Java::Env->GetStaticFieldID(renderManager_class, "instance", "Lnet/minecraft/client/renderer/entity/RenderManager;");
			renderManager_renderPosX = Java::Env->GetStaticFieldID(renderManager_class, "renderPosX", "D");
			renderManager_renderPosY = Java::Env->GetStaticFieldID(renderManager_class, "renderPosY", "D");
			renderManager_renderPosZ = Java::Env->GetStaticFieldID(renderManager_class, "renderPosZ", "D");
		}
		else {
			renderManager_instance = Java::Env->GetStaticFieldID(renderManager_class, " ", " ");
			renderManager_renderPosX = Java::Env->GetFieldID(renderManager_class, "renderPosX", "D");
			renderManager_renderPosY = Java::Env->GetFieldID(renderManager_class, "renderPosY", "D");
			renderManager_renderPosZ = Java::Env->GetFieldID(renderManager_class, "renderPosZ", "D");
		}

		renderManager_viewerPosX = Java::Env->GetFieldID(renderManager_class, "viewerPosX", "D");
		renderManager_viewerPosY = Java::Env->GetFieldID(renderManager_class, "viewerPosY", "D");
		renderManager_viewerPosZ = Java::Env->GetFieldID(renderManager_class, "viewerPosZ", "D");

		// -------- AxisAlignedBB --------
		Java::AssignClass("net.minecraft.util.AxisAlignedBB", axisAlignedBB_class);
		axisAlignedBB_minX = Java::Env->GetFieldID(axisAlignedBB_class, "minX", "D");
		axisAlignedBB_minY = Java::Env->GetFieldID(axisAlignedBB_class, "minY", "D");
		axisAlignedBB_minZ = Java::Env->GetFieldID(axisAlignedBB_class, "minZ", "D");
		axisAlignedBB_maxX = Java::Env->GetFieldID(axisAlignedBB_class, "maxX", "D");
		axisAlignedBB_maxY = Java::Env->GetFieldID(axisAlignedBB_class, "maxY", "D");
		axisAlignedBB_maxZ = Java::Env->GetFieldID(axisAlignedBB_class, "maxZ", "D");

		// -------- MovingObjectPosition --------
		Java::AssignClass("net.minecraft.util.MovingObjectPosition", movingObjectPosition_class);
		movingObjectPosition_hitVec = Java::Env->GetFieldID(movingObjectPosition_class, "hitVec", "Lnet/minecraft/util/Vec3;");
		movingObjectPosition_typeOfHit = Java::Env->GetFieldID(movingObjectPosition_class, "typeOfHit", "Lnet/minecraft/util/MovingObjectPosition$MovingObjectType;");

		// -------- Vec3 --------
		Java::AssignClass("net.minecraft.util.Vec3", vec3_class);
		vec3_xCoord = Java::Env->GetFieldID(vec3_class, "xCoord", "D");
		vec3_yCoord = Java::Env->GetFieldID(vec3_class, "yCoord", "D");
		vec3_zCoord = Java::Env->GetFieldID(vec3_class, "zCoord", "D");

		// -------- Inventory & ItemStack --------
		Java::AssignClass("net.minecraft.entity.player.InventoryPlayer", inventoryPlayer_class);
		inventoryPlayer_getCurrentItem = Java::Env->GetMethodID(inventoryPlayer_class, "getCurrentItem", "()Lnet/minecraft/item/ItemStack;");

		Java::AssignClass("net.minecraft.item.ItemStack", itemStack_class);
		itemStack_getItem = Java::Env->GetMethodID(itemStack_class, "getItem", "()Lnet/minecraft/item/Item;");

		initialized = true;
	}
	 
	static void DeleteRefs() {
		Java::Env->DeleteLocalRef(entity_class);
		Java::Env->DeleteLocalRef(entityLivingBase_class);
		Java::Env->DeleteLocalRef(playerControllerMP_class);
		Java::Env->DeleteLocalRef(activeRenderInfo_class);
		Java::Env->DeleteLocalRef(axisAlignedBB_class);
		Java::Env->DeleteLocalRef(movingObjectPosition_class);
		Java::Env->DeleteLocalRef(inventoryPlayer_class);
		Java::Env->DeleteLocalRef(itemStack_class);
		Java::Env->DeleteLocalRef(vec3_class);
	}
};