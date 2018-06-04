#pragma once
#include "cObject.h"

class cMonster : public cObject
{
	float init_x;
	float init_z;
	int hp;

	volatile bool is_Active;
public:
	cMonster();
	virtual ~cMonster();
public:
	void HearBeat();
	void WakeUpMonster();
public:
	void SetInitX(float x) { init_x = x; }
	void SetInitZ(float z) { init_z = z; }
};