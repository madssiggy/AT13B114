//=====================================
//    
//   debugcollision.cpp
//              created by Koki Nakano
//
//=====================================


// <説明>
// これはデバッグ用、当たり判定描画のソースコードです。
// 村瀬クラスで配布されたrenderer.hが必要になります。
// static変数なのでインスタンスを生成せずに関数を書くことができます。
// それと、付属のwhite.pngもアセットフォルダに入れといてください。


#include "main.h"
#include "renderer.h"

#include "debugcollision.h"
#include "debugcollision2D.h"
#include "debugcollision3D.h"



#define DEBUG_TEXTURE_PATH "asset/texture/white.png"




ID3D11ShaderResourceView* CDebugCollision::m_Texture;        //テクスチャ



void CDebugCollision::Init()
{
	//テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
		DEBUG_TEXTURE_PATH,
		NULL,
		NULL,
		&m_Texture,
		NULL);

	//読み込み確認
	assert(m_Texture);


	//当たり判定の初期化
	DebugSphereCollision::Init(m_Texture);
	DebugCubeCollision::Init(m_Texture);
	DebugCircleCollision::Init(m_Texture);
	DebugSquareCollision::Init(m_Texture);
}




void CDebugCollision::Uninit()
{
	//当たり判定の終了化
	DebugSphereCollision::Uninit();
	DebugCubeCollision::Uninit();
	DebugCircleCollision::Uninit();
	DebugSquareCollision::Uninit();
}