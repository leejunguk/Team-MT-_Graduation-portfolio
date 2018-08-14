#pragma once
#include <chrono>

using namespace std;
using namespace chrono;


/*
	ó�� �ؾ��� ����
	=> ����ȭ
		1. ���� ���� (�þ� ���� ����) : �̵�
			-->ProcessPack ������ �˻�

		2. ���� ���� & ���� ���� �� : ����
			-->���� viewlist�� ����� : ProcessPack ������
			-->timer���� �������� Attack ���ݻ��� Attack�̸� 2�ʸ��� ���� ����� ���ŵǰ� ��.

	=> �˹�
		3. ������ ���� ��� : �ǰ�
			--> ������ ������ �ǰ� ����� �ϰ� 3�ʵ��� �ڷ� �̵�
				�ǰݽ� WakeUP �ϰ� ProcessPack���� �����ð� ���� �ڷ� �и�������.

		CurrentState	���ݻ���
		PrevState		���� ����
						��������
						���ݹ���
						���� viewlist
*/
class cObject;

/*==================================================*/
class BaseState {

public:
	virtual void Enter(cObject*) = 0;
	virtual void Excute(cObject* object, float *temp_x, float *temp_z) = 0;
	virtual void Exit(cObject*) = 0;
	virtual void Initialize(cObject* target) = 0;
};

/*==================================================*/
class Ideal : public BaseState {
private:
	unsigned int ideal_time;

public:
	static Ideal* Instance();

	virtual void Enter(cObject*);
	virtual void Excute(cObject* object, float *temp_x, float *temp_z);
	virtual void Exit(cObject*);
	virtual void Initialize(cObject* target) {}
public:
	void Add_idealTime();
};


/*==================================================*/
class Attack : public BaseState {
	float attack_time = 0.f;
public:

	static Attack* Instance();

	virtual void Enter(cObject*);
	virtual void Excute(cObject* object, float *temp_x, float *temp_z);
	virtual void Exit(cObject*);
	virtual void Initialize(cObject* target) {}
};

/*==================================================*/
class BeAttack : public BaseState {

public:

	static BeAttack* Instance();

	virtual void Enter(cObject*);
	virtual void Excute(cObject* object, float *temp_x, float *temp_z);
	virtual void Exit(cObject*);
	virtual void Initialize(cObject* target) {}
};


/*==================================================*/
class Move : public BaseState {

	cObject* obj_target;
	float move_time = 0.f;
public:
	static Move* Instance();

	virtual void Enter(cObject*);
	virtual void Excute(cObject* object, float *temp_x, float *temp_z);
	virtual void Exit(cObject*);

public:
	void Initialize(cObject*);
};

/*==================================================*/
class Rush : public BaseState {
	float rush_time = 0.f;
public:

	static Rush* Instance();

	virtual void Enter(cObject*);
	virtual void Excute(cObject* object, float *temp_x, float *temp_z);
	virtual void Exit(cObject*);
	virtual void Initialize(cObject* target) {}
};


/*==================================================*/
class Wander : public BaseState {

	float mid_x, mid_z;
	float cur_x, cur_z;
	float radian = 0.f;

	float t_wander = 0.f;

public:
	static Wander* Instance();

	virtual void Enter(cObject*);
	virtual void Excute(cObject* object, float *temp_x, float *temp_z);
	virtual void Exit(cObject*);

	virtual void Initialize(cObject*);
	void Round();
};

/*===========================================================*/
/*===========================================================*/
static UINT GetCurTime()
{
	static auto start = high_resolution_clock::now();

	return duration_cast<milliseconds>(high_resolution_clock::now() - start).count();
}

static void Add_time(float *time) {
	(*time) = ((GetCurTime()*0.00001f) + (*time));
}


