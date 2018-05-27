#pragma once

#define MAX_BUFSIZE 4000
#define MAX_PACKET_SIZE 255

#define MAX_USER 8

#define SERVER_PORT 9000


#define SERVER

#define IDLE	0x00
#define UP		0x01
#define DOWN	0x02
#define RIGHT	0x04
#define LEFT	0x08


#define SC_POS				10
#define SC_PUT_PLAYER		11
#define SC_REMOVE_PLAYER	12
#define SC_CHAT				13
/*

*/
#pragma pack(push, 1)
//클라 -> 서버
struct cs_move_player {
	BYTE size;
	BYTE type;
	BYTE accX, accY, accZ;
};

//서버 -> 클라
struct sc_put_player {
	BYTE size;
	BYTE type;
	WORD id;
	float x, y, z;
};
struct sc_pos_player {
	BYTE size;
	BYTE type;
	WORD id;
	float x, y, z;
};
struct sc_remove_player {
	BYTE size;
	BYTE type;
	WORD id;
};


struct ColBox {
	BYTE radian;
};
struct EXOver {
	WSAOVERLAPPED wsaover;
	char is_event_type;
	WSABUF wsabuf;
	char io_buffer[MAX_BUFSIZE];
};
struct Client
{
	SOCKET s;
	float x, y, z;
	BYTE accX, accY, accZ;
	ColBox boudingbox;

	bool in_use; // 해당 클라이언트
	int packet_size;
	int prev_size;
	char prev_packet[MAX_PACKET_SIZE];
	EXOver exover;
};
#pragma pack(pop)