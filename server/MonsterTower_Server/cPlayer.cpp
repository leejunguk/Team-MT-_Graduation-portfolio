#include "stdafx.h"
#include "cPlayer.h"
#include "cObject.h"
#include "cMonster.h"

#include "cThreadFunc.h"
#include "cMyNameSpace.h"

cPlayer::cPlayer() : packet_size(0), saved_size(0)
{
	ZeroMemory(recv_buf, sizeof(recv_buf));
	ZeroMemory(&exover.wsaover, sizeof(exover.wsaover));

	viewMutex.lock();
	viewlist.clear();
	viewMutex.unlock();
	exover.wsabuf.buf = reinterpret_cast<char*>(recv_buf);
	exover.wsabuf.len = sizeof(recv_buf);
	my_sector = nullptr;
	hp = 100.f;
}

cPlayer::~cPlayer()
{
}

void cPlayer::RecvPacket()
{
	DWORD flags = 0;

	int ret_val = WSARecv(sock, &exover.wsabuf, 1, NULL, &flags, &exover.wsaover, NULL);
	if (ret_val != 0)
	{
		int error = WSAGetLastError();
		if (error != WSA_IO_PENDING)
			cout << " [error] WSARecv() on " << id << " Player::RecvPacket() " << endl;
	}
}

void cPlayer::ReadPacket(const UINT id, const int transferred)
{
	BYTE* ptr = recv_buf;

	int remained = transferred;

	while (0 < remained)
	{
		if (0 == packet_size)
			packet_size = ptr[0];

		int required = packet_size - saved_size;

		if (remained >= required)
		{
			memcpy(packet_buf + saved_size, ptr, required);

			ProcessPacket(id, packet_buf);

			remained -= required;
			ptr += required;

			packet_size = 0;
			saved_size = 0;
		}

		else
		{
			memcpy(packet_buf + saved_size, ptr, remained);

			saved_size += remained;

			remained = 0;
		}
	}
}

void cPlayer::UpdateViewList()
{
	//sector_search�� ������Ʈ �� ����Ʈ
	//viewlist�� ������Ʈ�� ����ϴ� viewlist

	unordered_set<UINT> add_list;
	unordered_set<UINT> remove_list;
	unordered_set<UINT> exist_list;

	//���� ������ ������Ʈ�� add_list�� �߰�
	for (auto add_player : near_list)
	{
		if (0 == viewlist.count(add_player))
		{
			add_list.insert(add_player);
		}
	}
	//������ �÷��̾ remove_list�� �߰�
	for (auto remove_player : viewlist)
	{
		if (0 == near_list.count(remove_player))
		{
			remove_list.insert(remove_player);
		}
	}
	//���� �־���, ���ݵ� �ִ� �÷��̾���� exist_list�� �߰�
	for (auto exist_player : near_list)
	{
		if (viewlist.count(exist_player))
		{
			exist_list.insert(exist_player);
		}
	}

	//1. ���� ���� �÷��̾� ó��
	for (auto add : add_list)
	{
		SendPutObject(id, add);

		//viewlist�� ���� �÷��̾ �־��ش�.
		viewMutex.lock();
		viewlist.insert(add);
		viewMutex.unlock();
		
		//��� �丮��Ʈ�� ������ �־��ְ�, ������ �÷��̾��� �� ������ �����ش�.
		if (add < MAX_USER)
		{
			cPlayer* other = dynamic_cast<cPlayer*>(objects[add]);

			other->viewMutex.lock();
			if (0 == other->GetViewlist().count(id))
			{
				other->GetViewlist().insert(id);
				other->viewMutex.unlock();
				SendPutObject(add, id);
			}
			//��� �丮��Ʈ�� ������ ���� ������ ������ �����ش�.
			else
			{
				other->viewMutex.unlock();
				SendMoveObject(add, id);
			}
		}
		else if (add >= MAX_USER && add < MAX_OBJECT_INDEX) //���� �̸� �����ش�.
		{
			cMonster* monster = dynamic_cast<cMonster*>(objects[add]);

			monster->WakeUpMonster(id);
		}
	}
	//2. �̹� �����ϴ� �÷��̾�� ó��
	for (auto exist : exist_list)
	{
		SendMoveObject(id, exist);

		if (exist < MAX_USER)
		{
			cPlayer* other = dynamic_cast<cPlayer*>(objects[exist]);
			//��� �丮��Ʈ�� ������ ��� �丮��Ʈ�� ���� ���� �־��ְ�, ���� �����Ѱ��� �˸���.
			other->viewMutex.lock();
			if (0 == other->GetViewlist().count(id))
			{
				other->GetViewlist().insert(id);
				other->viewMutex.unlock();
				SendPutObject(exist, id);
			}
			//��� �丮��Ʈ�� ������, ���� ������ ������ �����ش�.
			else
			{
				other->viewMutex.unlock();
				SendMoveObject(exist, id);
			}
		} 
	}
	//3. ������ �÷��̾�� ó��
	for (auto remove : remove_list)
	{
		//�� �丮��Ʈ���� ���Ž�Ű��, ������ �� �÷��̾ ���Ž�Ų�ٴ� �޼����� ������.
		viewMutex.lock();
		viewlist.erase(remove);
		viewMutex.unlock();
		SendRemoveObject(id, remove);

		if (remove < MAX_USER)
		{
			cPlayer* other = dynamic_cast<cPlayer*>(objects[remove]);

			other->viewMutex.lock();
			if (0 != other->GetViewlist().count(id))
			{
				other->GetViewlist().erase(id);
				other->viewMutex.unlock();
				SendRemoveObject(remove, id);
			}
			else
			{
				other->viewMutex.unlock();
			}
		}
	}


	// ViewList = Near_list
	// �� �丮��Ʈ�� ������Ʈ �Ǿ���.
}

bool cPlayer::AttackMonster(int *attack_target)
{
	for (int i = MAX_USER; i < MAX_OBJECT_INDEX; ++i) {
		if (objects[id]->GetScene() != objects[i]->GetScene()) continue;
		if (!CanAttack(id, i)) continue;
		NonTartgetStep(i);
		*attack_target = i;
		return true;
	}
	return false;
}

void cPlayer::NonTartgetStep(int i)
{
	objects[i]->ChangeState(BeAttack::Instance());
}

void cPlayer::Clear()
{
	for (auto send : viewlist)
	{
		if (send < MAX_USER)
		{
			cPlayer* other = dynamic_cast<cPlayer*>(objects[send]);

			if (0 != other->GetViewlist().count(id))
			{
				other->viewMutex.lock();
				other->GetViewlist().erase(id);
				other->viewMutex.unlock();
				SendRemoveObject(send, id);
			}
		}
	}
	viewlist.clear();
	if (my_sector) {
		my_sector->Remove(id);
	}
}
