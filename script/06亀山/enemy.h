#pragma once
#include "gameobject.h"
#include "collision.h"
#define ENEMY_INDEX (3)
class CEnemy:public CGameObject{
protected:
	CScene* m_scene;
	CModel* m_Model;
	D3DXVECTOR3 m_playerPos;
	CIRCLE m_Col;
	bool canUse=true;
	bool isMove=false;
	int m_shotTime;
	int m_coolTime;
public:
	CEnemy() {}
	~CEnemy() {}
	void Init();
	void Init(D3DXVECTOR3 pos);
	void Uninit();
	void Update();
	void Draw();
	
	void GetPlayerPos(D3DXVECTOR3 Ppos);
	const CIRCLE* GetCollision() { return &m_Col; }
	//void Destroy() { canUse = false; }
	void SetCanUse(bool set) { canUse = set; }
};