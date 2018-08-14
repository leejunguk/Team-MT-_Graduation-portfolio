#pragma once

const float VIEW_RADIUS = 1000;
const float MONSTER_RADIUS = 1000;

const int E_RECV = 1;
const int E_SEND = 2;
const int NPC_MOVE = 3;

const float NPC_MOVE_DISTANCE = 1;

const float OBJECT_SPEED = 1;

const float MONSTER_SPEED = 1;

typedef struct Npc_event
{
	UINT id; //오브젝트 ID
	int type; //EVENT의 타입w
	UINT time; //시간
	UINT target;
}NPC_EVENT;

//priority_queue 사용을 위한 비교연산자.(NPC_EVENT를 비교하기 위함)
class Comparison
{
public:
	bool operator() (const NPC_EVENT& e1, const NPC_EVENT& e2)
	{
		return (e1.time > e2.time);
	}
};

// 씬 진호
enum SCENE
{
	STAGE_ONE = 0,
	STAGE_TWO = 1,
	STAGE_THREE = 2
};