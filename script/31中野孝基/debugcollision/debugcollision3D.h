//=====================================
//    
//   debugcollision3D.h
//              created by Koki Nakano
//
//=====================================


#pragma once
#ifndef _DEBUGCOLLISION3D_H_
#define _DEBUGCOLLISION3D_H_




#define CIRCLEVERTEX3D (32)      //1�̉~���\������̂ɕK�v�Ȓ��_��


class DebugSphereCollision
{
private:
	static ID3D11ShaderResourceView* m_Texture;             //�e�N�X�`��
	static ID3D11Buffer*             m_VertexBuffer[3];     //���_�o�b�t�@

	static void Draw_XYCircle();
	static void Draw_YZCircle();
	static void Draw_ZXCircle();

public:
	static void Init(ID3D11ShaderResourceView* texture);
	static void Uninit();
	static void Draw(D3DXVECTOR3 position, D3DXVECTOR3 rotation, float radius, D3DXCOLOR color);
	static void Draw(D3DXVECTOR3 position, D3DXQUATERNION quaternion, float radius, D3DXCOLOR color);
};



class DebugCubeCollision
{
private:
	static ID3D11ShaderResourceView* m_Texture;             //�e�N�X�`��
	static ID3D11Buffer* m_VertexBuffer;                    //���_�o�b�t�@
public:
	static void Init(ID3D11ShaderResourceView* texture);
	static void Uninit();
	static void Draw(D3DXVECTOR3 position, D3DXVECTOR3 rotation, D3DXVECTOR3 radius, D3DXCOLOR color);
	static void Draw(D3DXVECTOR3 position, D3DXQUATERNION quaternion, D3DXVECTOR3 radius, D3DXCOLOR color);
};

#endif