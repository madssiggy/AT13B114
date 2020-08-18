//=====================================
//    
//   light.h
//              created by Koki Nakano
//
//=====================================


#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "main.h"
#include "renderer.h"


class CLight 
{
private:
	static LIGHT m_Light;


public:
	CLight() {}
	~CLight() {}

	static void Init();
	static void SetDiffuse(D3DXCOLOR diffuse) { m_Light.Diffuse = diffuse; }
	static void SetAmbient(D3DXCOLOR ambient) { m_Light.Ambient = ambient; }
	static void SetDirection(D3DXVECTOR4 direction)
	{
		m_Light.Direction = direction;
		D3DXVec4Normalize(&m_Light.Direction, &m_Light.Direction);
	}
	static void Switch(bool on)
	{
		m_Light.Enable = on;

		CRenderer::SetLight(m_Light);
	}
};


#endif