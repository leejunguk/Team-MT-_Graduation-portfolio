#pragma once

//���������� �����ϴ� ����
#include "cObject.h"
#include "cZone.h"

extern map<UINT, cObject*> objects;

extern volatile bool   thread_running;
extern volatile HANDLE g_iocp;
extern cScene g_scene;

extern mutex timerMutex;
extern mutex timerQueueMutex;
extern priority_queue<NPC_EVENT, vector<NPC_EVENT>, Comparison> event_queue;