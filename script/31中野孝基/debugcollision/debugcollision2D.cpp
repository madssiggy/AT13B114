//=====================================
//    
//   debugcollision2D.cpp
//              created by Koki Nakano
//
//=====================================


// <����>
// ����̓f�o�b�O�p�A�����蔻��`��̃\�[�X�R�[�h�ł��B
// �����N���X�Ŕz�z���ꂽrenderer.h���K�v�ɂȂ�܂��B
// static�ϐ��Ȃ̂ŃC���X�^���X�𐶐������Ɋ֐����������Ƃ��ł��܂��B
// Circle��Square�͕ʃ��m�Ȃ̂ŗ��������Ă�������
// ����ƁA�t����white.png���A�Z�b�g�t�H���_�ɓ���Ƃ��Ă��������B

//�@���̊֌W���]������Ɨ����������Ȃ�̂ŁA�����Őݒ肵�����C�g��؂��Ă��������B
//�K�C�h��Draw�֐��̒��ɃR�����g�ŏ����Ă��܂��B



#include "main.h"
#include "renderer.h"

#include "debugcollision2D.h"
#include "light.h"




//Circle�p
ID3D11ShaderResourceView* DebugCircleCollision::m_Texture;        //�e�N�X�`��
ID3D11Buffer* DebugCircleCollision::m_VertexBuffer;               //���_�o�b�t�@


//Square�p
ID3D11ShaderResourceView* DebugSquareCollision::m_Texture;        //�e�N�X�`��
ID3D11Buffer* DebugSquareCollision::m_VertexBuffer;               //���_�o�b�t�@



void DebugCircleCollision::Init(ID3D11ShaderResourceView* texture)
{
	//�|���S����ݒ�
	VERTEX_3D vertex[CIRCLEVERTEX2D];

	for (int i = 0; i < CIRCLEVERTEX2D; i++)
	{
		vertex[i].Position = D3DXVECTOR3(cosf(D3DXToRadian(i * (360.0f / (CIRCLEVERTEX2D - 1)))) * 0.5f, sinf(D3DXToRadian(i * (360.0f / (CIRCLEVERTEX2D - 1)))) * 0.5f, 0.0f);
		vertex[i].Diffuse  = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[i].Normal   = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vertex[i].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
	}



	//���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));                  //�\���̂̒��g��0�Ŗ��߂�
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * CIRCLEVERTEX2D;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;      //���_�o�b�t�@
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);     //���_�o�b�t�@��C���f�b�N�X�o�b�t�@�Ȃ�


	//�e�N�X�`���ǂݍ���
	m_Texture = texture;
}



void DebugCircleCollision::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();
}



void DebugCircleCollision::Draw(D3DXVECTOR3 position, D3DXVECTOR3 rotation, float radius, D3DXCOLOR color)
{
	//�}�g���N�X�ݒ�(2D��`�悷�邽�߂̐ݒ�)
	CRenderer::SetWorldViewProjection2D();


	//�}�g���N�X�ݒ�
	D3DXMATRIX mtxW, mtxT, mtxS, mtxR;
	D3DXMatrixScaling(&mtxS, radius, radius, 0.0f);
	D3DXMatrixRotationYawPitchRoll(&mtxR, rotation.y, rotation.x, 0.0f);
	D3DXMatrixTranslation(&mtxT, position.x, position.y, 0.0f);
	mtxW = mtxS * mtxR * mtxT;

	CRenderer::SetWorldMatrix(&mtxW);


	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//�}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = color;    //�e�N�X�`���𒣂�̂Ŕ��ɐݒ�
	CRenderer::SetMaterial(material);

	//�e�N�X�`���ݒ�
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//�v���~�e�B�u�g�|���W�ݒ�
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);


	//�|���S���`��(���_��,)
	CRenderer::GetDeviceContext()->Draw(CIRCLEVERTEX2D, 0);
}









void DebugSquareCollision::Init(ID3D11ShaderResourceView* texture)
{
	//�|���S����ݒ�
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



	//���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));                  //�\���̂̒��g��0�Ŗ��߂�
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;      //���_�o�b�t�@
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);     //���_�o�b�t�@��C���f�b�N�X�o�b�t�@�Ȃ�


	//�e�N�X�`���ǂݍ���
	m_Texture = texture;
}



void DebugSquareCollision::Uninit()
{
	m_VertexBuffer->Release();
	m_Texture->Release();
}



void DebugSquareCollision::Draw(D3DXVECTOR3 position, D3DXVECTOR3 rotation, D3DXVECTOR3 radius, D3DXCOLOR color)
{
	//�}�g���N�X�ݒ�(2D��`�悷�邽�߂̐ݒ�)
	CRenderer::SetWorldViewProjection2D();


	//�}�g���N�X�ݒ�
	D3DXMATRIX mtxW, mtxT, mtxS, mtxR;
	D3DXMatrixScaling(&mtxS, radius.x, radius.y, 0.0f);
	D3DXMatrixRotationYawPitchRoll(&mtxR, rotation.y, rotation.x, 0.0f);
	D3DXMatrixTranslation(&mtxT, position.x, position.y, 0.0f);
	mtxW = mtxS * mtxR * mtxT;

	CRenderer::SetWorldMatrix(&mtxW);


	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	//�}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = color;    //�e�N�X�`���𒣂�̂Ŕ��ɐݒ�
	CRenderer::SetMaterial(material);

	//�e�N�X�`���ݒ�
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	//�v���~�e�B�u�g�|���W�ݒ�
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);


	//�|���S���`��(���_��,)
	CRenderer::GetDeviceContext()->Draw(4, 0);
}