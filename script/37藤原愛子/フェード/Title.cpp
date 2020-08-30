
#include "Title.h"


#include "input.h"
#include "Game.h"
#include "manager.h"

#include "TitleLogo.h"

void CTitle::Init()
{
	AddGameObject<CTitleLogo>(LAYER_2D_1);


	m_bFadeIn = true;
	m_bFadeOut = false;
	CManager::StartFadeIn(30);
}

void CTitle::Update()
{
	CScene::Update();

	if (m_bFadeIn)
	{
		if (!CManager::IsFade())
		{
			m_bFadeIn = false;
		}
	}
	else if (m_bFadeOut)
	{
		if (!CManager::IsFade())
		{
			m_bFadeOut = false;
			CManager::SetScene<CGame>();
		}
	}
	else
	{
		if (CInput::GetKeyTrigger(VK_RETURN))
		{
			m_bFadeOut = true;
			CManager::StartFadeOut(60);
		}
	}
}