#pragma once

#include "../../../java/IClass.h"
#include "multiplayer/PlayerControllerMP.h"
#include "../entity/EntityLivingBase.h"
#include "../client/entity/EntityPlayerSP.h"
#include "multiplayer/WorldClient.h"
#include "renderer/ActiveRenderInfo.h"
#include "renderer/entity/RenderManager.h"
#include "../util/Timer.h"
#include "settings/GameSettings.h"
#include "../util/MovingObjectPosition.h"
#include "../../../strayCache.h"

struct CMinecraft : IClass
{
	CMinecraft();

	jclass GetClass();
	jobject GetInstance();

	CEntity GetRenderViewEntity();


	bool IsInGuiState();
	void ClickMouse();
	int getRightClickDelayTimer();
	void setRightClickDelayTimer(int);
	void setLeftClickCounter(int);

	CMovingObjectPosition GetMouseOver();


	CEntityPlayerSP* thePlayer;
	CPlayerControllerMP* playerController;
	CWorldClient* theWorld;
	CActiveRenderInfo* activeRenderInfo;
	CRenderManager* renderManager;
	CTimer* timer;
	CGameSettings* gameSettings;
};

