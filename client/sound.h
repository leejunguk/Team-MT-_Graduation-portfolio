#pragma once
#include <conio.h>
#include <iostream>
using namespace std;

#include "fmod.hpp"
#pragma	comment (lib, "fmodex64_vc.lib")
using namespace FMOD;



class SoundMgr {
	System*			pSystem;
	//	������ ������ŭ �ʿ�
	Sound*			pSound[5];
	//	���ÿ� ��µ� ���� ��ŭ �ʿ�
	Channel*		pChannel[2];
public:

	void sound_bgm()
	{
		//���� �ý����� ����
		System_Create(&pSystem);
		//	���� �ý��� �⺻ ����
		pSystem->init(2, FMOD_INIT_NORMAL, NULL);

		//	�������� ����
		pSystem->createSound(
			"onyak__background_LostTheWar.mp3",	//	�������ϰ��
			FMOD_HARDWARE | FMOD_LOOP_NORMAL,	//	�������Ͽɼ�
			NULL,			//	Ȯ�嵥����(��������)
			&pSound[0]);	//	������ ���� ������

		pSystem->createSound(
			"Ring.wav",
			FMOD_LOOP_OFF | FMOD_HARDWARE,
			NULL,
			&pSound[1]);

		pSystem->createSound(
			"ball.wav",
			FMOD_LOOP_OFF | FMOD_HARDWARE,
			NULL,
			&pSound[2]);

		pSystem->createSound(
			"die.wav",
			FMOD_LOOP_OFF | FMOD_HARDWARE,
			NULL,
			&pSound[3]);

		bool	bPlay = true;
		bool	IsBGPlaying = true;
		bool	IsEffectPlaying = false;
		bool	IsEffectPlaying2 = false;
		float	fVolume = 1;


		pChannel[0]->isPlaying(&IsBGPlaying);
		pChannel[1]->isPlaying(&IsEffectPlaying);

		pSystem->playSound(FMOD_CHANNEL_REUSE, pSound[0], false, &pChannel[0]);
		pChannel[0]->setVolume(fVolume);

		pSystem->playSound(FMOD_CHANNEL_REUSE, pSound[1], false, &pChannel[1]);

		//�������� ����
		//pSound[0]->release();
		//pSound[1]->release();

		//	�ý��� ����
		//pSystem->release();
		////	�ý��� �ݱ�
		//pSystem->close();


		//////////////////////////////////////////////////////////////
	}



	void sound_title()
	{
		//���� �ý����� ����
		System_Create(&pSystem);
		//	���� �ý��� �⺻ ����
		pSystem->init(2, FMOD_INIT_NORMAL, NULL);

		//	�������� ����
		pSystem->createSound(
			"title.wav",	//	�������ϰ��
			FMOD_HARDWARE | FMOD_LOOP_NORMAL,	//	�������Ͽɼ�
			NULL,			//	Ȯ�嵥����(��������)
			&pSound[0]);	//	������ ���� ������

		pSystem->createSound(
			"Sega.wav",
			FMOD_LOOP_OFF | FMOD_HARDWARE,
			NULL,
			&pSound[1]);

		bool	bPlay = true;
		bool	IsBGPlaying = true;
		bool	IsEffectPlaying = false;
		float	fVolume = 1;


		pChannel[0]->isPlaying(&IsBGPlaying);
		pChannel[1]->isPlaying(&IsEffectPlaying);


		pSystem->playSound(FMOD_CHANNEL_REUSE, pSound[0], false, &pChannel[0]);
		pChannel[1]->setVolume(fVolume);

		pSystem->playSound(FMOD_CHANNEL_REUSE, pSound[1], false, &pChannel[1]);


		//	�������� ����
		//pSound[0]->release();
		//pSound[1]->release();
		//
		////	�ý��� ����
		//pSystem->release();
		////	�ý��� �ݱ�
		//pSystem->close();


		//////////////////////////////////////////////////////////////
	}


	void sound_complete()
	{
		//���� �ý����� ����
		System_Create(&pSystem);
		//	���� �ý��� �⺻ ����
		pSystem->init(2, FMOD_INIT_NORMAL, NULL);

		//	�������� ����
		pSystem->createSound(
			"title.wav",	//	�������ϰ��
			FMOD_HARDWARE | FMOD_LOOP_NORMAL,	//	�������Ͽɼ�
			NULL,			//	Ȯ�嵥����(��������)
			&pSound[0]);	//	������ ���� ������

		pSystem->createSound(
			"Complete.wav",
			FMOD_LOOP_OFF | FMOD_HARDWARE,
			NULL,
			&pSound[1]);

		bool	bPlay = true;
		bool	IsBGPlaying = true;
		bool	IsEffectPlaying = false;
		float	fVolume = 1;


		pChannel[0]->isPlaying(&IsBGPlaying);
		pChannel[1]->isPlaying(&IsEffectPlaying);


		pSystem->playSound(FMOD_CHANNEL_REUSE, pSound[0], false, &pChannel[0]);
		pChannel[1]->setVolume(fVolume);

		pSystem->playSound(FMOD_CHANNEL_REUSE, pSound[1], false, &pChannel[1]);


		//	�������� ����
		//pSound[0]->release();
		//pSound[1]->release();
		//
		////	�ý��� ����
		//pSystem->release();
		////	�ý��� �ݱ�
		//pSystem->close();


		//////////////////////////////////////////////////////////////
	}
};