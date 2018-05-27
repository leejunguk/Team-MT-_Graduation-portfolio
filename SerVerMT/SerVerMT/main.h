#pragma once

#include "stdafx.h"


#define MAP_HEIGHT 5100
#define MAP_WIDTH 5100

Client g_client[MAX_USER];
HANDLE g_iocp;

using namespace std;

static const char EVT_RECV = 0;
static const char EVT_SEND = 1;
static const char EVT_EVENT = 3;

void SendPacket(int cl, void *packet)
{
	EXOver *o = new EXOver;
	char *p = reinterpret_cast<char *>(packet);
	o->is_event_type = false;
	memcpy(o->io_buffer, p, p[0]);
	o->wsabuf.buf = o->io_buffer;
	o->wsabuf.len = o->io_buffer[0];
	ZeroMemory(&o->wsaover, sizeof(WSAOVERLAPPED));

	int ret = WSASend(g_client[cl].s, &o->wsabuf, 1, NULL, 0, &o->wsaover, NULL);

	if (0 != ret)
	{
		int err_num = WSAGetLastError();
		if (WSA_IO_PENDING != err_num)
			error_display("Error in SendPacket : ", err_num);
	}
}
void SendPutObject(int client, int object_id)
{
	sc_put_player p;
	p.id = object_id;
	p.size = sizeof(p);
	p.type = SC_PUT_PLAYER;
	p.x = g_client[object_id].x;
	p.y = g_client[object_id].y;

	SendPacket(client, &p);
}
void SendRemoveObject(int client, int object_id)
{
	sc_remove_player p;
	p.id = object_id;
	p.size = sizeof(p);
	p.type = SC_REMOVE_PLAYER;

	SendPacket(client, &p);
}

/*
==========	==========
점속 및 처리.
패킷 처리
==========	==========
*/
void ProcessPacket(int my_key, char* pack) {

	cs_move_player *p = reinterpret_cast<cs_move_player*>(pack);
	float pos[3] = {g_client[my_key].x, g_client[my_key].y, g_client[my_key].z};
	switch (pack[1])
	{
	case UP:
		pos[0] += p->accX*3;
		pos[2] += p->accZ*3;
		break;
	case DOWN:
		pos[0] -= p->accX * 3;
		pos[2] -= p->accZ * 3;
		break;
	case RIGHT:
		pos[0] += p->accZ * 3;
		pos[2] -= p->accX * 3;
		break;
	case LEFT:
		pos[0] -= p->accZ * 3;
		pos[2] += p->accX * 3;
		break;
	default:
		break;
	}
	if (pos[0] < 0)				pos[0] = 0;
	if (pos[2] < 0)				pos[2] = 0;
	if (pos[0] > MAP_WIDTH)		pos[0] = MAP_WIDTH;
	if (pos[2] > MAP_HEIGHT)	pos[2] = MAP_HEIGHT;
	
	g_client[my_key].x = pos[0];
	g_client[my_key].z = pos[2];


	for (int i = 0; i < MAX_USER; ++i) {
		if (i == my_key) continue;
		if (g_client[i].in_use == false) continue;
	}


	sc_pos_player sp;
	sp.id = my_key;
	sp.size = sizeof(sc_pos_player);
	sp.type = SC_POS;
	sp.x = pos[0];
	sp.z = pos[2];
	//자기 자신에게 보냄
	SendPacket(my_key, &sp);


	for (int i = 0; i < MAX_USER; ++i)
	{
		if (my_key == i) continue;
		SendPutObject(i, my_key);
	}
}


void DisCountPlayer(int key)
{
	closesocket(g_client[key].s);
	g_client[key].in_use = false;
	cout << "client [" << key << "] DisCount\n";

	sc_put_player p;
	p.id = key;
	p.size = sizeof(p);
	p.type = SC_REMOVE_PLAYER;

	for (int i = 0 ; i < MAX_USER ; ++i)
		if (g_client[i].in_use == true)
			SendPacket(i, &p);

}


/**/
void WorkerThread()
{
	while (true)
	{
		unsigned long data_size;
		unsigned long long key;
		WSAOVERLAPPED *p_over;

		BOOL is_success = GetQueuedCompletionStatus(g_iocp, &data_size, &key, &p_over, INFINITE);
		//cout << "GQCS from Client [" << key << "] with size [" << data_size << "]\n";


		//에러처리
		if (is_success == 0) {
			cout << "Error in GQCS - key [" << key << "]" << endl;
			DisCountPlayer(key);
			continue;
		}
		//접속종료 처리
		if (data_size == 0)
		{
			DisCountPlayer(key);
			continue;
		}

		//Send , Recv 처리
		EXOver *o = reinterpret_cast<EXOver *>(p_over);
		if (o->is_event_type == EVT_RECV)
		{
			int r_size = data_size;//지금 받은 데이터 크기
			char *ptr = o->io_buffer;//지금 받은 데이터
			while (0 < r_size) {
				int p_size;//현재 조립할 데이터의 전체 크기
				if (0 != g_client[key].packet_size)
				{
					p_size = g_client[key].packet_size;
				}
				else {
					p_size = ptr[0];
					g_client[key].packet_size = p_size;
				}

				//
				int remain = p_size - g_client[key].prev_size;//조립 완성까지 남은 데이터 크기
				if (remain <= r_size) {
					//pakcet build
					memcpy(g_client[key].prev_packet + g_client[key].prev_size,
						ptr, remain);

					ProcessPacket(static_cast<int>(key), g_client[key].prev_packet);
					r_size -= remain;
					ptr += remain;
					g_client[key].packet_size = 0;
					g_client[key].prev_size = 0;
				}
				else
				{
					memcpy(g_client[key].prev_packet + g_client[key].prev_size,
						ptr,
						r_size);
					g_client[key].prev_size += r_size;
					r_size -= r_size;
					ptr += r_size;

				}

			}
			unsigned long rflag = 0;
			ZeroMemory(&g_client[key].exover.wsaover, sizeof(WSAOVERLAPPED));
			WSARecv(g_client[key].s, &g_client[key].exover.wsabuf, 1, NULL,
				&rflag, &g_client[key].exover.wsaover, NULL);
		}
		else if (EVT_SEND == o->is_event_type)
		{
			delete o;
		}

	}
}

void AcceptThread()
{
	auto g_socket = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	SOCKADDR_IN bind_addr;
	ZeroMemory(&bind_addr, sizeof(SOCKADDR_IN));
	bind_addr.sin_family = AF_INET;
	bind_addr.sin_port = htons(SERVER_PORT);
	bind_addr.sin_addr.s_addr = INADDR_ANY;

	::bind(g_socket, reinterpret_cast<sockaddr*>(&bind_addr), sizeof(SOCKADDR_IN));
	listen(g_socket, 1000);

	while (true)
	{
		SOCKADDR_IN client_addr;
		ZeroMemory(&client_addr, sizeof(SOCKADDR_IN));
		client_addr.sin_family = AF_INET;
		client_addr.sin_port = htons(SERVER_PORT);
		client_addr.sin_addr.s_addr = INADDR_ANY;
		int client_length = sizeof(SOCKADDR_IN);

		auto new_socket = WSAAccept(g_socket, reinterpret_cast<sockaddr*>(&client_addr), &client_length, NULL, NULL);
		cout << "Accept \n";

		int new_key = -1;
		for (int i = 0; i < MAX_USER; ++i)
			if (g_client[i].in_use == false) {
				new_key = i;
				break;
			}
		if (new_key == -1) {
			cout << "max user exceeded!!!" << endl;
			continue;
		}
		g_client[new_key].s = new_socket;
		CreateIoCompletionPort(reinterpret_cast<HANDLE>(new_socket), g_iocp, new_key, 0);
		g_client[new_key].in_use = true;

		unsigned long flag = 0;
		int ret = WSARecv(g_client[new_key].s, &g_client[new_key].exover.wsabuf,
			1, NULL, &flag, &g_client[new_key].exover.wsaover, NULL);	//	recv 오류가 안뜨는 문제가 있음
		if (0 != ret)
		{
			int err_num = WSAGetLastError();
			if (WSA_IO_PENDING == err_num)
				error_display("Recv in AcceptThread", err_num);
		}


		sc_put_player p;
		p.id = new_key;
		p.size = sizeof(p);
		p.type = SC_PUT_PLAYER;
		p.x = g_client[new_key].x;
		p.z = g_client[new_key].z;
		//내 접속을 다른 사람들 모두에게 달림.
		for (int i = 0; i < MAX_USER; ++i)
		{
			if (g_client[i].in_use == true) {
				SendPacket(i, &p);
			}
			if (i == new_key) continue;
		}
	}


}

