#include "collider.h"
#include"box_collider.h"
#include"sphere_collider.h"
//
#include"input.h"
#include"player.h"
void CCollider::Judge()
{
	std::vector<CBoxCollider*> box;
	std::vector<CSphereCollider*> sphere;

	CManager::GetScene()->FindConponents(&box);
	CManager::GetScene()->FindConponents(&sphere);

	for (CCollider* b : box)
	{
		b->GetHitVector()->clear();
	}
	for (CCollider* s : sphere)
	{
		s->GetHitVector()->clear();
	}
	int size = box.size();

	for (int i = 0; i < (size - 1); i++)
	{
		for (int j = i + 1; j < size; j++)
		{
			if (i!=j)
			{

				if (OBB(box[j], box[i]) == true)
				{
					box[i]->AddHitCollider(box[j]);
					box[j]->AddHitCollider(box[i]);
				}
			}

	

		}
	}
	size = sphere.size();

	for (int i = 0; i < (size - 1); i++)
	{
		for (int j = i + 1; j < size; j++)
		{
			if (i != j)
			{
				if (Sphere(sphere[j], sphere[i]) == true)
				{
					sphere[i]->AddHitCollider(sphere[j]);
					sphere[j]->AddHitCollider(sphere[i]);
				}
			}
			
		}
	}
};
bool CCollider::Sphere(CSphereCollider* a, CSphereCollider* b)
{
	D3DXVECTOR3 interval = a->GetGameObject()->GetPosition() - b->GetGameObject()->GetPosition();
	float l = sqrtf((interval.x*interval.x) + (interval.y*interval.y) + (interval.z*interval.z));

	if (l < a->GetLenth() + b->GetLenth())
	{

		return true;
	}
	return false;
}
bool CCollider::OBB(CBoxCollider* a, CBoxCollider* b)
{
	if (CInput::GetKeyTrigger('Z'))
	{
		int a = 1;
	}
	D3DXVECTOR3 interval = a->GetGameObject()->GetPosition() - b->GetGameObject()->GetPosition();
	D3DXVECTOR3 an1 = a->GetGameObject()->GetForward();
	D3DXVECTOR3 an2 = a->GetGameObject()->GetRight();
	D3DXVECTOR3 an3 = a->GetGameObject()->GetUp();
	D3DXVECTOR3 bn1 = b->GetGameObject()->GetForward();
	D3DXVECTOR3 bn2 = b->GetGameObject()->GetRight();
	D3DXVECTOR3 bn3 = b->GetGameObject()->GetUp();

	D3DXVECTOR3 a1 = an1 * a->GetGameObject()->GetScale().z;
	D3DXVECTOR3 a2 = an2 * a->GetGameObject()->GetScale().x;
	D3DXVECTOR3 a3 = an3 * a->GetGameObject()->GetScale().y;
	D3DXVECTOR3 b1 = bn1 * b->GetGameObject()->GetScale().z;
	D3DXVECTOR3 b2 = bn2 * b->GetGameObject()->GetScale().x;
	D3DXVECTOR3 b3 = bn3 * b->GetGameObject()->GetScale().y;
	float rA = D3DXVec3Length(&a1);
	float rB = LenSegOnSeparateAxis(&an1, &b1, &b2, &b3);
	float l = fabs(D3DXVec3Dot(&interval, &an1));


	// 分離軸 : A : Flont
	rA = D3DXVec3Length(&a1);
	rB = LenSegOnSeparateAxis(&an1, &b1, &b2, &b3);
	l = fabs(D3DXVec3Dot(&interval, &an1));
	if (l > rA + rB)
		return false; // 衝突していない

	 // 分離軸 : A : Right
	rA = D3DXVec3Length(&a2);
	rB = LenSegOnSeparateAxis(&an2, &b1, &b2, &b3);
	l = fabs(D3DXVec3Dot(&interval, &an2));
	if (l > rA + rB)
		return false;
	
	// 分離軸 : A : Up
	rA = D3DXVec3Length(&a3);
	rB = LenSegOnSeparateAxis(&an3, &b1, &b2, &b3);
	l = fabs(D3DXVec3Dot(&interval, &an3));
	if (l > rA + rB)
		return false;

	// 分離軸 : B : Flont
	rA = LenSegOnSeparateAxis(&bn1, &a1, &a2, &a3);
	rB = D3DXVec3Length(&b1);
	l = fabs(D3DXVec3Dot(&interval, &bn1));
	if (l > rA + rB)
		return false;

	// 分離軸 : B : Right
	rA = LenSegOnSeparateAxis(&bn2, &a1, &a2, &a3);
	rB = D3DXVec3Length(&b2);
	l = fabs(D3DXVec3Dot(&interval, &bn2));
	if (l > rA + rB)
		return false;

	// 分離軸 : B : Up
	rA = LenSegOnSeparateAxis(&bn3, &a1, &a2, &a3);
	rB = D3DXVec3Length(&b3);
	l = fabs(D3DXVec3Dot(&interval, &bn3));
	if (l > rA + rB)
		return false;
	
	/*
	// 分離軸 : C11
	D3DXVECTOR3 Cross;
	D3DXVec3Cross(&Cross, &an1, &bn1);
	rA = LenSegOnSeparateAxis(&Cross, &a2, &a3);
	rB = LenSegOnSeparateAxis(&Cross, &b2, &b3);
	l = fabs(D3DXVec3Dot(&interval, &Cross));
	if (l > rA + rB)
		return false;

	// 分離軸 : C12
	D3DXVec3Cross(&Cross, &an1, &bn2);
	rA = LenSegOnSeparateAxis(&Cross, &a2, &a3);
	rB = LenSegOnSeparateAxis(&Cross, &b1, &b3);
	l = fabs(D3DXVec3Dot(&interval, &Cross));
	if (l > rA + rB)
		return false;

	// 分離軸 : C13
	D3DXVec3Cross(&Cross, &an1, &bn3);
	rA = LenSegOnSeparateAxis(&Cross, &a2, &a3);
	rB = LenSegOnSeparateAxis(&Cross, &b1, &b2);
	l = fabs(D3DXVec3Dot(&interval, &Cross));
	if (l > rA + rB)
		return false;
	
	// 分離軸 : C21
	D3DXVec3Cross(&Cross, &an2, &bn1);
	rA = LenSegOnSeparateAxis(&Cross, &a1, &a3);
	rB = LenSegOnSeparateAxis(&Cross, &b2, &b3);
	l = fabs(D3DXVec3Dot(&interval, &Cross));
	if (l > rA + rB)
		return false;

	// 分離軸 : C22
	D3DXVec3Cross(&Cross, &an2, &bn2);
	rA = LenSegOnSeparateAxis(&Cross, &a1, &a3);
	rB = LenSegOnSeparateAxis(&Cross, &b1, &b3);
	l = fabs(D3DXVec3Dot(&interval, &Cross));
	if (l > rA + rB)
		return false;

	// 分離軸 : C23
	D3DXVec3Cross(&Cross, &an2, &bn3);
	rA = LenSegOnSeparateAxis(&Cross, &a1, &a3);
	rB = LenSegOnSeparateAxis(&Cross, &b1, &b2);
	l = fabs(D3DXVec3Dot(&interval, &Cross));
	if (l > rA + rB)
		return false;

	// 分離軸 : C31
	D3DXVec3Cross(&Cross, &an3, &bn1);
	rA = LenSegOnSeparateAxis(&Cross, &a1, &a2);
	rB = LenSegOnSeparateAxis(&Cross, &b2, &b3);
	l = fabs(D3DXVec3Dot(&interval, &Cross));
	if (l > rA + rB)
		return false;

	// 分離軸 : C32
	D3DXVec3Cross(&Cross, &an3, &bn2);
	rA = LenSegOnSeparateAxis(&Cross, &a1, &a2);
	rB = LenSegOnSeparateAxis(&Cross, &b1, &b3);
	l = fabs(D3DXVec3Dot(&interval, &Cross));
	if (l > rA + rB)
		return false;

	// 分離軸 : C33
	D3DXVec3Cross(&Cross, &an3, &bn3);
	rA = LenSegOnSeparateAxis(&Cross, &a1, &a2);
	rB = LenSegOnSeparateAxis(&Cross, &b1, &b2);
	l = fabs(D3DXVec3Dot(&interval, &Cross));
	if (l > rA + rB)
		return false;
		*/
	// 分離平面が存在しないので「衝突している」
	return true;


}
float LenSegOnSeparateAxis(D3DXVECTOR3 *Sep, D3DXVECTOR3 *e1, D3DXVECTOR3 *e2, D3DXVECTOR3 *e3)
{
	// 3つの内積の絶対値の和で投影線分長を計算
	// 分離軸Sepは標準化されていること
	FLOAT r1 = fabs((D3DXVec3Dot(Sep, e1)));
	FLOAT r2 = fabs((D3DXVec3Dot(Sep, e2)));
	FLOAT r3 = fabs((D3DXVec3Dot(Sep, e3)));

	return r1 + r2 + r3;
};