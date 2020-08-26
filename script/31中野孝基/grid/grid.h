//=====================================
//    
//   grid.cpp
//              created by Koki Nakano
//
//=====================================

#ifndef _GRID_H_
#define _GRID_H_


#include "gameobject.h"


class CGrid : public CGameObject
{
private:
	ID3D11ShaderResourceView* m_Texture      = NULL;        //�e�N�X�`��
	ID3D11Buffer*             m_VertexBuffer = NULL;        //���_�o�b�t�@
public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};



#endif