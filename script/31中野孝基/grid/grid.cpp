//=====================================
//    
//   grid.cpp
//              created by Koki Nakano
//
//=====================================


// <説明>
// これはデバッグ用、グリッド描画のソースコードです。
// 村瀬クラスで配布されたrenderer.hが必要になります。
// また、CGameObjectクラスを継承しているので、もし継承しないのであれば、
// D3DXVECTOR3 m_Position を追加してください。
// それと、付属のwhite.pngもアセットフォルダに入れといてください。

//基本的に関数内のコードは書き換えずに下記の #define 3つを書き換えてください



#include "main.h"
#include "renderer.h"
#include "grid.h"



//1ラインのブロック数
#define GRID_SQUARE_NUM (20)  

//グリッドカラー
#define GRID_COLOR D3DXVECTOR4(1.0f ,1.0f ,1.0f ,1.0f)

//画像のファイルパス
#define GRID_TEXTURE_PASS "asset/texture/white.png"



//構成に必要な頂点数
#define GRID_VERTEX_NUM ((GRID_SQUARE_NUM + 1) * 4)



void CGrid::Init()
{      
	int vtx_half       = (GRID_SQUARE_NUM + 1) * 2;        //全頂点の中間(縦と横の切り替わり地点)
	float vtx_num_harf = GRID_SQUARE_NUM * 0.5f;           //左右前後の幅(半径)



	//ポリゴンを設定
	VERTEX_3D vertex[GRID_VERTEX_NUM];

	//x軸と平行な線
	for (int i = 0; i < GRID_SQUARE_NUM + 1; i++)
	{
		vertex[i * 2].Position     = D3DXVECTOR3(-vtx_num_harf, 0.0f, -vtx_num_harf + i);
		vertex[i * 2].Diffuse      = GRID_COLOR;
		vertex[i * 2].Normal       = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vertex[i * 2].TexCoord     = D3DXVECTOR2(0.0f, 0.0f);

		vertex[i * 2 + 1].Position = D3DXVECTOR3(vtx_num_harf, 0.0f, -vtx_num_harf + i);
		vertex[i * 2 + 1].Diffuse  = GRID_COLOR;
		vertex[i * 2 + 1].Normal   = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vertex[i * 2 + 1].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
	}
	//z軸と平行な線
	for (int i = 0; i < GRID_SQUARE_NUM + 1; i++)
	{
		vertex[i * 2 + vtx_half].Position     = D3DXVECTOR3(-vtx_num_harf + i, 0.0f, -vtx_num_harf);
		vertex[i * 2 + vtx_half].Diffuse      = GRID_COLOR;
		vertex[i * 2 + vtx_half].Normal       = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vertex[i * 2 + vtx_half].TexCoord     = D3DXVECTOR2(0.0f, 0.0f);

		vertex[i * 2 + vtx_half + 1].Position = D3DXVECTOR3(-vtx_num_harf + i, 0.0f, vtx_num_harf);
		vertex[i * 2 + vtx_half + 1].Diffuse  = GRID_COLOR;
		vertex[i * 2 + vtx_half + 1].Normal   = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vertex[i * 2 + vtx_half + 1].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
	}




	//頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));                  //構造体の中身を0で埋める
	bd.Usage = D3D11_USAGE_DYNAMIC;               //頂点の情報を後で書き換えられるようにする
	bd.ByteWidth = sizeof(VERTEX_3D) * GRID_VERTEX_NUM;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;      //頂点バッファ
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;   //頂点の情報を後で書き換えられるようにする

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;


	//テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
		GRID_TEXTURE_PASS,
		NULL,
		NULL,
		&m_Texture,
		NULL);

	//読み込み確認
	assert(m_Texture);    


	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);     //頂点バッファやインデックスバッファなど
}



void CGrid::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();
}



void CGrid::Update()
{

}



void CGrid::Draw()
{
#if defined(_DEBUG) || defined(DEBUG)


	//マトリクス設定
	D3DXMATRIX mtxT;
	D3DXMatrixTranslation(&mtxT, m_Position.x, m_Position.y, m_Position.z);
	
	CRenderer::SetWorldMatrix(&mtxT);



	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);


	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	CRenderer::SetMaterial(material);

	//テクスチャ設定
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//プリミティブトポロジ設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);


	//ポリゴン描画(頂点数,)
	CRenderer::GetDeviceContext()->Draw(GRID_VERTEX_NUM, 0);


#endif
}