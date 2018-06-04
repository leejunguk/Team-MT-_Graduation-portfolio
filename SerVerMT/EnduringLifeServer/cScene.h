#pragma once

#include "cZone.h"

class cScene
{
	cZone village;
	cZone battle;
public:
	cScene();
	~cScene();
public:
	void UpdateZone(const UINT id);
};