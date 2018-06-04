#pragma once

struct EXOver {
	WSAOVERLAPPED wsaover;
	int command;
	WSABUF wsabuf;
	char io_buf[MAX_BUFF_SIZE];
};