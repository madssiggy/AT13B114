#pragma once

#include "GameObject.h"

class CFade : public CGameObject
{
protected:

	ID3D11Buffer*			m_VertexBuffer;
	ID3D11ShaderResourceView*	m_Texture;


	D3DXCOLOR m_FadeColor;	//色
	bool m_bFadeIn;
	bool m_bFadeOut;

	int m_FadeFrame;			//フェードするフレーム数
	int m_FrameCount;	//ずっとカウントしている
	int m_StartFrame;	//フェードスタート時間


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