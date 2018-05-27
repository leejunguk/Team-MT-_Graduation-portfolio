#pragma once

#include <winsock2.h>
#include <winsock.h>
#include <windowsx.h>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <utility>

#include <vector>
#include <thread>
#include "../../protocol.h"
#pragma comment(lib, "ws2_32")

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
	std::cout << L"¿¡·¯%s\n" << lpMsgBuf << std::endl << std::endl;
	LocalFree(lpMsgBuf);
}

