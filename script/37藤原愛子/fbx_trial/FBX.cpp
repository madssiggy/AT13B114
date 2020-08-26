/*
	   �^�_�Q�Q�Q�Q�Q�Q�Q�Q�Q�Q_�Q�^�_
�@�@ �^�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ �_
�@�@�b�@FBX���f���ǂݍ���  [fbx.cpp]   �b
�@�@�b�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ �b
�@�@�b�@�@�@date	: 2020/08/26   �@  �b
�@�@�b�@�@�@auther	: Aiko Fujiwara�@  �b
�@�@ �_�Q�Q�Q�Q�Q�Q�Q�Q�Q�Q�Q�Q�Q�Q�Q_�^
 */

//��VS�Œǉ��̈ˑ��t�@�C���ݒ肵�Ȃ��l�p
#pragma comment (lib, "libfbxsdk-mt.lib")
#pragma comment (lib, "libxml2-mt.lib")
#pragma comment (lib, "zlib-mt.lib")


//�e�N�X�`�����Ȃ��������p�̃e�N�X�`���A�p�ӂ���K�v������܂�
#define NONE_TEXTURE_PATH ("asset\\texture\\white.png")
//���A�j���[�V�����i���o�[�i�����ŃZ�b�g�����ԍ��̃A�j���[�V�������Đ��A���p�I����Ȃ��̂ŃJ�X�^�}�C�Y�v�j
#define ANIM_NUMBER (0)


#include "main.h"
#include "renderer.h"
#include "FBX.h"


void CModelFBX::Load(const char *FileName)
{

	m_FileName = FileName;


	//****************FBX�̓ǂݍ���************************
	m_FbxManager = FbxManager::Create();
	assert(m_FbxManager);

	m_FbxScene = FbxScene::Create(m_FbxManager, "fbxscene");
	assert(m_FbxScene);

	FbxImporter *fbxImporter = FbxImporter::Create(m_FbxManager, "imp");
	assert(fbxImporter);

	//�t�@�C���ǂݍ���
	assert(fbxImporter->Initialize(FileName, -1, m_FbxManager->GetIOSettings()));
	fbxImporter->Import(m_FbxScene);

	fbxImporter->Destroy();

	
	FbxGeometryConverter converter(m_FbxManager);
	// �|���S�����O�p�`�ɂ���
	converter.Triangulate(m_FbxScene, true);

	
	//********�A�j���[�V�����ǂݍ���****************************

	m_Anim = true;
	m_AnimStackNumber = ANIM_NUMBER;

	//�A�j���[�V�������ꗗ�擾
	FbxArray<FbxString*> AnimStackNameArray;
	m_FbxScene->FillAnimStackNameArray(AnimStackNameArray);

	int animation_number_count = m_FbxScene->GetSrcObjectCount<FbxAnimStack>();
	if (animation_number_count > 0 && m_AnimStackNumber < animation_number_count)
	{//�A�j���[�V��������

		//�b��I��AnimStackNumber�Ԗڂ̃A�j���[�V�����݂̂��擾
		FbxAnimStack *AnimationStack = m_FbxScene->FindMember<FbxAnimStack>(AnimStackNameArray[m_AnimStackNumber]->Buffer());
		//���݂̃A�j���[�V�����ɐݒ�
		m_FbxScene->SetCurrentAnimationStack(AnimationStack);

		//�^�C���̐ݒ�
		FbxTakeInfo *takeInfo = m_FbxScene->GetTakeInfo(*(AnimStackNameArray[m_AnimStackNumber]));
		m_StartTime = takeInfo->mLocalTimeSpan.GetStart();
		m_StopTime = takeInfo->mLocalTimeSpan.GetStop();
		//�P�t���[���̎��Ԃ�ݒ�
		m_FrameTime.SetTime(0, 0, 0, 1, 0, FbxTime::eFrames60);
		//FrameTime.SetTime(0, 0, 0, 1, 0, m_FbxScene->GetGlobalSettings().GetTimeMode());
		m_TimeCount = m_StartTime;

		// 1�t���[�����ԂŊ���΃t���[�����ɂȂ�܂�
		//int startFrame = (int)(m_StartTime.Get() / m_FrameTime.Get());
		//int stopFrame = (int)(m_StartTime.Get() / m_FrameTime.Get());

	}
	else
	{
		m_Anim = false;
	}
	

	//*****************���b�V���쐬************************************
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
		//�A�j���[�V�����X�V
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
	//�m�[�h���̃��b�V���̎��o��
	for (int i = 0; i < node->GetNodeAttributeCount(); i++)
	{
		if (node->GetNodeAttributeByIndex(i)->GetAttributeType() == FbxNodeAttribute::eMesh) {

			CMesh* mesh = new CMesh(node, m_Anim);
			mesh->Load(m_FileName);
			m_Mesh.push_back(mesh);
		}
	}

	//�q�m�[�h�����ׂ�
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

	//�g���ĂȂ�
	/*
	// �C���f�b�N�X�o�b�t�@����
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
	
	//*************�A�j���[�V��������*****************
	if (m_Anim)
	{

		//�}�g���N�X�쐬
		FbxMatrix globalPosition = m_MeshNode->EvaluateGlobalTransform(timeCount);
		FbxVector4 t0 = m_MeshNode->GetGeometricTranslation(FbxNode::eSourcePivot);
		FbxVector4 r0 = m_MeshNode->GetGeometricRotation(FbxNode::eSourcePivot);
		FbxVector4 s0 = m_MeshNode->GetGeometricScaling(FbxNode::eSourcePivot);
		FbxAMatrix geometryOffset = FbxAMatrix(t0, r0, s0);


		//�R���g���[���|�C���g���}�g���N�X�쐬
		FbxMatrix *clusterDeformation = new FbxMatrix[m_Mesh->GetControlPointsCount()];
		memset(clusterDeformation, 0, sizeof(FbxMatrix) * m_Mesh->GetControlPointsCount());

		FbxSkin *skinDeformer = (FbxSkin *)m_Mesh->GetDeformer(0, FbxDeformer::eSkin);
		if (skinDeformer != nullptr)
		{
			int clusterCount = skinDeformer->GetClusterCount();

			for (int clusterIndex = 0; clusterIndex < clusterCount; clusterIndex++) {
				// �N���X�^(�{�[��)�̎��o���i�悭�킩���ĂȂ��j
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


			//�C���f�b�N�X�f�[�^�擾
			int* indices = m_Mesh->GetPolygonVertices();

			for (int i = 0; i < m_Mesh->GetPolygonVertexCount(); i++)
			{//�C���f�b�N�X����R���g���[���|�C���g���Ăяo���đS�C���f�b�N�X�������
				int index = indices[i];
				FbxVector4 outVertex = clusterDeformation[index].MultNormalize(m_Mesh->GetControlPointAt(index));
				m_Vertices[i].Position.x = (FLOAT)outVertex[0];
				m_Vertices[i].Position.y = (FLOAT)outVertex[1];
				m_Vertices[i].Position.z = (FLOAT)outVertex[2];

			}
			delete[] clusterDeformation;


			//�f�[�^���o�b�t�@�ɓ��꒼��
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



	//**************�`��***************************

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// �C���f�b�N�X�o�b�t�@�ݒ�
	//CRenderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// �v���~�e�B�u�g�|���W�ݒ�
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// �}�e���A���ݒ�
	CRenderer::SetMaterial(m_Material);

	// �e�N�X�`���ݒ�
	CRenderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// �|���S���`��
	//CRenderer::GetDeviceContext()->DrawIndexed(m_Mesh->GetPolygonVertexCount(), 0, 0);
	CRenderer::GetDeviceContext()->Draw(m_Mesh->GetPolygonVertexCount(), 0);
}



void CMesh::LoadMaterial(const char *FileName)
{
	//���b�V�����ƂɃ}�e���A����ǂݍ���
	//�i�}�e���A�����ɓǂݍ��񂾌�Ɋ֘A�t�����ق����悳���j

	bool texture_none = true;

	if (m_Mesh->GetElementMaterialCount() > 0)
	{//���b�V���Ƀ}�e���A���L

		//********1���b�V��1�}�e���A���݂̂̑Ή��A�ŏ��̃}�e���A����ǂݍ��ށi�}���`�}�e���A���̏����s�� ���� �j

		// Mesh���̃}�e���A�������擾
		FbxLayerElementMaterial* material = m_Mesh->GetElementMaterial(0);

		// FbxSurfaceMaterial�̃C���f�b�N�X�o�b�t�@����C���f�b�N�X���擾
		int	index = material->GetIndexArray().GetAt(0);
		
		// GetSrcObject<FbxSurfaceMaterial>�Ń}�e���A���擾
		FbxSurfaceMaterial* surface_material = m_MeshNode->GetSrcObject<FbxSurfaceMaterial>(index);

		if (surface_material != nullptr)
		{
			//FbxSurfaceMaterial�͓��ނ���A�������ĂȂ��̂�Lambert�̕�����Specular�擾�ł��Ȃ�
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


			//******�e�N�X�`���ǂݍ��݁i������}�e���A�����Ƃɓǂݍ���ł邩��{���͑S���ǂݍ��񂾌�Ɋ֘A�t�����ق����悳���j
			
			//�e�N�X�`����FbxFileTexture��FbxLayeredTexture�̂ǂ��炩�BLayered�̏ꍇ�����e�N�X�`��
			int TextureNum = prop.GetSrcObjectCount<FbxFileTexture>();

			if (TextureNum > 0)
			{//�ʏ�e�N�X�`��
				for (int i = 0; i < TextureNum; i++)
				{
					//prop����FbxFileTexture���擾
					FbxFileTexture* texture = prop.GetSrcObject<FbxFileTexture>(i);
					// �e�N�X�`���t�@�C���p�X���擾
					std::string Path = texture->GetRelativeFileName();

					Path = (std::string)FileName + "\\..\\" + Path;

					//�e�N�X�`���ǂݍ���
					D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
						Path.c_str(),
						NULL,
						NULL,
						&m_Texture,
						NULL
					);

					//uv�擾�p
					m_TextureUvSetName = texture->UVSet;


					if (m_Texture != nullptr)
					{//�擾����
						texture_none = false;
					}

					break;	//��������1����Ȃ���������̂���
				}

			}
			else
			{
				//�����e�N�X�`�����ǂ������ׂ�
				int layeredTextureCount = prop.GetSrcObjectCount<FbxLayeredTexture>();

				if (layeredTextureCount > 0)
				{//�����e�N�X�`��
					for (int j = 0; j < layeredTextureCount; j++) 
					{
						//�ЂƂÂʏ�e�N�X�`�����擾���ēǂݍ���
						FbxLayeredTexture* layeredTexture = prop.GetSrcObject<FbxLayeredTexture>(j);
						int textureCount = layeredTexture->GetSrcObjectCount<FbxFileTexture>();

						for (int k = 0; k < textureCount; k++) {
							FbxFileTexture* texture = prop.GetSrcObject<FbxFileTexture>(k);

							if (texture) {
								// �e�N�X�`���t�@�C���p�X���擾
								std::string Path = texture->GetRelativeFileName();

								Path = (std::string)FileName + "\\..\\" + Path;

								//�e�N�X�`���ǂݍ���
								D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
									Path.c_str(),
									NULL,
									NULL,
									&m_Texture,
									NULL
								);


								m_TextureUvSetName = texture->UVSet;

								
								if (m_Texture != nullptr)
								{//�擾����
									texture_none = false;
								}

								break;	//��������1����Ȃ���������̂���

							}
						}
						break;	//�{���S���e�N�X�`���ǂݍ���ŉ�������͂������ǁA�Ƃ肠�����g���ĂȂ�����ꖇ������break
					}
				}
				
			}
		}

	}

	if (texture_none)
	{
		//�e�N�X�`�����Ȃ����p�ɔ��e�N�X�`���ǂݍ���
		D3DX11CreateShaderResourceViewFromFile(CRenderer::GetDevice(),
			NONE_TEXTURE_PATH,
			NULL,
			NULL,
			&m_Texture,
			NULL
		);

		//�p�X�̏ꏊ�ɂȂ����̓G���[�f��
		assert(m_Texture);
	}
}


void CMesh::LoadVertexData()
{

	//***********���_�f�[�^�쐬*******************
	int vertixSize = m_Mesh->GetPolygonVertexCount();
	m_Vertices = new VERTEX_3D[vertixSize];


	{
		// *********���_�o�b�t�@�����@�@�i�A�j���[�V�������邩�ǂ����Ńo�b�t�@�̍����ς����ق����ǂ������j
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * vertixSize;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		CRenderer::GetDevice()->CreateBuffer(&bd, NULL, &m_VertexBuffer);
	}


	int* indices = m_Mesh->GetPolygonVertices();

	//************���W�擾********************
	for (int i = 0; i < vertixSize; i++)
	{//�C���f�b�N�X����R���g���[���|�C���g���Ăяo���đS�C���f�b�N�X�������
		int index = indices[i];
		m_Vertices[i].Position.x = (FLOAT)m_Mesh->GetControlPointAt(index)[0];
		m_Vertices[i].Position.y = (FLOAT)m_Mesh->GetControlPointAt(index)[1];
		m_Vertices[i].Position.z = (FLOAT)m_Mesh->GetControlPointAt(index)[2];

		//�Ƃ肠������(�J���[�擾�ł���͂�)
		m_Vertices[i].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	}


	//***********�@���擾**********************

	int layerNum = m_Mesh->GetLayerCount();
	for (int i = 0; i < layerNum; i++) {
		FbxLayer* layer = m_Mesh->GetLayer(i);
		FbxLayerElementNormal* normalElem = layer->GetNormals();
		if (normalElem == 0) {
			continue;   // �@������
		}

		// �}�b�s���O���[�h�E���t�@�����X���[�h�擾
		FbxLayerElement::EMappingMode mappingMode = normalElem->GetMappingMode();
		FbxLayerElement::EReferenceMode refMode = normalElem->GetReferenceMode();

		if (mappingMode == FbxLayerElement::eByPolygonVertex) {
			if (refMode == FbxLayerElement::eDirect) {
				// ���ڎ擾�i���_�j
				for (int j = 0; j < vertixSize; j++) {
					//���W�Ɠ��l��
					m_Vertices[j].Normal.x = (float)normalElem->GetDirectArray().GetAt(j)[0];
					m_Vertices[j].Normal.y = (float)normalElem->GetDirectArray().GetAt(j)[1];
					m_Vertices[j].Normal.z = (float)normalElem->GetDirectArray().GetAt(j)[2];
				}
			}
		}
		else if (mappingMode == FbxLayerElement::eByControlPoint) {
			if (refMode == FbxLayerElement::eDirect) {
				//�C���f�b�N�X����擾�i�R���g���[���|�C���g�j
				for (int j = 0; j < vertixSize; j++) {
					int index = normalElem->GetIndexArray().GetAt(j);
					m_Vertices[j].Normal.x = (float)normalElem->GetDirectArray().GetAt(index)[0];
					m_Vertices[j].Normal.y = (float)normalElem->GetDirectArray().GetAt(index)[1];
					m_Vertices[j].Normal.z = (float)normalElem->GetDirectArray().GetAt(index)[2];
				}
			}
		}
	}


	//***************UV�擾(���ނ���)***************************

	// uvset�̖��O�ۑ��p
	FbxStringList uvset_names;
	// UVSet�̖��O���X�g���擾
	m_Mesh->GetUVSetNames(uvset_names);

	FbxArray<FbxVector2> uv_buffer;

	// �e�N�X�`���擾���ɕۑ�����UVSet�̖��O����UVSet���擾����i�e�N�X�`���擾�o���ĂĂ�����uv�擾�ł��Ȃ��ꍇ��̃��f����uv�Q�Ƃ��ł��ĂȂ��j
	for (int i = 0; i < uvset_names.GetCount(); i++)
	{
		FbxString uvname = uvset_names.GetStringAt(i);

		if (uvname == m_TextureUvSetName)
		{
			m_Mesh->GetPolygonVertexUVs(uvset_names.GetStringAt(i), uv_buffer);

			int Size = uv_buffer.Size() < vertixSize ? uv_buffer.Size() : vertixSize;	//�ߏ�Q�Ɩh�~�p
			for (int j = 0;j < Size; j++) {
				FbxVector2& uv = uv_buffer[j];
				m_Vertices[j].TexCoord.x = uv[0];
				m_Vertices[j].TexCoord.y = 1.0f - uv[1];

			}
			break;
		}
	}

	//*******�擾�����f�[�^���o�b�t�@�ɕۑ����Ă���******
	D3D11_MAPPED_SUBRESOURCE msr;
	CRenderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	memcpy_s(msr.pData, msr.RowPitch, (void*)(m_Vertices), sizeof(VERTEX_3D) * m_Mesh->GetPolygonVertexCount());

	CRenderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);


}