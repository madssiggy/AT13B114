//=====================================
//    
//   light.cpp
//              created by Koki Nakano
//
//=====================================


//ライトを使いやすくしたクラスです
//設定・変更したい場所で変更したいパラメータの関数を呼ぶだけです



#include "light.h"


LIGHT CLight::m_Light;


void CLight::Init()
{
	m_Light.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f,0.0f);
	m_Light.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_Light.Direction = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
}