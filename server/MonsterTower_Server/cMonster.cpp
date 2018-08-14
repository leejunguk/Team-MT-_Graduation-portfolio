#include "stdafx.h"
#include "cMonster.h"
#include "cMyNamespace.h"
#include "cThreadFunc.h"
#include "cPlayer.h"

cMonster::cMonster()
{
	my_sector = nullptr;
	is_Active = false;

	m_pPrevState = Ideal::Instance();
	m_pCurrentState = Ideal::Instance();
	m_pCurrentState->Enter(this);
}

cMonster::~cMonster()
{
	
}

void cMonster::HearBeat(const UINT target_id)
{
	bool is_move = false;
	bool search_near_player = false;
	int near_player = -1;
	float distance = 1000.0f;

	UpdatesSectorSearch();
	

	// near_list ������Ʈ
	float move_x = x;
	float move_z = z;
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


				float temp_x = objects[near_player]->GetX() - x;
				float temp_z = objects[near_player]->GetZ() - z;
				float size_vector = sqrtf((temp_x * temp_x) + (temp_z * temp_z));

				sight_x = temp_x / size_vector;
				sight_z = temp_z / size_vector;

				mtx.lock();
				m_pCurrentState->Excute(this, &move_x, &move_z);
				mtx.unlock();
			}

		}
	}



	if (search_near_player)
	{
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


	//==���� ó��
	//���ݹ���
	//
	if (m_pPrevState == Ideal::Instance() && state_change == false) {
		mtx.lock();
		SetStateChange(true);
		int num = rand() % 2;
		switch (num) {
		case 0:
			ChangeState(Wander::Instance());
			GetCurState()->Initialize(objects[target_id]);
			break;
		case 1:
			ChangeState(Rush::Instance());
			GetCurState()->Initialize(objects[target_id]);
			break;
		}
		mtx.unlock();
	}
	/*============================================================*/

	//����ó�� (������)
	if (state_change == false) {
		SetStateChange(true);
		cs_packet_animation p;
		p.type = SC_ANIM;
		p.id = id;
		p.size = sizeof(p);
		p.anim_num = GetAnimNum();
		SendPacket(target_id, &p);
	}
	/*============================================================*/



	unordered_set<UINT> view;

	//�̵� �� �� ���͸� ã�´�
	GetCurrZone()->UpdateSector(id);

	//view�� ���� near_list���� �ֺ� �÷��̾ �ִ´�.
	view.clear();

	for (auto prev_player : near_list)
	{
		if (prev_player < MAX_USER)
		{
			view.insert(prev_player);
		}
	}

	// near_list ������Ʈ�� �Ѵ�.
	UpdatesSectorSearch();

	//temp�� ������Ʈ�� near_list���� �ֺ� �÷��̾ �ִ´�.
	unordered_set<UINT> temp;
	
	temp.clear();
	
	for (auto curr_player : near_list)
	{
		if (curr_player < MAX_USER)
			temp.insert(curr_player);
	}

	//view : �̵� �� near_list���� �ֺ� �÷��̾ ���� �����̳�
	//temp : �̵� �� near_list���� �ֺ� �÷��̾ ���� �����̳�

	//view�� Ž��
	for (auto prev : view)
	{
		cPlayer* player = dynamic_cast<cPlayer*>(objects[prev]);
		
		if (player->GetIsUse() == false)
			continue;
		//temp�� ������
		if (temp.count(prev) && is_move)
		{
			SendMoveObject(prev, id);
		}
		//temp�� ������

		if(temp.count(prev) == false)
		{
			SendRemoveObject(prev, id);
		}
	}
	//���� ���� �÷��̾�ó��
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

	//�ֺ��� �÷��̾ ������
	if (temp.empty() == false)
	{
		AddTimer(id, target_id, NPC_MOVE, 10);
		return;
	}
	is_Active = false;
}

void cMonster::WakeUpMonster(UINT temp_id)
{
	if (true == is_Active) 
		return;
	is_Active = true;

	AddTimer(id, temp_id, NPC_MOVE, 200);
}
