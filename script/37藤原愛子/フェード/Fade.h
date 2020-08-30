#pragma once

#include "GameObject.h"

class CFade : public CGameObject
{
protected:

	ID3D11Buffer*			m_VertexBuffer;
	ID3D11ShaderResourceView*	m_Texture;


	D3DXCOLOR m_FadeColor;	//�F
	bool m_bFadeIn;
	bool m_bFadeOut;

	int m_FadeFrame;			//�t�F�[�h����t���[����
	int m_FrameCount;	//�����ƃJ�E���g���Ă���
	int m_StartFrame;	//�t�F�[�h�X�^�[�g����


public:

	CFade() {};
	~CFade() {};


	void Init();
	void Uninit();
	void Update();
	void Draw();

	void Start(int frame, D3DXCOLOR color, bool bOut);
	bool IsFade();

};