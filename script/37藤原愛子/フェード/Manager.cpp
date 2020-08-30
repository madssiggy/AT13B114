
#include "main.h"
#include "manager.h"
#include "renderer.h"

#include "input.h"
#include "audio_clip.h"

#include "Game.h"
#include "Title.h"

#include "Fade.h"


CScene* CManager::m_Scene = NULL;

CFade* CManager::m_Fade = NULL;

void CManager::Init()
{

	CRenderer::Init();

	CInput::Init();

	CAudioClip::Init();
	CAudioClip::SetMasterVolume(0.05f);

	m_Fade = new CFade();
	m_Fade->Init();

	SetScene<CGame>();
}

void CManager::Uninit()
{

	m_Scene->Uninit();
	delete m_Scene;

	m_Fade->Uninit();
	delete m_Fade;

	CAudioClip::Uninit();

	CInput::Uninit();

	CRenderer::Uninit();

}

void CManager::Update()
{
	CInput::Update();

	m_Fade->Update();

	m_Scene->Update();
}

void CManager::Draw()
{

	CRenderer::Begin();

	m_Scene->Draw();

	m_Fade->Draw();

	

	CRenderer::End();

}


void CManager::StartFadeIn(int Frame)
{
	m_Fade->Start(Frame, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), false);
}
void CManager::StartFadeOut(int Frame)
{
	m_Fade->Start(Frame, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), true);
}
bool CManager::IsFade()
{
	return m_Fade->IsFade();
}