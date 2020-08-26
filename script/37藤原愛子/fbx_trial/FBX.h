/*
	   �^�_�Q�Q�Q�Q�Q�Q�Q�Q�Q�Q_�Q�^�_
�@�@ �^�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ �_
�@�@�b�@FBX���f���ǂݍ���  [fbx.h]     �b
�@�@�b�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ �b
�@�@�b�@�@�@date	: 2020/08/26   �@  �b
�@�@�b�@�@�@auther	: Aiko Fujiwara�@  �b
�@�@ �_�Q�Q�Q�Q�Q�Q�Q�Q�Q�Q�Q�Q�Q�Q�Q_�^
 */

 //���F���f���y�уe�N�X�`���̃t�@�C�����̑S�p�����ɑΉ����Ă��Ȃ��i���ׂ�ΑΉ����@�łĂ���j
 //���F�Ȃ񂩏d���B�C���f�b�N�X���g���ĂȂ����炩��


#pragma once

#include <fbxsdk.h>
#include <vector>


class CMesh
{
	ID3D11Buffer*	m_VertexBuffer;
	ID3D11Buffer*	m_IndexBuffer;

	FbxMesh *m_Mesh = NULL;

	FbxNode *m_MeshNode = NULL;

	VERTEX_3D *m_Vertices;

	unsigned int* m_Indices;	//���g�p

	MATERIAL m_Material;

	ID3D11ShaderResourceView* m_Texture = NULL;


	FbxString m_TextureUvSetName;

	bool m_Anim;

	void LoadMaterial(const char *FileName);	//FileName�̓e�N�X�`���擾���̃p�X�Q�ƂɎg��
	void LoadVertexData();


public:

	CMesh() = delete;

	CMesh(FbxNode* node, bool anim) : m_MeshNode(node), m_Anim(anim) {};


	void Draw(FbxTime timeCount);

	void Load(const char *FileName);	//FileName�̓e�N�X�`���擾���̃p�X�Q�ƂɎg��
	void Unload();

};

class CModelFBX
{
private:


	const char *m_FileName;

	FbxManager *m_FbxManager = NULL;
	FbxScene *m_FbxScene = NULL;

	std::vector<CMesh*> m_Mesh;


	FbxTime m_FrameTime, m_TimeCount, m_StartTime, m_StopTime;
	int m_AnimStackNumber = 0;

	bool m_Anim;


	void CreateMeshNode(FbxNode* node);

public:

	void Draw();

	void Load(const char *FileName);
	void Unload();


};

