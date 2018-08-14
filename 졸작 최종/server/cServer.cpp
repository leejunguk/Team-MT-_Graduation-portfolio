#include "stdafx.h"
#include "cServer.h"

#include "cPlayer.h"
#include "cMonster.h"

#include "cThreadFunc.h"
#include "cMyNamespace.h"


cServer::cServer()
{
	Initialize();
}

cServer::~cServer()
{
	Release();
}

void cServer::Initialize()
{
	srand(unsigned(time(NULL)));

	wcout.imbue(locale("korean"));
	wcout << L"���� ����!\n ";
	thread_running = true;
	SYSTEM_INFO systeminfo;
	GetSystemInfo(&systeminfo);
	cpuNumber = (systeminfo.dwNumberOfProcessors /2 )* 1.5;

	cout << "CPU ���� : " << cpuNumber << endl;
	//CPU ������ �ھ���� 1.5��

	g_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);


	for (int i = 0; i < MAX_OBJECT_INDEX ; ++i)
	{
		if (i < MAX_USER)
			objects[i] = new cPlayer();
		else
		{
			objects[i] = new cMonster();
			objects[i]->SetID(i);
			objects[i]->SetX(2550);
			objects[i]->SetZ(2550);
			objects[i]->SetIsUse(true);
			objects[i]->SetScene(STAGE_ONE);
			g_scene.UpdateZone(i);
		}
	}

	a_thread = thread{ AcceptThread };

	for (unsigned int i = 0; i < cpuNumber; ++i)
		w_threads.push_back(new thread{ WorkerThread });

	t_thread = thread{ TimerThread };
}

void cServer::Progress()
{
	cout << " ���� ���� [y/Y] " << endl;

	int key;

	while (true)
	{
		if (_kbhit() != false)
		{
			key = _getch();

			if (key == 'y' || key == 'Y')
			{
				thread_running = false;
				break;
			}
		}
	}
	SYSTEM_INFO systeminfo;
	GetSystemInfo(&systeminfo);
	DWORD cpu_count = systeminfo.dwNumberOfProcessors;

	// worker_thread ����
	for (int i = 0; i < cpuNumber; ++i)
		PostQueuedCompletionStatus(g_iocp, 0, 0, nullptr);

	// accept_thread ����
	while (true)
	{
		SOCKET tmp_sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

		SOCKADDR_IN server_addr;
		ZeroMemory(&server_addr, sizeof(SOCKADDR_IN));
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(MY_SERVER_PORT);
		server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

		int retval = WSAConnect(tmp_sock, reinterpret_cast<SOCKADDR*>(&server_addr), sizeof(server_addr), NULL, NULL, NULL, NULL);
		if (SOCKET_ERROR != retval)
			break;
	}
}

void cServer::Release()
{
	a_thread.join();

	for (auto worker : w_threads)
	{
		worker->join();
		delete worker;
	}
	w_threads.clear();

	t_thread.join();

	for (auto object : objects)
	{
		delete object.second;
		object.second = nullptr;
	}
	objects.clear();
}