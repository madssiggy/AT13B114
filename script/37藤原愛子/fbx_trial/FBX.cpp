/*
	   ／＼＿＿＿＿＿＿＿＿＿＿_＿／＼
　　 ／　　　　　　　　　　　　　　　 ＼
　　｜　FBXモデル読み込み  [fbx.cpp]   ｜
　　｜　　　　　　　　　　　　　　　　 ｜
　　｜　　　date	: 2020/08/26   　  ｜
　　｜　　　auther	: Aiko Fujiwara　  ｜
　　 ＼＿＿＿＿＿＿＿＿＿＿＿＿＿＿＿_／
 */

//↓VSで追加の依存ファイル設定しない人用
#pragma comment (lib, "libfbxsdk-mt.lib")
#pragma comment (lib, "libxml2-mt.lib")
#pragma comment (lib, "zlib-mt.lib")


//テクスチャがなかった時用のテクスチャ、用意する必要があります
#define NONE_TEXTURE_PATH ("asset\\texture\\white.png")
//仮アニメーションナンバー（ここでセットした番号のアニメーションを再生、実用的じゃないのでカスタマイズ要）
#define ANIM_NUMBER (0)


#include "main.h"
#include "renderer.h"
#include "FBX.h"


void CModelFBX::Load(const char *FileName)
{

	m_FileName = FileName;


	//****************FBXの読み込み************************
	m_FbxManager = FbxManager::Create();
	assert(m_FbxManager);

	m_FbxScene = FbxScene::Create(m_FbxManager, "fbxscene");
	assert(m_FbxScene);

	FbxImporter *fbxImporter = FbxImporter::Create(m_FbxManager, "imp");
	assert(fbxImporter);

	//ファイル読み込み
	assert(fbxImporter->Initialize(FileName, -1, m_FbxManager->GetIOSettings()));
	fbxImporter->Import(m_FbxScene);

	fbxImporter->Destroy();

	
	FbxGeometryConverter converter(m_FbxManager);
	// ポリゴンを三角形にする
	converter.Triangulate(m_FbxScene, true);

	
	//********アニメーション読み込み****************************

	m_Anim = true;
	m_AnimStackNumber = ANIM_NUMBER;

	//アニメーション名一覧取得
	FbxArray<FbxString*> AnimStackNameArray;
	m_FbxScene->FillAnimStackNameArray(AnimStackNameArray);

	int animation_number_count = m_FbxScene->GetSrcObjectCount<FbxAnimStack>();
	if (animation_number_count > 0 && m_AnimStackNumber < animation_number_count)
	{//アニメーションあり

		//暫定的にAnimStackNumber番目のアニメーションのみを取得
		FbxAnimStack *AnimationStack = m_FbxScene->FindMember<FbxAnimStack>(AnimStackNameArray[m_AnimStackNumber]->Buffer());
		//現在のアニメーションに設定
		m_FbxScene->SetCurrentAnimationStack(AnimationStack);

		//タイムの設定
		FbxTakeInfo *takeInfo = m_FbxScene->GetTakeInfo(*(AnimStackNameArray[m_AnimStackNumber]));
		m_StartTime = takeInfo->mLocalTimeSpan.GetStart();
		m_StopTime = takeInfo->mLocalTimeSpan.GetStop();
		//１フレームの時間を設定
		m_FrameTime.SetTime(0, 0, 0, 1, 0, FbxTime::eFrames60);
		//FrameTime.SetTime(0, 0, 0, 1, 0, m_FbxScene->GetGlobalSettings().GetTimeMode());
		m_TimeCount = m_StartTime;

		// 1フレーム時間で割ればフレーム数になります
		//int startFrame = (int)(m_StartTime.Get() / m_FrameTime.Get());
		//int stopFrame = (int)(m_StartTime.Get() / m_FrameTime.Get());

	}
	else
	{
		m_Anim = false;
	}
	

	//*****************メッシュ作成************************************
	CreateMeshNode(m_FbxScene->GetRootNode());


}


void CModelFBX::Unload()
{
	for (CMesh* mesh : m_Mesh)
	{
		mesh->Unload();
		delete mesh;
	}
	m_Mesh.clear();


	m_FbxScene->Destroy();
	m_FbxManager->Destroy();

}

void CModelFBX::Draw()
{
	if (m_Anim)
	{
		//アニメーション更新
		m_TimeCount += m_FrameTime;
		if (m_TimeCount > m_StopTime) m_TimeCount = m_StartTime;

	}

	for (CMesh* mesh : m_Mesh)
	{
		mesh->Draw(m_TimeCount);
	}


}

void CModelFBX::CreateMeshNode(FbxNode* node)
{
	//ノード内のメッシュの取り出し
	for (int i = 0; i < node->GetNodeAttributeCount(); i++)
	{
		if (node->GetNodeAttributeByIndex(i)->GetAttributeType() == FbxNodeAttribute::eMesh) {

			CMesh* mesh = new CMesh(node, m_Anim);
			mesh->Load(m_FileName);
			m_Mesh.push_back(mesh);
		}
	}

	//子ノードも調べる
	for (int i = 0; i < node->GetChildCount(); i++) {

		CreateMeshNode(node->GetChild(i));

	}
}


void CMesh::Load(const char *FileName)
{
	
	m_Mesh = m_MeshNode->GetMesh();
	m_TextureUvSetName = "";


	LoadMaterial(FileName);

	LoadVertexData();

	//使ってない
	/*
	// インデックスバッファ生成
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(int) * m_Mesh->GetPolygonVertexCount();
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_Mesh->GetPolygonVertices();
		//sd.pSysMem = m_Indices;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);
	}

	*/

}

void CMesh::Unload()
{
	m_VertexBuffer->Release();
	//m_IndexBuffer->Release();

	delete[] m_Vertices;

	//delete[] m_Indices;

	m_Texture->Release();
}

void CMesh::Draw(FbxTime timeCount)
{
	
	//*************アニメーション処理*****************
	if (m_Anim)
	{

		//マトリクス作成
		FbxMatrix globalPosition = m_MeshNode->EvaluateGlobalTransform(timeCount);
		FbxVector4 t0 = m_MeshNode->GetGeometricTranslation(FbxNode::eSourcePivot);
		FbxVector4 r0 = m_MeshNode->GetGeometricRotation(FbxNode::eSourcePivot);
		FbxVector4 s0 = m_MeshNode->GetGeometricScaling(FbxNode::eSourcePivot);
		FbxAMatrix geometryOffset = FbxAMatrix(t0, r0, s0);


		//コントロールポイント分マトリクス作成
		FbxMatrix *clusterDeformation = new FbxMatrix[m_Mesh->GetControlPointsCount()];
		memset(clusterDeformation, 0, sizeof(FbxMatrix) * m_Mesh->GetControlPointsCount());

		FbxSkin *skinDeformer = (FbxSkin *)m_Mesh->GetDeformer(0, FbxDeformer::eSkin);
		if (skinDeformer != nullptr)
		{
			int clusterCount = skinDeformer->GetClusterCount();

			for (int clusterIndex = 0; clusterIndex < clusterCount; clusterIndex++) {
				// クラスタ(ボーン)の取り出し（よくわかってない）
				FbxCluster *cluster = skinDeformer->GetCluster(clusterIndex);
				FbxMatrix vertexTransformMatrix;
				FbxAMatrix referenceGlobalInitPosition;
				FbxAMatrix clusterGlobalInitPosition;
				FbxMatrix clusterGlobalCurrentPosition;
				FbxMatrix clusterRelativeInitPosition;
				FbxMatrix clusterRelativeCurrentPositionInverse;
				cluster->GetTransformMatrix(referenceGlobalInitPosition);
				referenceGlobalInitPosition *= geometryOffset;
				cluster->GetTransformLinkMatrix(clusterGlobalInitPosition);
				clusterGlobalCurrentPosition = cluster->GetLink()->EvaluateGlobalTransform(timeCount);
				clusterRelativeInitPosition = clusterGlobalInitPosition.Inverse() * referenceGlobalInitPosition;
				clusterRelativeCurrentPositionInverse = globalPosition.Inverse() * clusterGlobalCurrentPosition;
				vertexTransformMatrix = clusterRelativeCurrentPositionInverse * clusterRelativeInitPosition;

				for (int i = 0; i < cluster->GetControlPointIndicesCount(); i++) {
					int index = cluster->GetControlPointIndices()[i];
					double weight = cluster->GetControlPointWeights()[i];
					FbxMatrix influence = vertexTransformMatrix * weight;
					clusterDeformation[index] += influence;
				}
			}


			//インデックスデータ取得
			int* indices = m_Mesh->GetPolygonVertices();

			for (int i = 0; i < m_Mesh->GetPolygonVertexCount(); i++)
			{//インデックスからコントロールポイントを呼び出して全インデックス分いれる
				int index = indices[i];
				FbxVector4 outVertex = clusterDeformation[index].MultNormalize(m_Mesh->GetControlPointAt(index));
				m_Vertices[i].Position.x = (FLOAT)outVertex[0];
				m_Vertices[i].Position.y = (FLOAT)outVertex[1];
				m_Vertices[i].Position.z = (FLOAT)outVertex[2];

			}
			delete[] clusterDeformation;


			//データをバッファに入れ直す
			D3D11_MAPPED_SUBRESOURCE msr;
			CRenderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

			memcpy_s(msr.pData, msr.RowPitch, (void*)(m_Vertices), sizeof(VERTEX_3D) * m_Mesh->GetPolygonVertexCount());

			CRenderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

		}
		else
		{
			m_Anim = false;
		}

	}



	//**************描画***************************

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// インデックスバッファ設定
	//CRenderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// プリミティブトポロジ設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// マテリアル設定
	CRenderer::SetMaterial(m_Material);

	// テクスチャ設定
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// ポリゴン描画
	//CRenderer::GetDeviceContext()->DrawIndexed(m_Mesh->GetPolygonVertexCount(), 0, 0);
	CRenderer::GetDeviceContext()->Draw(m_Mesh->GetPolygonVertexCount(), 0);
}



void CMesh::LoadMaterial(const char *FileName)
{
	//メッシュごとにマテリアルを読み込み
	//（マテリアルを先に読み込んだ後に関連付けたほうがよさげ）

	bool texture_none = true;

	if (m_Mesh->GetElementMaterialCount() > 0)
	{//メッシュにマテリアル有

		//********1メッシュ1マテリアルのみの対応、最初のマテリアルを読み込む（マルチマテリアルの処理不明 ＞＜ ）

		// Mesh側のマテリアル情報を取得
		FbxLayerElementMaterial* material = m_Mesh->GetElementMaterial(0);

		// FbxSurfaceMaterialのインデックスバッファからインデックスを取得
		int	index = material->GetIndexArray().GetAt(0);
		
		// GetSrcObject<FbxSurfaceMaterial>でマテリアル取得
		FbxSurfaceMaterial* surface_material = m_MeshNode->GetSrcObject<FbxSurfaceMaterial>(index);

		if (surface_material != nullptr)
		{
			//FbxSurfaceMaterialは二種類ある、分岐入れてないのでLambertの方だとSpecular取得できない
			FbxProperty prop = surface_material->FindProperty(FbxSurfaceMaterial::sAmbient);

			FbxDouble3 ambient = prop.Get<FbxDouble3>();
			m_Material.Ambient.r = ambient[0];
			m_Material.Ambient.g = ambient[1];
			m_Material.Ambient.b = ambient[2];
			m_Material.Ambient.a = 1.0f;

			prop = surface_material->FindProperty(FbxSurfaceMaterial::sEmissive);
			FbxDouble3 emissive = prop.Get<FbxDouble3>();
			m_Material.Emission.r = emissive[0];
			m_Material.Emission.g = emissive[1];
			m_Material.Emission.b = emissive[2];
			m_Material.Emission.a = 0.0f;


			prop = surface_material->FindProperty(FbxSurfaceMaterial::sSpecular);
			FbxDouble3 specular = prop.Get<FbxDouble3>();
			m_Material.Specular.r = specular[0];
			m_Material.Specular.g = specular[1];
			m_Material.Specular.b = specular[2];
			m_Material.Specular.a = 1.0f;

			prop = surface_material->FindProperty(FbxSurfaceMaterial::sDiffuse);

			FbxDouble3 diffuse = prop.Get<FbxDouble3>();
			m_Material.Diffuse.r = diffuse[0];
			m_Material.Diffuse.g = diffuse[1];
			m_Material.Diffuse.b = diffuse[2];
			m_Material.Diffuse.a = 1.0f;


			//******テクスチャ読み込み（これもマテリアルごとに読み込んでるから本来は全部読み込んだ後に関連付けたほうがよさげ）
			
			//テクスチャはFbxFileTextureかFbxLayeredTextureのどちらか。Layeredの場合複数テクスチャ
			int TextureNum = prop.GetSrcObjectCount<FbxFileTexture>();

			if (TextureNum > 0)
			{//通常テクスチャ
				for (int i = 0; i < TextureNum; i++)
				{
					//propからFbxFileTextureを取得
					FbxFileTexture* texture = prop.GetSrcObject<FbxFileTexture>(i);
					// テクスチャファイルパスを取得
					std::string Path = texture->GetRelativeFileName();

					Path = (std::string)FileName + "\\..\\" + Path;

					//テクスチャ読み込み
					D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
						Path.c_str(),
						NULL,
						NULL,
						&m_Texture,
						NULL
					);

					//uv取得用
					m_TextureUvSetName = texture->UVSet;


					if (m_Texture != nullptr)
					{//取得成功
						texture_none = false;
					}

					break;	//そもそも1じゃない時があるのか謎
				}

			}
			else
			{
				//複数テクスチャかどうか調べる
				int layeredTextureCount = prop.GetSrcObjectCount<FbxLayeredTexture>();

				if (layeredTextureCount > 0)
				{//複数テクスチャ
					for (int j = 0; j < layeredTextureCount; j++) 
					{
						//ひとつづつ通常テクスチャを取得して読み込む
						FbxLayeredTexture* layeredTexture = prop.GetSrcObject<FbxLayeredTexture>(j);
						int textureCount = layeredTexture->GetSrcObjectCount<FbxFileTexture>();

						for (int k = 0; k < textureCount; k++) {
							FbxFileTexture* texture = prop.GetSrcObject<FbxFileTexture>(k);

							if (texture) {
								// テクスチャファイルパスを取得
								std::string Path = texture->GetRelativeFileName();

								Path = (std::string)FileName + "\\..\\" + Path;

								//テクスチャ読み込み
								D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
									Path.c_str(),
									NULL,
									NULL,
									&m_Texture,
									NULL
								);


								m_TextureUvSetName = texture->UVSet;

								
								if (m_Texture != nullptr)
								{//取得成功
									texture_none = false;
								}

								break;	//そもそも1じゃない時があるのか謎

							}
						}
						break;	//本来全部テクスチャ読み込んで何かするはずだけど、とりあえず使ってないから一枚だけでbreak
					}
				}
				
			}
		}

	}

	if (texture_none)
	{
		//テクスチャがない時用に白テクスチャ読み込み
		D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
			NONE_TEXTURE_PATH,
			NULL,
			NULL,
			&m_Texture,
			NULL
		);

		//パスの場所にない時はエラー吐く
		assert(m_Texture);
	}
}


void CMesh::LoadVertexData()
{

	//***********頂点データ作成*******************
	int vertixSize = m_Mesh->GetPolygonVertexCount();
	m_Vertices = new VERTEX_3D[vertixSize];


	{
		// *********頂点バッファ生成　　（アニメーションするかどうかでバッファの作り方変えたほうが良さそう）
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * vertixSize;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		CRenderer::GetDevice()->CreateBuffer(&bd, NULL, &m_VertexBuffer);
	}


	int* indices = m_Mesh->GetPolygonVertices();

	//************座標取得********************
	for (int i = 0; i < vertixSize; i++)
	{//インデックスからコントロールポイントを呼び出して全インデックス分いれる
		int index = indices[i];
		m_Vertices[i].Position.x = (FLOAT)m_Mesh->GetControlPointAt(index)[0];
		m_Vertices[i].Position.y = (FLOAT)m_Mesh->GetControlPointAt(index)[1];
		m_Vertices[i].Position.z = (FLOAT)m_Mesh->GetControlPointAt(index)[2];

		//とりあえず白(カラー取得できるはず)
		m_Vertices[i].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	}


	//***********法線取得**********************

	int layerNum = m_Mesh->GetLayerCount();
	for (int i = 0; i < layerNum; i++) {
		FbxLayer* layer = m_Mesh->GetLayer(i);
		FbxLayerElementNormal* normalElem = layer->GetNormals();
		if (normalElem == 0) {
			continue;   // 法線無し
		}

		// マッピングモード・リファレンスモード取得
		FbxLayerElement::EMappingMode mappingMode = normalElem->GetMappingMode();
		FbxLayerElement::EReferenceMode refMode = normalElem->GetReferenceMode();

		if (mappingMode == FbxLayerElement::eByPolygonVertex) {
			if (refMode == FbxLayerElement::eDirect) {
				// 直接取得（頂点）
				for (int j = 0; j < vertixSize; j++) {
					//座標と同様に
					m_Vertices[j].Normal.x = (float)normalElem->GetDirectArray().GetAt(j)[0];
					m_Vertices[j].Normal.y = (float)normalElem->GetDirectArray().GetAt(j)[1];
					m_Vertices[j].Normal.z = (float)normalElem->GetDirectArray().GetAt(j)[2];
				}
			}
		}
		else if (mappingMode == FbxLayerElement::eByControlPoint) {
			if (refMode == FbxLayerElement::eDirect) {
				//インデックスから取得（コントロールポイント）
				for (int j = 0; j < vertixSize; j++) {
					int index = normalElem->GetIndexArray().GetAt(j);
					m_Vertices[j].Normal.x = (float)normalElem->GetDirectArray().GetAt(index)[0];
					m_Vertices[j].Normal.y = (float)normalElem->GetDirectArray().GetAt(index)[1];
					m_Vertices[j].Normal.z = (float)normalElem->GetDirectArray().GetAt(index)[2];
				}
			}
		}
	}


	//***************UV取得(一種類だけ)***************************

	// uvsetの名前保存用
	FbxStringList uvset_names;
	// UVSetの名前リストを取得
	m_Mesh->GetUVSetNames(uvset_names);

	FbxArray<FbxVector2> uv_buffer;

	// テクスチャ取得時に保存したUVSetの名前からUVSetを取得する（テクスチャ取得出来ててここでuv取得できない場合大体モデルのuv参照ができてない）
	for (int i = 0; i < uvset_names.GetCount(); i++)
	{
		FbxString uvname = uvset_names.GetStringAt(i);

		if (uvname == m_TextureUvSetName)
		{
			m_Mesh->GetPolygonVertexUVs(uvset_names.GetStringAt(i), uv_buffer);

			int Size = uv_buffer.Size() < vertixSize ? uv_buffer.Size() : vertixSize;	//過剰参照防止用
			for (int j = 0;j < Size; j++) {
				FbxVector2& uv = uv_buffer[j];
				m_Vertices[j].TexCoord.x = uv[0];
				m_Vertices[j].TexCoord.y = 1.0f - uv[1];

			}
			break;
		}
	}

	//*******取得したデータをバッファに保存しておく******
	D3D11_MAPPED_SUBRESOURCE msr;
	CRenderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	memcpy_s(msr.pData, msr.RowPitch, (void*)(m_Vertices), sizeof(VERTEX_3D) * m_Mesh->GetPolygonVertexCount());

	CRenderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);


}