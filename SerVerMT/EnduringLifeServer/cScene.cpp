#include "stdafx.h"
#include "cScene.h"
#include "cZone.h"
#include "cObject.h"
#include "cSector.h"
#include "cMyNamespace.h"

cScene::cScene()
{
}

cScene::~cScene()
{
}

void cScene::UpdateZone(const UINT id)
{
	cObject* object = objects[id];
	if (object->GetScene() == TOWN)
		object->SetZone(&village);
	if (object->GetScene() == FIELD)
		object->SetZone(&battle);
	object->GetCurrZone()->UpdateSector(id);
}
