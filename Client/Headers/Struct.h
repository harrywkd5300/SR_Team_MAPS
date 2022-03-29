#pragma once
#include "../Default/stdafx.h"

typedef struct Tag_WeaponInfo
{
	float			m_fFireRate;		//�߻�ð�
	float			m_fBulletSpeed;		//�߻�ü�ӵ�
	float			m_fReloadRate;		//�����ð�

	float			m_fFireRateTimer;		// �߻�ð�üũ
	float			m_fReloadRateTimer;		// �����ð�üũ

	float			m_fXRecoilPower;	//����ݵ�
	float			m_fYRecoilPower;	//�����ݵ�

	float			m_fBulletRange;		//�Ѿ� ��Ÿ�

	unsigned int	m_iCurBullet;		//�����Ѿ�
	unsigned int	m_iMaxBullet;		//�ִ��Ѿ�
	unsigned int	m_iDamage;			//������

	bool			m_bBulletEffect;	// �Ѿ� ����Ʈ ���� (true - ��ƼŬ / false - RECT)  
	bool			m_bETCEffect;		//��Ÿ ����Ʈ ���� (true - ��ƼŬ / false - RECT)
	bool			m_bReloading;		//������ ������ Ȯ��

	bool			bIsEquipped;		//���뿩��


}WEAPONINFO;

//
//template<typename T>
//bool operator()(T& T)
//{
//}