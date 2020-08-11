#pragma once
#include "collider.h"
class CSphereCollider :public CCollider
{
private:
	float m_lenth = 0;
public:
	CSphereCollider();
	~CSphereCollider();
	void Init();
	void Uninit();
	void Update();
	void Draw();
	float GetLenth() { return m_lenth; }
	void SetLenth(float set) { m_lenth = set; }

};
