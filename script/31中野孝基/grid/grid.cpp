//=====================================
//    
//   grid.cpp
//              created by Koki Nakano
//
//=====================================


// <����>
// ����̓f�o�b�O�p�A�O���b�h�`��̃\�[�X�R�[�h�ł��B
// �����N���X�Ŕz�z���ꂽrenderer.h���K�v�ɂȂ�܂��B
// �܂��ACGameObject�N���X���p�����Ă���̂ŁA�����p�����Ȃ��̂ł���΁A
// D3DXVECTOR3 m_Position ��ǉ����Ă��������B
// ����ƁA�t����white.png���A�Z�b�g�t�H���_�ɓ���Ƃ��Ă��������B

//��{�I�Ɋ֐����̃R�[�h�͏����������ɉ��L�� #define 3�����������Ă�������



#include "main.h"
#include "renderer.h"
#include "grid.h"
#include "light.h"


//1���C���̃u���b�N��
#define GRID_SQUARE_NUM (20)  

//�O���b�h�J���[
#define GRID_COLOR D3DXVECTOR4(1.0f ,1.0f ,1.0f ,1.0f)

//�摜�̃t�@�C���p�X
#define GRID_TEXTURE_PASS "asset/texture/white.png"



//�\���ɕK�v�Ȓ��_��
#define GRID_VERTEX_NUM ((GRID_SQUARE_NUM + 1) * 4)



void CGrid::Init()
{      
	int vtx_half       = (GRID_SQUARE_NUM + 1) * 2;        //�S���_�̒���(�c�Ɖ��̐؂�ւ��n�_)
	float vtx_num_harf = GRID_SQUARE_NUM * 0.5f;           //���E�O��̕�(���a)



	//�|���S����ݒ�
	VERTEX_3D vertex[GRID_VERTEX_NUM];

	//x���ƕ��s�Ȑ�
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
	//z���ƕ��s�Ȑ�
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




	//���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));                            //�\���̂̒��g��0�Ŗ��߂�
	bd.Usage = D3D11_USAGE_DEFAULT;               
	bd.ByteWidth = sizeof(VERTEX_3D) * GRID_VERTEX_NUM;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;                //���_�o�b�t�@
	bd.CPUAccessFlags = 0;   

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;


	//�e�N�X�`���ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
		GRID_TEXTURE_PASS,
		NULL,
		NULL,
		&m_Texture,
		NULL);

	//�ǂݍ��݊m�F
	assert(m_Texture);    


	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);     //���_�o�b�t�@��C���f�b�N�X�o�b�t�@�Ȃ�
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
	CLight::Switch(false);



	//�}�g���N�X�ݒ�
	D3DXMATRIX mtxT;
	D3DXMatrixTranslation(&mtxT, m_Position.x, m_Position.y, m_Position.z);
	
	CRenderer::SetWorldMatrix(&mtxT);



	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);


	//�}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	CRenderer::SetMaterial(material);

	//�e�N�X�`���ݒ�
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//�v���~�e�B�u�g�|���W�ݒ�
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);


	//�|���S���`��(���_��,)
	CRenderer::GetDeviceContext()->Draw(GRID_VERTEX_NUM, 0);


	CLight::Switch(true);
#endif
}