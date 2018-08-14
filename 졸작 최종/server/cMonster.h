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
	void HearBeat(const UINT target_id);
	void WakeUpMonster(UINT temp_id);
public:
	void SetInitX(float x) { init_x = x; }
	void SetInitZ(float z) { init_z = z; }
};