#include "stdafx.h"
#include "MonsterAI.h"
#include "cObject.h"
#include "cMonster.h"


/*==================================================*/
Ideal * Ideal::Instance()
{
	static Ideal instance;
	return &instance;
}

void Ideal::Enter(cObject* object)
{
	ideal_time = GetCurTime();
}


void Ideal::Excute(cObject* object,  float *temp_x, float *temp_z)
{
	/*Add_idealTime();
	if (ideal_time >= 5000) {
		object->ChangeState(Move::Instance());
		ideal_time = 0;
		return;
	}

	if (object->Can_see(1, 20)) {
		object->ChangeState(Move::Instance());
		std::cout << "시야 내 : -> move \n";
	}
	else {
		std::cout << "시야 외 : ideal \n";
	}*/
}


void Ideal::Exit(cObject* cObject)
{
}

//
//
void Ideal::Add_idealTime()
{
	ideal_time = GetCurTime() + ideal_time;
}

/*==================================================*/

//Attack
/*==================================================*/
Attack * Attack::Instance()
{
	static Attack instance;
	return &instance;
}

void Attack::Enter(cObject* object)
{
	//object->Initialize();
	attack_time = 0.f;
}


void Attack::Excute(cObject* object, float *temp_x, float *temp_z)
{
	Add_time(&attack_time);
	if (attack_time > 500) {
		attack_time = 0.f;
		object->ChangeState(Move::Instance());
	}
}


void Attack::Exit(cObject* cObject)
{
}

/*==================================================*/


//BeAttack
/*==================================================*/
BeAttack * BeAttack::Instance()
{
	static BeAttack instance;
	return &instance;
}

void BeAttack::Enter(cObject* object)
{
}


void BeAttack::Excute(cObject* object, float *temp_x, float *temp_z)
{
}


void BeAttack::Exit(cObject* cObject)
{
}

/*==================================================*/


//Move
/*==================================================*/
Move * Move::Instance()
{
	static Move instance;
	return &instance;
}

void Move::Enter(cObject* object)
{
	move_time = 0.f;
	//object->Initialize();
}


void Move::Excute(cObject* object, float *temp_x, float *temp_z)
{
	object->Move(temp_x, temp_z, MONSTER_SPEED);
	Add_time(&move_time);

	if (move_time > 500) {
		move_time = 0.f;
		object->ChangeState(Attack::Instance());
	}
}


void Move::Exit(cObject* object)
{
}

void Move::Initialize(cObject *object)
{
	obj_target = object;
}

/*==================================================*/

//Rush
/*==================================================*/
Rush * Rush::Instance()
{
	static Rush instance;
	return &instance;
}

void Rush::Enter(cObject* object)
{
	rush_time = 0.f;
}

void Rush::Excute(cObject* object, float *temp_x, float *temp_z)
{
	Add_time(&rush_time);
	object->Move(temp_x, temp_z, MONSTER_SPEED*2);
	if (rush_time>100) {
		rush_time = 0.f;
		object->ChangeState(Move::Instance());
	}
}


void Rush::Exit(cObject* object)
{
}
/*==================================================*/


//Wander
/*==================================================*/
Wander * Wander::Instance()
{
	static Wander instance;
	return &instance;
}

void Wander::Enter(cObject* object)
{
	radian = 0.f;
	t_wander = 0.f;
	cur_x = object->GetX();
	cur_z = object->GetZ();
}


void Wander::Excute(cObject* object, float *temp_x, float *temp_z)
{
	Add_time(&t_wander);
	radian += (0.1f*GetCurTime()*0.000001);

	*temp_x = mid_x + cur_x*cos(radian*3.141592);
	*temp_z = mid_z + cur_z*sin(radian*3.141592);
	object->Move(temp_x, temp_z, MONSTER_SPEED/2);

	if (t_wander>100) {
		t_wander = 0.f;
		object->ChangeState(Move::Instance());
	}
}


void Wander::Exit(cObject* object)
{
}

void Wander::Initialize(cObject *player)
{
	mid_x = player->GetX();
	mid_z = player->GetZ();

	//radian = atan2(cur_x-mid_x, cur_z - mid_z);
	radian = atan2(cur_z - mid_z, cur_x - mid_x);
}

void Wander::Round()
{
}

/*==================================================*/

