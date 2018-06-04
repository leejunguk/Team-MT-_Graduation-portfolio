#include "stdafx.h"
#include "cMonster.h"
#include "cMyNamespace.h"
#include "cThreadFunc.h"
#include "cPlayer.h"

cMonster::cMonster()
{
	my_sector = nullptr;
	is_Active = false;
}

cMonster::~cMonster()
{
	
}

void cMonster::HearBeat()
{
	bool is_move = false;
	bool search_near_player = false;
	int near_player = -1;
	float distance = 1000.0f;

	UpdatesSectorSearch();
	// near_list 업데이트

	unordered_set<UINT> player_list = near_list;

	for (auto player : player_list)
	{
		if (player < MAX_USER)
		{
			float temp = sqrtf((x - objects[player]->GetX())*(x - objects[player]->GetX()) +
				(z - objects[player]->GetZ())*(z - objects[player]->GetZ()));

			if (temp < MONSTER_RADIUS && temp < distance)
			{
				search_near_player = true;
				distance = temp;
				near_player = player;
			}
		}
	}


	float move_x = x;
	float move_z = z;

	if (search_near_player)
	{
		float temp_x = objects[near_player]->GetX() - x;
		float temp_z = objects[near_player]->GetZ() - z;
		float size_vector = sqrtf((temp_x * temp_x) + (temp_z * temp_z));

		sight_x = temp_x / size_vector;
		sight_z = temp_z / size_vector;

		if (distance > 10.0)
		{
			move_x = move_x + sight_x * MONSTER_SPEED;
			move_z = move_z + sight_z * MONSTER_SPEED;
		}
		is_move = true;
	}
	else
	{
		is_move = false;
	}


	if (move_x >= 0.0f && move_x < BOARD_WIDTH && move_z >= 0.0f && move_z < BOARD_HEIGHT && is_move)
	{
		x = move_x;
		z = move_z;
	}
	else
	{
		sight_x = 0.0f;
		sight_z = -1.0f;
	}

	unordered_set<UINT> view;

	//이동 한 후 섹터를 찾는다
	GetCurrZone()->UpdateSector(id);

	//view에 기존 near_list에서 주변 플레이어만 넣는다.
	view.clear();

	for (auto prev_player : near_list)
	{
		if (prev_player < MAX_USER)
		{
			view.insert(prev_player);
		}
	}

	// near_list 업데이트를 한다.
	UpdatesSectorSearch();

	//temp에 업데이트된 near_list에서 주변 플레이어만 넣는다.
	unordered_set<UINT> temp;
	
	temp.clear();
	
	for (auto curr_player : near_list)
	{
		if (curr_player < MAX_USER)
			temp.insert(curr_player);
	}

	//view : 이동 전 near_list에서 주변 플레이어만 넣은 컨테이너
	//temp : 이동 후 near_list에서 주변 플레이어만 넣은 컨테이너

	//view를 탐색
	for (auto prev : view)
	{
		cPlayer* player = dynamic_cast<cPlayer*>(objects[prev]);
		
		if (player->GetIsUse() == false)
			continue;
		//temp에 있으면
		if (temp.count(prev) && is_move)
		{
			SendMoveObject(prev, id);
		}
		//temp에 없으면

		if(temp.count(prev) == false)
		{
			SendRemoveObject(prev, id);
		}
	}
	//새로 들어온 플레이어처리
	for (auto curr : temp)
	{
		cPlayer* player = dynamic_cast<cPlayer*>(objects[curr]);

		if (player->GetIsUse() == false)
			continue;

		if (0 == view.count(curr))
		{
			SendPutObject(curr, id);
		}
	}

	//주변에 플레이어가 있으면
	if (temp.empty() == false)
	{
		AddTimer(id, NPC_MOVE, 10);
		return;
	}
	is_Active = false;
}

void cMonster::WakeUpMonster()
{
	if (true == is_Active) 
		return;
	is_Active = true;

	AddTimer(id, NPC_MOVE, 200);
}
