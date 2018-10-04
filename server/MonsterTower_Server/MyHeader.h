#pragma once

const float VIEW_RADIUS = 1500;
const float MONSTER_ATTACK_RADIUS = 300;
const float MONSTER_RADIUS = 1400;

const int E_RECV = 1;
const int E_SEND = 2;


const int NPC_ACTIVE = 10;
const int NPC_IDLE = 0;
const int NPC_MOVE = 1;
const int NPC_ATTACK = 2;
const int NPC_DAMAGED = 3;
const int NPC_SKILL = 4;
const int NPC_DIE = 5;
const int NPC_ENDACTIVE = 20;


const float NPC_MOVE_DISTANCE = 1;

const float OBJECT_SPEED = 500;

const float MONSTER_SPEED = 400;

typedef struct Npc_event
{
	UINT id; //������Ʈ ID
	int type; //EVENT�� Ÿ��w
	UINT time; //�ð�
	UINT target;
}NPC_EVENT;

//priority_queue ����� ���� �񱳿�����.(NPC_EVENT�� ���ϱ� ����)
class Comparison
{
public:
	bool operator() (const NPC_EVENT& e1, const NPC_EVENT& e2)
	{
		return (e1.time > e2.time);
	}
};

// �� ��ȣ
enum SCENE
{
	STAGE_ONE = 0,
	STAGE_TWO = 1,
	STAGE_THREE = 2
};