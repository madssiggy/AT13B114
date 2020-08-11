#pragma once
#include "collider.h"
class CBoxCollider :
	public CCollider
{
public:
	CBoxCollider();
	~CBoxCollider();
	void Init();
	void Uninit();
	void Update();
	void Draw();
};

