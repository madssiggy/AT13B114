#pragma once
#include "component.h"
#include"main.h"
#include<vector>
#include"manager.h"
#include"scene.h"
float LenSegOnSeparateAxis(D3DXVECTOR3 *Sep, D3DXVECTOR3 *e1, D3DXVECTOR3 *e2, D3DXVECTOR3 *e3 = 0);
class CSphereCollider;
class CBoxCollider; 
class CCollider :public CComponent
{
protected:
	std::vector<CCollider*> m_hitCollider;

public:
	CCollider() {}
	~CCollider() {}
	std::vector<CCollider*>* GetHitVector() { return &m_hitCollider; }
	void CreanHitVector() { m_hitCollider.clear(); }
	void AddHitCollider(CCollider* add) { m_hitCollider.push_back(add); }
	
	static void Judge();
	static bool Sphere(CSphereCollider* a, CSphereCollider* b);
	
	static bool OBB(CBoxCollider* a, CBoxCollider* b);
};
float LenSegOnSeparateAxis(D3DXVECTOR3 *Sep, D3DXVECTOR3 *e1, D3DXVECTOR3 *e2, D3DXVECTOR3 *e3);





