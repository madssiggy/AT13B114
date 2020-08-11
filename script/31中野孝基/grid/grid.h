#ifndef _GRID_H_
#define _GRID_H_

//=====================================
//    
//   grid.cpp
//              created by Koki Nakano
//
//=====================================


#include "gameobject.h"


class CGrid : public CGameObject
{
private:
	ID3D11ShaderResourceView* m_Texture      = NULL;        //テクスチャ
	ID3D11Buffer*             m_VertexBuffer = NULL;        //頂点バッファ
public:
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};



#endif