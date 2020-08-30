#pragma once

#include "Scene.h"

class CTitle : public CScene
{
private:
	bool m_bFadeIn;
	bool m_bFadeOut;
public:

	void Init();
	void Update();


};