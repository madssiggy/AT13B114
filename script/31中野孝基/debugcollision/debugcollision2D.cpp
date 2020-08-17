//=====================================
//    
//   debugcollision2D.cpp
//              created by Koki Nakano
//
//=====================================


// <説明>
// これはデバッグ用、当たり判定描画のソースコードです。
// 村瀬クラスで配布されたrenderer.hが必要になります。
// static変数なのでインスタンスを生成せずに関数を書くことができます。
// CircleとSquareは別モノなので両方書いてください
// それと、付属のwhite.pngもアセットフォルダに入れといてください。

//法線の関係上回転をすると裏側が黒くなるので、自分で設定したライトを切ってください。
//ガイドをDraw関数の中にコメントで書いています。



#include "main.h"
#include "renderer.h"

#include "debugcollision2D.h"
#include "light.h"




//Circle用
ID3D11ShaderResourceView* DebugCircleCollision::m_Texture;        //テクスチャ
ID3D11Buffer* DebugCircleCollision::m_VertexBuffer;               //頂点バッファ


//Square用
ID3D11ShaderResourceView* DebugSquareCollision::m_Texture;        //テクスチャ
ID3D11Buffer* DebugSquareCollision::m_VertexBuffer;               //頂点バッファ



void DebugCircleCollision::Init(ID3D11ShaderResourceView* texture)
{
	//ポリゴンを設定
	VERTEX_3D vertex[CIRCLEVERTEX2D];

	for (int i = 0; i < CIRCLEVERTEX2D; i++)
	{
		vertex[i].Position = D3DXVECTOR3(cosf(D3DXToRadian(i * (360.0f / (CIRCLEVERTEX2D - 1)))) * 0.5f, sinf(D3DXToRadian(i * (360.0f / (CIRCLEVERTEX2D - 1)))) * 0.5f, 0.0f);
		vertex[i].Diffuse  = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[i].Normal   = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vertex[i].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
	}



	//頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));                  //構造体の中身を0で埋める
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * CIRCLEVERTEX2D;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;      //頂点バッファ
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);     //頂点バッファやインデックスバッファなど


	//テクスチャ読み込み
	m_Texture = texture;
}



void DebugCircleCollision::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();
}



void DebugCircleCollision::Draw(D3DXVECTOR3 position, D3DXVECTOR3 rotation, float radius, D3DXCOLOR color)
{
	//マトリクス設定(2Dを描画するための設定)
	CRenderer::SetWorldViewProjection2D();


	//マトリクス設定
	D3DXMATRIX mtxW, mtxT, mtxS, mtxR;
	D3DXMatrixScaling(&mtxS, radius, radius, 0.0f);
	D3DXMatrixRotationYawPitchRoll(&mtxR, rotation.y, rotation.x, 0.0f);
	D3DXMatrixTranslation(&mtxT, position.x, position.y, 0.0f);
	mtxW = mtxS * mtxR * mtxT;

	CRenderer::SetWorldMatrix(&mtxW);


	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = color;    //テクスチャを張るので白に設定
	CRenderer::SetMaterial(material);

	//テクスチャ設定
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//プリミティブトポロジ設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);


	//ポリゴン描画(頂点数,)
	CRenderer::GetDeviceContext()->Draw(CIRCLEVERTEX2D, 0);
}









void DebugSquareCollision::Init(ID3D11ShaderResourceView* texture)
{
	//ポリゴンを設定
	VERTEX_3D vertex[4];

	{
		vertex[0].Position = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);
		vertex[1].Position = D3DXVECTOR3( 0.5f, -0.5f, 0.0f);
		vertex[2].Position = D3DXVECTOR3(-0.5f,  0.5f, 0.0f);
		vertex[3].Position = D3DXVECTOR3( 0.5f,  0.5f, 0.0f);
	}

	for (int i = 0; i < 4; i++)
	{
		vertex[i].Normal   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vertex[i].Diffuse  = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[i].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
	}



	//頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));                  //構造体の中身を0で埋める
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;      //頂点バッファ
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);     //頂点バッファやインデックスバッファなど


	//テクスチャ読み込み
	m_Texture = texture;
}



void DebugSquareCollision::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();
}



void DebugSquareCollision::Draw(D3DXVECTOR3 position, D3DXVECTOR3 rotation, D3DXVECTOR3 radius, D3DXCOLOR color)
{
	//マトリクス設定(2Dを描画するための設定)
	CRenderer::SetWorldViewProjection2D();


	//マトリクス設定
	D3DXMATRIX mtxW, mtxT, mtxS, mtxR;
	D3DXMatrixScaling(&mtxS, radius.x, radius.y, 0.0f);
	D3DXMatrixRotationYawPitchRoll(&mtxR, rotation.y, rotation.x, 0.0f);
	D3DXMatrixTranslation(&mtxT, position.x, position.y, 0.0f);
	mtxW = mtxS * mtxR * mtxT;

	CRenderer::SetWorldMatrix(&mtxW);


	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = color;    //テクスチャを張るので白に設定
	CRenderer::SetMaterial(material);

	//テクスチャ設定
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//プリミティブトポロジ設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);


	//ポリゴン描画(頂点数,)
	CRenderer::GetDeviceContext()->Draw(4, 0);
}