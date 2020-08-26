#include "main.h"


#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "input.h"

#include "player.h"
#include "enemy.h"

#include "bullet.h"
#include "scene.h"

D3DXVECTOR3 mag(0.1f, 0.1f, 0.1f);//移動スピードの調節用。



void CEnemy::Init() {
	m_scene = CManager::GetScene();
	m_Model = new CModel();
	m_Model->Load("asset\\model\\human.obj");


	m_Position = D3DXVECTOR3(0.0f, 0.0f, 50.0f);
	m_Rotation = D3DXVECTOR3(30.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_playerPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


}
void CEnemy::Uninit() {

	m_Model->Unload();
	delete m_Model;
}
void CEnemy::Update() {
//リターン押すと追尾モードスタート
	if (isMove==false&&CInput::GetKeyTrigger(VK_RETURN)) {
		isMove = true;
	}

	if (isMove==true) {
		m_coolTime++;//球発射のやつ
//
		D3DXVECTOR3 moveWay = m_playerPos - m_Position;//ベクトルでエネミーからプレイヤーに向かって伸びるベクトルを作る
		D3DXVec3Normalize(&moveWay, &moveWay);//正規化。ベクトルをちっちゃくくする。でもまだベクトルとしてデカいのでちいちゃくする
		moveWay.x *= mag.x;
		moveWay.y *= mag.y;
		moveWay.z *= mag.z;
		m_Position += moveWay;//ベクトルを加算してその方向へ向かって移動
	
		if (m_coolTime >= m_shotTime) {
			D3DXVECTOR3 BulletSpeed;
			BulletSpeed.x=moveWay.x * 1.3f;
			BulletSpeed.y = moveWay.y * 1.3f;
			BulletSpeed.z = moveWay.z * 1.3f;
			m_scene->AddGameObject<CBullet>(1)->Create(m_Position, BulletSpeed,1);
			m_playerPos = m_scene->GetGameObject<CPlayer>(1)->GetPosition();
			m_coolTime = 0;
		}
	}
	
}
void CEnemy::Draw() {//マトリクス設定

		D3DXMATRIX world, scale, rot, trans;
		D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
		D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.x, m_Rotation.y, m_Rotation.z);
		D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
		world = scale * rot*trans;
		CRenderer::SetWorldMatrix(&world);



		m_Model->Draw();
	
}

void CEnemy::GetPlayerPos(D3DXVECTOR3 Ppos) {
	m_playerPos = Ppos;
}