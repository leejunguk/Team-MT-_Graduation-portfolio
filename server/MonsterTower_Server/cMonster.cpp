#include "stdafx.h"
#include "cMonster.h"
#include "cMyNamespace.h"
#include "cThreadFunc.h"
#include "cPlayer.h"

cMonster::cMonster()
{
	my_sector = nullptr;
	is_Active = false;
	target_id = -1;

	m_pPrevState = Ideal::Instance();
	m_pCurrentState = Ideal::Instance();
	m_pCurrentState->Enter(this);

	hp = 100.f;
}

cMonster::~cMonster()
{
	
}

void cMonster::HearBeat(const UINT target_id)
{
	bool is_move = false;
	bool search_near_player = false;
	int near_player = target_id;

	UpdatesSectorSearch();
	unordered_set<UINT> player_list = near_list;
	//������ �þ߸� ���ϴ� ����
	for (auto temp_user : player_list) {
		float temp_distance = sqrtf((x - objects[temp_user]->GetX())*(x - objects[temp_user]->GetX()) +
			(z - objects[temp_user]->GetZ())*(z - objects[temp_user]->GetZ()));
		if (temp_distance < MONSTER_RADIUS + 100) {
			search_near_player = true;
			near_player = temp_user;

			float temp_x = objects[near_player]->GetX() - x;
			float temp_z = objects[near_player]->GetZ() - z;
			float size_vector = sqrtf((temp_x * temp_x) + (temp_z * temp_z));

			mtx.lock();
			sight_x = temp_x / size_vector;
			sight_z = temp_z / size_vector;
			mtx.unlock();
		}
	}
	if (search_near_player)
		is_move = true;
	else
		is_move = false;



	/*============================================================*/
	EventSendProcess(near_player, GetAnimNum());
	mtx.lock();
	GetCurState()->Excute(this);
	mtx.unlock();
	//����ó�� (������)
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
			SendMoveObject(prev, id);

		//temp�� ������
		if(temp.count(prev) == false)
			SendRemoveObject(prev, id);
	}
	//���� ���� �÷��̾�ó��
	for (auto curr : temp)
	{
		cPlayer* player = dynamic_cast<cPlayer*>(objects[curr]);

		if (player->GetIsUse() == false)
			continue;

		if (0 == view.count(curr))
			SendPutObject(curr, id);
	}



	//�ֺ��� �÷��̾ ������
	if (temp.empty() == false){
		AddTimer(id, near_player, NPC_ACTIVE, 10);
		return;
	}

	for (auto temp_user : near_list) {
		if (temp_user >= MAX_USER) continue;
		if (!CanSee(id, temp_user)) {
			mtx.lock();
			ChangeState(Ideal::Instance());
			mtx.unlock();
		}
		cs_packet_animation p;
		p.type = SC_ANIM;
		p.id = id;
		p.x = x;
		p.z = z;
		p.target_id = near_player;
		p.hp = hp;
		p.size = sizeof(p);
		p.anim_num = GetAnimNum();
		SendPacket(temp_user, &p);
	}
	is_Active = false;
}


void cMonster::WakeUpMonster(UINT temp_id)
{
	if (true == is_Active) 
		return;
	is_Active = true;

	AddTimer(id, temp_id, NPC_ACTIVE, 200);
}

void cMonster::EventSendProcess(int target, int AnimNum)
{
	//�ִϸ��̼��� �ٲ𶧸� ó��
	if (false == state_change)
	{
		mtx.lock();
		GetCurState()->Initialize(objects[target]);
		SetStateChange(true);
		mtx.unlock();

		cs_packet_npc_anim p_anim;
		p_anim.type = SC_ANIM;
		p_anim.id = id;
		p_anim.anim_num = AnimNum;
		p_anim.size = sizeof(p_anim);
		for (auto temp_user : near_list) {
			if (temp_user >= MAX_USER) continue;
			SendPacket(temp_user, &p_anim);
		}
		if (AnimNum == NPC_ATTACK) {
			p_anim.id = target;
			p_anim.anim_num = 6;
			for (auto temp_user : near_list) {
				if (temp_user >= MAX_USER) continue;
					SendPacket(temp_user, &p_anim);
			}
		}
	}

	//�̵�, ü�� ��Ŷó��
	switch (AnimNum)
	{
	case NPC_SKILL:
	case NPC_MOVE:
		sc_packet_pos p_pos;
		p_pos.type = SC_POS;
		p_pos.id = id;
		p_pos.x;
		p_pos.y;
		p_pos.sight_x = sight_x*100;
		p_pos.sight_z = sight_z*100;
		p_pos.size = sizeof(p_pos);
		for (auto temp_user : near_list) {
			if (temp_user >= MAX_USER) continue;
			SendPacket(temp_user, &p_pos);
		}
		break;
	case NPC_ATTACK:
		cs_packet_npc_condition p_attck;
		p_attck.type = SC_CONDITION;
		p_attck.id = target;
		p_attck.hp = objects[target]->GetMyHP();
		p_attck.size = sizeof(p_attck);
		for (auto temp_user : near_list) {
			if (temp_user >= MAX_USER) continue;
			SendPacket(temp_user, &p_attck);
		}
		break;
	case NPC_DAMAGED:
		cs_packet_npc_condition p_con;
		p_con.type = SC_CONDITION;
		p_con.id = id;
		p_con.hp = hp;
		p_con.size = sizeof(p_con);
		for (auto temp_user : near_list) {
			if (temp_user >= MAX_USER) continue;
			SendPacket(temp_user, &p_con);
		}
		break;
	default:
		break;
	}
}
