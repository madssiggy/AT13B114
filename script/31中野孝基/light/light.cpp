//=====================================
//    
//   light.cpp
//              created by Koki Nakano
//
//=====================================


//���C�g���g���₷�������N���X�ł�
//�ݒ�E�ύX�������ꏊ�ŕύX�������p�����[�^�̊֐����ĂԂ����ł�



#include "light.h"


LIGHT CLight::m_Light;


void CLight::Init()
{
	m_Light.Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f,0.0f);
	m_Light.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_Light.Direction = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
}