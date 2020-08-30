#include "main.h"
#include "renderer.h"
#include "Fade.h"




void CFade::Init()
{

	m_FadeColor = { 0.0f, 0.0f, 0.0f, 0.0f };

	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(m_FadeColor.r, m_FadeColor.b, m_FadeColor.g, m_FadeColor.a);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(m_FadeColor.r, m_FadeColor.b, m_FadeColor.g, m_FadeColor.a);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(m_FadeColor.r, m_FadeColor.b, m_FadeColor.g, m_FadeColor.a);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(m_FadeColor.r, m_FadeColor.b, m_FadeColor.g, m_FadeColor.a);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);


	//頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	//テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
		"asset/texture/white.png",
		NULL,
		NULL,
		&m_Texture,
		NULL
	);

	assert(m_Texture);	//リリース時には実行されない。nullの場合エラーを吐いてくれる
}


void CFade::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();
}

void CFade::Update()
{

	if (m_bFadeIn)
	{
		//経過時間
		int elapsedFrame = m_FrameCount - m_StartFrame;

		//完了比率を求める
		float ratio = (float)elapsedFrame / m_FadeFrame;
		//完了しているか
		if (ratio >= 1.0f)
		{
			//ratio = 1.0f;
			m_bFadeIn = false;
		}

		//α値を変える
		m_FadeColor.a = 1.0f - ratio;
	}
	else if (m_bFadeOut)
	{
		//経過時間
		int elapsedFrame = m_FrameCount - m_StartFrame;

		//完了比率を求める
		float ratio = (float)elapsedFrame / m_FadeFrame;

		//完了しているか
		if (ratio >= 1.0f)
		{
			//ratio = 1.0f;
			m_bFadeOut = false;
		}

		//α値を変える
		m_FadeColor.a = ratio;
	}

	m_FrameCount++;
}



void CFade::Draw()
{

	if (m_FadeColor.a <= 0.0001f)
		return;


	D3D11_MAPPED_SUBRESOURCE msr;
	CRenderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(m_FadeColor.r, m_FadeColor.b, m_FadeColor.g, m_FadeColor.a);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(m_FadeColor.r, m_FadeColor.b, m_FadeColor.g, m_FadeColor.a);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(m_FadeColor.r, m_FadeColor.b, m_FadeColor.g, m_FadeColor.a);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(m_FadeColor.r, m_FadeColor.b, m_FadeColor.g, m_FadeColor.a);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);


	CRenderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);


	//マトリクス設定
	CRenderer::SetWorldViewProjection2D();

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(
		0, 1, &m_VertexBuffer, &stride, &offset
	);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	CRenderer::SetMaterial(material);


	//テクスチャ設定
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//プリミティブトポロジ設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画
	CRenderer::GetDeviceContext()->Draw(4, 0);
}

/*-------------------------------------------------------------------------------------
	  Fade_Start
-------------------------------------------------------------------------------------*/
void CFade::Start(int frame, D3DXCOLOR color, bool bOut)
{
	m_FadeColor = color;	//色
	if (bOut)
	{//フェードinかoutか
		m_bFadeOut = true;
		m_FadeColor.a = 0.0f;
	}
	else
	{
		m_bFadeIn = true;
		m_FadeColor.a = 1.0f;
	}		
	m_FadeFrame = frame;
	m_StartFrame = m_FrameCount;	//フェードスタート時間
}

/*-------------------------------------------------------------------------------------
	  Fade_IsFade
-------------------------------------------------------------------------------------*/
bool CFade::IsFade()
{
	return m_bFadeOut || m_bFadeIn;
}