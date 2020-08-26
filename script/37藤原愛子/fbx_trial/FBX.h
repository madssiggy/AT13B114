/*
	   ／＼＿＿＿＿＿＿＿＿＿＿_＿／＼
　　 ／　　　　　　　　　　　　　　　 ＼
　　｜　FBXモデル読み込み  [fbx.h]     ｜
　　｜　　　　　　　　　　　　　　　　 ｜
　　｜　　　date	: 2020/08/26   　  ｜
　　｜　　　auther	: Aiko Fujiwara　  ｜
　　 ＼＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿_／
 */

 //注：モデル及びテクスチャのファイル名の全角文字に対応していない（調べれば対応方法でてくる）
 //注：なんか重い。インデックスを使ってないからかも


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

	unsigned int* m_Indices;	//未使用

	MATERIAL m_Material;

	ID3D11ShaderResourceView* m_Texture = NULL;


	FbxString m_TextureUvSetName;

	bool m_Anim;

	void LoadMaterial(const char *FileName);	//FileNameはテクスチャ取得時のパス参照に使う
	void LoadVertexData();


public:

	CMesh() = delete;

	CMesh(FbxNode* node, bool anim) : m_MeshNode(node), m_Anim(anim) {};


	void Draw(FbxTime timeCount);

	void Load(const char *FileName);	//FileNameはテクスチャ取得時のパス参照に使う
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

