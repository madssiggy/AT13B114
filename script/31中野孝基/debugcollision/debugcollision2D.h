//=====================================
//    
//   debugcollision3D.h
//              created by Koki Nakano
//
//=====================================


#pragma once
#ifndef _DEBUGCOLLISION2D_H_
#define _DEBUGCOLLISION2D_H_




#define CIRCLEVERTEX2D (32)


class DebugCircleCollision
{
private:
	static ID3D11ShaderResourceView* m_Texture;             //テクスチャ
	static ID3D11Buffer* m_VertexBuffer;				    //頂点バッファ

public:
	static void Init(ID3D11ShaderResourceView* texture);
	static void Uninit();
	static void Draw(D3DXVECTOR3 position, D3DXVECTOR3 rotation, float radius, D3DXCOLOR color);
};



class DebugSquareCollision
{
private:
	static ID3D11ShaderResourceView* m_Texture;             //テクスチャ
	static ID3D11Buffer* m_VertexBuffer;                    //頂点バッファ

public:
	static void Init(ID3D11ShaderResourceView* texture);
	static void Uninit();
	static void Draw(D3DXVECTOR3 position, D3DXVECTOR3 rotation, D3DXVECTOR3 radius, D3DXCOLOR color);
};

#endif