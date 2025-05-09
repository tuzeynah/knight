#pragma once

#include "../../../../java/IClass.h"
#include "../../item/ItemStack.h"
#include "../../../../strayCache.h"

struct CInventoryPlayer : IClass
{
	CInventoryPlayer();
	CInventoryPlayer(jobject instance);

	jclass GetClass();
	jobject GetInstance();

	CItemStack GetCurrentItem();
};