#pragma once
#include "cObject.h"

class cPlayer : public cObject
{
private:
	SOCKET sock;

	BYTE packet_size;
	BYTE saved_size;
	BYTE packet_buf[MAX_PACKET_SIZE];
	BYTE recv_buf[MAX_BUFF_SIZE];

	unordered_set<UINT> viewlist;
	mutex viewMutex;
public:
	const SOCKET& GetSock() const { return sock; }
	unordered_set<UINT>& GetViewlist() { return viewlist; }
public:
	void SetSock(const SOCKET& s) { sock = s; }
public:
	void RecvPacket();
	void ReadPacket(const UINT id, const int transferred);
	void UpdateViewList();
	
	bool AttackMonster(int *attack_target);
	void NonTartgetStep(int i);
public:
	void Clear();
public:
	cPlayer();
	virtual ~cPlayer();
};

