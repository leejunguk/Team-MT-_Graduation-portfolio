// WindowsProject1.cpp: ���� ���α׷��� �������� �����մϴ�.
//


#include <WinSock2.h>
#include <windowsx.h>
#include <stdio.h>



#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <iostream>

#include "../../../protocol.h"
#pragma comment (lib, "ws2_32.lib")


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.


static void error_display(const char *msg, int err_no)
{
	WCHAR *lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	std::cout << msg << std::endl;
	std::cout << L"����%s\n" << lpMsgBuf << std::endl << std::endl;
	LocalFree(lpMsgBuf);
}

struct Character {
	BYTE state;
	float x, y, z;
	BYTE accX, accY, accZ;
	ColBox boudingbox;

};
//
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define MAX_LOADSTRING 100
#define	WM_SOCKET				WM_USER + 1

// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

												// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);



HANDLE g_iocp;
Character g_client[MAX_USER];
SOCKET g_socket;

HWND main_window_handle = NULL; // save the window handle
HINSTANCE main_instance = NULL; // save the instance
HWND hWnd = NULL;
char buffer[80];                // used to print text

								//
WSABUF	send_wsabuf;
char 	send_buffer[MAX_BUFSIZE];

WSABUF	recv_wsabuf;
char	recv_buffer[MAX_BUFSIZE];

char	packet_buffer[MAX_BUFSIZE];
DWORD		in_packet_size = 0;
int		saved_packet_size = 0;
int		g_myid;

using namespace std;


void ProcessPacket(char* pack) {

	static bool first_time = true;
	switch (pack[1])
	{
	case SC_PUT_PLAYER:
	{
		sc_put_player *my_packet = reinterpret_cast<sc_put_player *>(pack);
		int id = my_packet->id;
		if (first_time) {
			first_time = false;
			g_myid = id;
		}
		g_client[id].x = my_packet->x;
		g_client[id].y = my_packet->y;
		g_client[id].z = my_packet->z;

		break;
	}
	case SC_POS:
	{
		sc_pos_player *my_packet = reinterpret_cast<sc_pos_player *>(pack);
		int other_id = my_packet->id;

		g_client[other_id].x = my_packet->x;
		g_client[other_id].y = my_packet->y;
		g_client[other_id].z = my_packet->z;
		break;
	}
	case SC_REMOVE_PLAYER:
	{
		sc_remove_player *my_packet = reinterpret_cast<sc_remove_player *>(pack);
		int other_id = my_packet->id;
		g_client[other_id].state = false;
		break;
	}
	}
}

void ReadPacket(SOCKET sock)
{
	DWORD iobyte, ioflag = 0;

	int ret = WSARecv(sock, &recv_wsabuf, 1, &iobyte, &ioflag, NULL, NULL);
	if (ret) {
		int err_code = WSAGetLastError();
		printf("Recv Error [%d]\n", err_code);
	}

	BYTE *ptr = reinterpret_cast<BYTE *>(recv_buffer);

	while (0 != iobyte) {
		if (0 == in_packet_size) in_packet_size = ptr[0];
		if (iobyte + saved_packet_size >= in_packet_size) {
			memcpy(packet_buffer + saved_packet_size, ptr, in_packet_size - saved_packet_size);
			ProcessPacket(packet_buffer);
			ptr += in_packet_size - saved_packet_size;
			iobyte -= in_packet_size - saved_packet_size;
			in_packet_size = 0;
			saved_packet_size = 0;
		}
		else {
			memcpy(packet_buffer + saved_packet_size, ptr, iobyte);
			saved_packet_size += iobyte;
			iobyte = 0;
		}
	}
}

//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

	hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(NULL));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(NULL);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(NULL));

	return RegisterClassExW(&wcex);
}




int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: ���⿡ �ڵ带 �Է��մϴ�.

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadStringW(hInstance, NULL, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, NULL, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(NULL));
	MSG msg;
	//
	main_window_handle = hWnd;
	main_instance = hInstance;


	WSADATA	wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	g_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

	SOCKADDR_IN ServerAddr;
	ZeroMemory(&ServerAddr, sizeof(SOCKADDR_IN));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(SERVER_PORT);
	ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	int Result = WSAConnect(g_socket, (sockaddr *)&ServerAddr, sizeof(ServerAddr), NULL, NULL, NULL, NULL);

	WSAAsyncSelect(g_socket, main_window_handle, WM_SOCKET, FD_CLOSE | FD_READ);

	send_wsabuf.buf = send_buffer;
	send_wsabuf.len = MAX_BUFSIZE;
	recv_wsabuf.buf = recv_buffer;
	recv_wsabuf.len = MAX_BUFSIZE;


	// �⺻ �޽��� �����Դϴ�.
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// test if this is a quit
			if (msg.message == WM_QUIT)
				break;

			// translate any accelerator keys
			TranslateMessage(&msg);

			// send the message to the window proc
			DispatchMessage(&msg);
		} // end if


	}

	return (int)msg.wParam;
}



//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static BYTE dir = 0;
	static cs_move_player *my_packet = reinterpret_cast<cs_move_player *>(send_buffer);
	static int ret;
	switch (message)
	{
	case WM_KEYDOWN:
		if (wParam == VK_RIGHT)	dir &= RIGHT;
		if (wParam == VK_LEFT)	dir &= LEFT;
		if (wParam == VK_UP)	dir &= UP;
		if (wParam == VK_DOWN)	dir &= DOWN;

		//���� �����͸� ����.
		my_packet->size = sizeof(my_packet);
		send_wsabuf.len = sizeof(my_packet);

		DWORD iobyte;
		ret = WSASend(g_socket, &send_wsabuf, 1, &iobyte, 0, NULL, NULL);
		if (ret) {
			int error_code = WSAGetLastError();
			printf("Error while sending packet [%d]", error_code);
		}
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�.
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SOCKET:
	{
		if (WSAGETSELECTERROR(lParam)) {
			closesocket((SOCKET)wParam);
			exit(-1);
			break;
		}
		switch (WSAGETSELECTEVENT(lParam)) {
		case FD_READ:
			ReadPacket((SOCKET)wParam);
			break;
		case FD_CLOSE:
			closesocket((SOCKET)wParam);
			exit(-1);
			break;
		}
	}
	default:
		break;
	}
	return (DefWindowProc(hWnd, message, wParam, lParam));
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
