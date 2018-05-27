
#pragma once
//
#include "main.h"

void Initialize() {

	g_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	srand((unsigned)time(NULL));


	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;

	for (int i = 0; i < MAX_USER; ++i)
	{
		g_client[i].x = rand() % (MAP_WIDTH-100)+50;
		g_client[i].z = rand() % (MAP_HEIGHT-100)+50;


		g_client[i].in_use = false;
		ZeroMemory(&g_client[i].exover.wsaover, sizeof(WSAOVERLAPPED));
		g_client[i].exover.wsabuf.buf = g_client[i].exover.io_buffer;
		g_client[i].exover.wsabuf.len = sizeof(g_client[i].exover.io_buffer);
		g_client[i].exover.is_event_type = EVT_RECV;
		g_client[i].prev_size = 0;
		g_client[i].packet_size = 0;
	}
}

int main(void) {

	vector <thread> worker;
	Initialize();

	for (int i = 0; i < 4; ++i) {
		worker.push_back(thread{WorkerThread} );
	}
	thread accept{ AcceptThread };


	for (auto &th : worker) {
		th.join();
	}
	accept.join();

	WSACleanup();
}