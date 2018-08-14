#include "stdafx.h"
#include "cThreadFunc.h"

#include "Overlapped_Ex.h"
#include "cMyNamespace.h"

#include "cObject.h"
#include "cPlayer.h"
#include "cMonster.h"

void AcceptThread()
{
	WSADATA	wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	auto g_socket = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	SOCKADDR_IN bind_addr;
	ZeroMemory(&bind_addr, sizeof(SOCKADDR_IN));
	bind_addr.sin_family = AF_INET;
	bind_addr.sin_port = htons(MY_SERVER_PORT);
	bind_addr.sin_addr.s_addr = INADDR_ANY;

	::bind(g_socket, reinterpret_cast<sockaddr *>(&bind_addr), sizeof(SOCKADDR_IN));

	listen(g_socket, SOMAXCONN);

	UINT new_id = 0;

	while (true) {
		SOCKADDR_IN c_addr;
		ZeroMemory(&bind_addr, sizeof(SOCKADDR_IN));
		c_addr.sin_family = AF_INET;
		c_addr.sin_port = htons(MY_SERVER_PORT);
		c_addr.sin_addr.s_addr = INADDR_ANY;
		int c_length = sizeof(SOCKADDR_IN);

		SOCKET new_socket = WSAAccept(g_socket, reinterpret_cast<sockaddr *>(&c_addr), &c_length, NULL, NULL);

		if (thread_running == false)
		{
			closesocket(new_socket);
			break;
		}

		cout << new_id << " client enter" << endl;

		CreateIoCompletionPort(reinterpret_cast<HANDLE>(new_socket), g_iocp, new_id, 0);

		cPlayer* Player = dynamic_cast<cPlayer*>(objects[new_id]);
		
		Player->SetSock(new_socket);
		Player->SetID(new_id);
		Player->SetScene(STAGE_ONE);
		Player->SetIsUse(true);

		float tempx, tempz;
		sc_packet_put_player p;
		p.id = new_id;
		p.size = sizeof(sc_packet_put_player);
		p.type = SC_PUT_PLAYER;
		
		tempx = (roundf(objects[new_id]->GetX() * 10) / 10) * 10;
		tempz = (roundf(objects[new_id]->GetZ() * 10) / 10) * 10;

		p.x = tempx;
		p.y = tempz;


		//�ʱ�ȭ �� ������ �ڽſ��� ������		
		SendPacket(new_id, &p);

		//���� �丮��Ʈ�� �ۼ�, �� �丮��Ʈ�� ���� �÷��̾�鿡�� �� ������ ����.
		UpdataPlayerView(new_id);

		Player->RecvPacket();
		++new_id;
		// ������ �������� �ٸ� �÷��̾� ������ ����
	}

	closesocket(g_socket);
	WSACleanup();

	cout << " exit accept thread " << endl;
}

void WorkerThread()
{
	while (true) {
		unsigned long data_size;
		unsigned long long key; //unsigend long long 
		EXOver *p_over;

		BOOL is_success = GetQueuedCompletionStatus(g_iocp, &data_size, &key, reinterpret_cast<LPWSAOVERLAPPED *>(&p_over), INFINITE);

		//cout << key << endl;
		//WorkerThread ����
		if (thread_running == false)
			break;
		// ���� ó��
		if (0 == is_success) {
			cout << "Error in GQCS key[" << key << "]\n";
		}
		// �������� ó��
		if (0 == data_size)
		{
			DisconnectPlayer(key);
			continue;
		}
		// Send/Recv ó��
		//EXOver *o = reinterpret_cast<EXOver *>(p_over);
		if (E_RECV == p_over->command)
		{
			AfterRecv(key, data_size);
		}
		else if (E_SEND == p_over->command)
		{
			//cout << "Send Complete to Client : " << key << std::endl;
			delete p_over;
		}
		else if (NPC_MOVE == p_over->command)
		{
			cMonster* monster = dynamic_cast<cMonster*>(objects[key]);

			if (monster->GetIsUse() == false)
				continue;
			monster->HearBeat(p_over->target);
			delete p_over;
		}
		else
		{
			std::cout << "Unknown GQCS Event Type!\n";
			exit(-1);
		}
	}
	cout << " exit worker thread " << endl;
}

void TimerThread()
{
	while (1)
	{
		Sleep(10);

		if (thread_running == false)
			break;

		timerMutex.lock();
		if (event_queue.empty())
		{
			timerMutex.unlock();
			continue;
		}

		NPC_EVENT top_event = event_queue.top();

		while (top_event.time <= GetCurrTime())
		{
			event_queue.pop();
			timerMutex.unlock();

			//�̺�Ʈ ó��
			ProcessEvent(top_event);
			//cout << top_event.time << endl;
			//cout << top_event.id << endl;

			timerMutex.lock();
			if (event_queue.empty())
			{
				timerMutex.unlock();
				break;
			}
			top_event = event_queue.top();
		}
		timerMutex.unlock();
	}
	//cout << " exit timer thread " << endl;

}

void AfterRecv(const UINT id, const int transferred)
{
	cPlayer* player = dynamic_cast<cPlayer*>(objects[id]);

	player->ReadPacket(id, transferred);
	player->RecvPacket();
}

void ProcessPacket(const UINT cl, BYTE * packet)
{
	int packet_type = 0;
	switch(packet[1])
	{
	case CS_UP:
	case CS_DOWN:
	case CS_LEFT:
	case CS_RIGHT:
	case CS_TELEPORT:
		packet_type = 0;
		break;
	case CS_SIGHT:
		packet_type = 1;
		break;
	case CS_ATTACK:
		packet_type = 2;
		break;
	case CS_ANIM:
		packet_type = 3;
	}

	if (packet_type == 3)
	{
		cs_packet_animation *my_packet = reinterpret_cast<cs_packet_animation *>(packet);
		objects[cl]->anim_num = my_packet->anim_num;

		//cout << my_packet->anim_num << endl;

		cPlayer* player = dynamic_cast<cPlayer*>(objects[cl]);

		unordered_set<UINT> temp;

		temp = player->GetViewlist();

		for (auto other : temp)
		{
			if (other < MAX_USER)
			{
				if (objects[cl]->GetID() < MAX_USER)
				{
					cs_packet_animation p;
					p.type = SC_ANIM;
					p.id = cl;
					p.size = sizeof(p);
					p.anim_num = objects[cl]->anim_num;
					SendPacket(other, &p);
				}
			}
		}
		return;
	}
	// ���� ��Ŷ
	if (packet_type == 2)
	{
		//cout << cl << "�� ������Ŷ�� �޾ҽ��ϴ�" << endl;

		cPlayer* player = dynamic_cast<cPlayer*>(objects[cl]);

		player->AttackMonster();


		return;
	}

	//�þ� ��Ŷ
	if (packet_type == 1) 
	{
		//cout << "�þ� ��Ŷ �޾ѽ��ϴ� "<< endl;
		cs_packet_sight * p = reinterpret_cast<cs_packet_sight *>(packet);

		float dirX = p->dir_x * 0.01f; //�÷��̾��� ���⺤�� x��
		float dirZ = p->dir_z * 0.01f; //�÷��̾��� ���⺤�� z��

		objects[cl]->SetSightX(dirX);
		objects[cl]->SetSightZ(dirZ);

		cPlayer* player = dynamic_cast<cPlayer*>(objects[cl]);

		unordered_set<UINT> temp;

		temp = player->GetViewlist();

		for (auto other : temp)
		{
			if (other < MAX_USER)
			{
				if (objects[cl]->GetID() < MAX_USER)
					SendMoveObject(other, cl);
			}
		}
		return;
	}

	cs_packet_move *p = reinterpret_cast<cs_packet_move *>(packet);

	float x = objects[cl]->GetX();
	float y = objects[cl]->GetZ();

	float dirX = p->dir_x * 0.01f; //�÷��̾��� ���⺤�� x��
	float dirZ = p->dir_z * 0.01f; //�÷��̾��� ���⺤�� z��


	switch (p->type)
	{
	case CS_UP:
		x = x + dirX * OBJECT_SPEED;
		y = y + dirZ * OBJECT_SPEED;
		break;
	case CS_DOWN:
		x = x - dirX * OBJECT_SPEED;
		y = y - dirZ * OBJECT_SPEED;

		break;
	case CS_LEFT:
		x = x - dirZ * OBJECT_SPEED;
		y = y + dirX * OBJECT_SPEED;

		break;
	case CS_RIGHT:
		x = x + dirZ * OBJECT_SPEED;
		y = y - dirX * OBJECT_SPEED;
		break;
	case CS_TELEPORT:
		x = x + dirX * OBJECT_SPEED * 10;
		y = y + dirZ * OBJECT_SPEED * 10;
		break;
	case CS_NEXTSTAGE:
		objects[cl]->SetScene(STAGE_TWO);
		objects[cl]->SetX(BOARD_WIDTH / 2);
		objects[cl]->SetZ(900);
		
		
		objects[cl]->SetSightX(dirX);
		objects[cl]->SetSightZ(dirZ);

			UpdataPlayerView(cl);

			SendSceneObject(cl, objects[cl]->GetScene());

			SendMoveObject(cl);
		break;
	default:
		cout << "UnKnown Protocol from Client [" << cl << "]" << endl;
	}
	// cout << x << " , " << y << endl;
	// �̵� �Ŀ� �� ���� �����ϸ�.

	//if (CollisionCheck(x, y, BOARD_WIDTH / 2, 50, 15, 15) && objects[cl]->GetScene() == TOWN)
	//{
	//	/*
	//	cout << "�浹�߾�" << endl;
	//	*/
	//	objects[cl]->SetScene(FIELD);
	//	objects[cl]->SetX(BOARD_WIDTH / 2);
	//	objects[cl]->SetZ(900);
	//
	//
	//	objects[cl]->SetSightX(dirX);
	//	objects[cl]->SetSightZ(dirZ);
	//	//��Ż�� ����

	//	UpdataPlayerView(cl);

	//	SendSceneObject(cl, objects[cl]->GetScene());

	//	SendMoveObject(cl);

	//	return;
	//}
	//if (CollisionCheck(x, y, BOARD_WIDTH / 2, 950, 15, 15) && objects[cl]->GetScene() == FIELD)
	//{
	//	/*
	//	cout << "�浹�߾�" << endl;
	//	*/
	//	objects[cl]->SetScene(TOWN);
	//	objects[cl]->SetX(BOARD_WIDTH / 2);
	//	objects[cl]->SetZ(100);
	//	SendSceneObject(cl, objects[cl]->GetScene());
	//

	//	objects[cl]->SetSightX(dirX);
	//	objects[cl]->SetSightZ(dirZ);
	//	//��Ż�� ����

	//	UpdataPlayerView(cl);

	//	SendMoveObject(cl);

	//	return;
	//}


	if (x >= 0.0f && x < BOARD_WIDTH && y >= 0.0f && y < BOARD_HEIGHT)
	{
		cPlayer* player = dynamic_cast<cPlayer*>(objects[cl]);

		unordered_set<UINT> temp;

		temp = player->GetViewlist();

		for (auto other : temp)
		{
			if (CollisionCheck(player->GetX(), player->GetZ(), objects[other]->GetX(), objects[other]->GetZ(), 30, 30))
			{
				//cout << player->GetID() << "�� �÷��̾ " << objects[other]->GetID() << "�� ������Ʈ���� �浹" << endl;
				
				x = x + 2 * player->GetSightX() * OBJECT_SPEED;
				y = y + 2 * player->GetSightZ() * OBJECT_SPEED;

				break;
			}
		}
		objects[cl]->SetX(x);
		objects[cl]->SetZ(y);
	}
	else
	{
		//cout << "���� ������ϴ�" << endl;
	}
	objects[cl]->SetSightX(dirX);
	objects[cl]->SetSightZ(dirZ);
	//��Ż�� ����

	UpdataPlayerView(cl);

	SendMoveObject(cl);
}

void ProcessEvent(NPC_EVENT& event_object)
{
	EXOver *over_ex = new EXOver;

	if (event_object.type == NPC_MOVE) {
		over_ex->command = NPC_MOVE;
		over_ex->target = event_object.target;
	}

	PostQueuedCompletionStatus(g_iocp, 1, event_object.id, (LPOVERLAPPED)&over_ex->wsaover);
}

void DisconnectPlayer(UINT cl)
{
	cPlayer* player = dynamic_cast<cPlayer*>(objects[cl]);

	//��Ŷ �����ֱ����� ���� viewlist�� �ִ� ������Ʈ���� viewlist���� ���� ���Ž�������Ѵ�.

	if (player->GetIsUse())
	{
		closesocket(player->GetSock());
		player->Clear();
		player->SetIsUse(false);
		cout << "Client [" << cl << "] Disconnected \n";
	}
}

void SendPacket(UINT cl, void * packet)
{
	EXOver *o = new EXOver;
	char *p = reinterpret_cast<char *>(packet);
	memcpy(o->io_buf, p, p[0]);
	o->command = E_SEND;
	o->wsabuf.buf = o->io_buf;
	o->wsabuf.len = p[0];
	ZeroMemory(&o->wsaover, sizeof(WSAOVERLAPPED));

	int ret = WSASend(dynamic_cast<cPlayer*>(objects[cl])->GetSock(), &o->wsabuf, 1, NULL, 0, &o->wsaover, NULL);
	if (0 != ret)
	{
		int err_no = WSAGetLastError();
		if (WSA_IO_PENDING != err_no)
			Errordisplay("Error : ", err_no);
	}
}

void SendSceneObject(int client, SCENE scene)
{
	sc_packet_scene_player p;
	p.id = client;
	p.size = sizeof(sc_packet_scene_player);
	p.type = SC_SCENE;
	p.scene = scene;

	SendPacket(client, &p);
}


void SendMoveObject(int client, int object_id)
{
	float tempx, tempz;
	float sightx, sightz;

	sc_packet_pos p;
	p.id = object_id;
	p.size = sizeof(sc_packet_pos);
	p.type = SC_POS;

	tempx = (roundf(objects[object_id]->GetX() * 10) / 10) * 10;
	tempz = (roundf(objects[object_id]->GetZ() * 10) / 10) * 10;

	sightx = (roundf(objects[object_id]->GetSightX() * 100) / 100) * 100;
	sightz = (roundf(objects[object_id]->GetSightZ() * 100) / 100) * 100;

	p.x = tempx;
	p.y = tempz;
	p.sight_x = sightx;
	p.sight_z = sightz;

	SendPacket(client, &p);
}

void SendMoveObject(int client)
{
	float tempx, tempz;
	float sightx, sightz;

	sc_packet_pos p;
	p.id = client;
	p.size = sizeof(sc_packet_pos);
	p.type = SC_POS;

	tempx = (roundf(objects[client]->GetX() * 10) / 10) * 10;
	tempz = (roundf(objects[client]->GetZ() * 10) / 10) * 10;

	sightx = (roundf(objects[client]->GetSightX() * 100) / 100) * 100;
	sightz = (roundf(objects[client]->GetSightZ() * 100) / 100) * 100;

	p.x = tempx;
	p.y = tempz;
	p.sight_x = sightx;
	p.sight_z = sightz;
	SendPacket(client, &p);
}

void SendPutObject(int client, int object_id) {

	float tempx, tempz;
	float sightx, sightz;

	sc_packet_put_player p;
	p.id = object_id;
	p.size = sizeof(p);
	p.type = SC_PUT_PLAYER;

	tempx = (roundf(objects[object_id]->GetX() * 10) / 10) * 10;
	tempz = (roundf(objects[object_id]->GetZ() * 10) / 10) * 10;

	sightx = (roundf(objects[object_id]->GetSightX() * 100) / 100) * 100;
	sightz = (roundf(objects[object_id]->GetSightZ() * 100) / 100) * 100;

	p.x = tempx;
	p.y = tempz;
	p.sight_x = sightx;
	p.sight_z = sightz;

	SendPacket(client, &p);
}

void SendRemoveObject(int client, int object_id) {
	sc_packet_remove_player p;
	p.id = object_id;
	p.size = sizeof(p);
	p.type = SC_REMOVE_PLAYER;
	SendPacket(client, &p);
}

void Errordisplay(const char * msg, int err_no)
{
	WCHAR *lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	cout << msg;
	wprintf(L"����%s\n", lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void UpdataPlayerView(const UINT id)
{
	cPlayer* player = dynamic_cast<cPlayer*>(objects[id]);
	
	g_scene.UpdateZone(id);

	player->UpdatesSectorSearch();
	player->UpdateViewList();
}

void AddTimer(const UINT id, const UINT temp_id, const int type, const UINT time)
{
	timerQueueMutex.lock();
	NPC_EVENT new_event;

	new_event.id = id;
	new_event.type = type;
	new_event.time = GetCurrTime() + time;
	new_event.target = temp_id;

	event_queue.push(new_event);
	timerQueueMutex.unlock();
}

static UINT GetCurrTime()
{
	static auto start = high_resolution_clock::now();

	return duration_cast<milliseconds>(high_resolution_clock::now() - start).count();
}

bool CollisionCheck(float x1, float y1, float x2, float y2, float size1, float size2)
{
	if (x1 - size1 / 2 > x2 + size2 / 2)
		return false;
	if (x1 + size1 / 2 < x2 - size2 / 2)
	 	return false;
	if (y1 + size1 / 2 < y2 - size2 / 2)
	 	return false;
	if (y1 - size1 / 2 > y2 + size2 / 2)
	 	return false;
	return true;
}