//=====================================
//    
//   debugcollision.cpp
//              created by Koki Nakano
//
//=====================================


// <����>
// ����̓f�o�b�O�p�A�����蔻��`��̃\�[�X�R�[�h�ł��B
// �����N���X�Ŕz�z���ꂽrenderer.h���K�v�ɂȂ�܂��B
// static�ϐ��Ȃ̂ŃC���X�^���X�𐶐������Ɋ֐����������Ƃ��ł��܂��B
// ����ƁA�t����white.png���A�Z�b�g�t�H���_�ɓ���Ƃ��Ă��������B


#include "main.h"
#include "renderer.h"

#include "debugcollision.h"
#include "debugcollision2D.h"
#include "debugcollision3D.h"



#define DEBUG_TEXTURE_PATH "asset/texture/white.png"




ID3D11ShaderResourceView* CDebugCollision::m_Texture;        //�e�N�X�`��



void CDebugCollision::Init()
{
	//�e�N�X�`���ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
		DEBUG_TEXTURE_PATH,
		NULL,
		NULL,
		&m_Texture,
		NULL);

	//�ǂݍ��݊m�F
	assert(m_Texture);


	//�����蔻��̏�����
	DebugSphereCollision::Init(m_Texture);
	DebugCubeCollision::Init(m_Texture);
	DebugCircleCollision::Init(m_Texture);
	DebugSquareCollision::Init(m_Texture);
}




void CDebugCollision::Uninit()
{
	//�����蔻��̏I����
	DebugSphereCollision::Uninit();
	DebugCubeCollision::Uninit();
	DebugCircleCollision::Uninit();
	DebugSquareCollision::Uninit();
}