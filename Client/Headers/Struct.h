#pragma once
#include "../Default/stdafx.h"

typedef struct Tag_WeaponInfo
{
	float			m_fFireRate;		//발사시간
	float			m_fBulletSpeed;		//발사체속도
	float			m_fReloadRate;		//장전시간

	float			m_fFireRateTimer;		// 발사시간체크
	float			m_fReloadRateTimer;		// 장전시간체크

	float			m_fXRecoilPower;	//수평반동
	float			m_fYRecoilPower;	//수직반동

	float			m_fBulletRange;		//총알 사거리

	unsigned int	m_iCurBullet;		//현재총알
	unsigned int	m_iMaxBullet;		//최대총알
	unsigned int	m_iDamage;			//데미지

	bool			m_bBulletEffect;	// 총알 이펙트 종류 (true - 파티클 / false - RECT)  
	bool			m_bETCEffect;		//기타 이펙트 여부 (true - 파티클 / false - RECT)
	bool			m_bReloading;		//재장전 중인지 확인

	bool			bIsEquipped;		//착용여부


}WEAPONINFO;

//
//template<typename T>
//bool operator()(T& T)
//{
//}