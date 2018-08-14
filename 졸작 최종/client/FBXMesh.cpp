#include "stdafx.h"
#include "FBXMesh.h"

// �������� ��


CFBXMesh::CFBXMesh(ID3D12Device *pd3dDevice, char *pszFileName, float fSize , ID3D12GraphicsCommandList *pd3dCommandList) : CMeshIlluminatedTextured(pd3dDevice, pd3dCommandList)
{
	m_fFBXModelSize = fSize;
	m_fFBXAnimationTime = 0.0f;
	m_nFBXAnimationNum = 0;
	m_nFBXNowFrameNum = 0; 
	m_nBuffers = 0;
	ifstream fin;

	int filename_length = strlen(pszFileName);
	char* ch = new char[200];
	char* sToken = new char[50];
	char* temp;
	fin.open(pszFileName);
	if (!fin.fail())
	{
		// �����͸� �о�� �ʿ��� ����, �ε���, ��, �ִϸ��̼� �� �ľ�
		fin.getline(ch, 200);
		fin.getline(ch, 200);
		m_nVertices = stoi(ch);
		fin.getline(ch, 200);
		m_nIndices = stoi(ch);
		fin.getline(ch, 200);
		m_nBoneCount = stoi(ch);
		fin.getline(ch, 200);
		m_nAnimationClip = stoi(ch);

		// ���� �����͸� ����
		fin.getline(ch, 200);
		m_pd3dxvPositions = new XMFLOAT3[m_nVertices];
		m_pd3dxvNormals = new XMFLOAT3[m_nVertices];
		m_pd3dxvTexCoords = new XMFLOAT2[m_nVertices];
		if (m_nBoneCount)
		{
			m_pd3dxvBoneIndices = new XMFLOAT4[m_nVertices];
			m_pd3dxvBoneWeights = new XMFLOAT4[m_nVertices];
		}

		for (int i = 0; i < m_nVertices; i++)
		{
			// Vertice
			fin.getline(ch, 200);
			sToken = strtok_s(ch, " ", &temp);	 m_pd3dxvPositions[i].x = stof(sToken);
			sToken = strtok_s(NULL, " ", &temp); m_pd3dxvPositions[i].y = stof(sToken);
			sToken = strtok_s(NULL, " ", &temp); m_pd3dxvPositions[i].z = stof(sToken);
			// Normal
			fin.getline(ch, 200);
			sToken = strtok_s(ch, " ", &temp);   m_pd3dxvNormals[i].x = stof(sToken);
			sToken = strtok_s(NULL, " ", &temp); m_pd3dxvNormals[i].y = stof(sToken);
			sToken = strtok_s(NULL, " ", &temp); m_pd3dxvNormals[i].z = stof(sToken);
			// UV
			fin.getline(ch, 200);
			sToken = strtok_s(ch, " ", &temp);   m_pd3dxvTexCoords[i].x = stof(sToken);
			sToken = strtok_s(NULL, " ", &temp); m_pd3dxvTexCoords[i].y = stof(sToken);
			if (m_nBoneCount)
			{			// BoneIndice
				fin.getline(ch, 200);
				sToken = strtok_s(ch, " ", &temp); 	 m_pd3dxvBoneIndices[i].x = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxvBoneIndices[i].y = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxvBoneIndices[i].z = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxvBoneIndices[i].w = stof(sToken);
				// BoneWeight
				fin.getline(ch, 200);
				sToken = strtok_s(ch, " ", &temp);   m_pd3dxvBoneWeights[i].x = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxvBoneWeights[i].y = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxvBoneWeights[i].z = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxvBoneWeights[i].w = stof(sToken);
			}
		}

		// �ε��� ������ ����
		fin.getline(ch, 200);
		m_pnIndices = new UINT[m_nIndices];  // �޽�������. 

		for (int i = 0; i < m_nIndices; i += 3)
		{
			fin.getline(ch, 200);
			sToken = strtok_s(ch, " ", &temp);   m_pnIndices[i] = stoi(sToken);
			sToken = strtok_s(NULL, " ", &temp); m_pnIndices[i + 1] = stoi(sToken);
			sToken = strtok_s(NULL, " ", &temp); m_pnIndices[i + 2] = stoi(sToken);
		}

		// (�ִϸ��̼��� ������ �޽��� ���) �� ������ �ִϸ��̼� ���� ����
		if (m_nBoneCount)
		{
			fin.getline(ch, 200);
			m_pBoneHierarchy = new UINT[m_nBoneCount];
			m_pd3dxmtxBoneOffsets = new XMFLOAT4X4[m_nBoneCount];
			m_pd3dxmtxSQTTransform = new XMMATRIX[m_nBoneCount];
			m_pd3dxmtxFinalBone = new XMFLOAT4X4[m_nBoneCount];

			// �θ� ���븦 ����Ű�� BoneHierarchy�� ����
			for (int i = 0; i < m_nBoneCount; i++)
			{
				fin.getline(ch, 200);
				m_pBoneHierarchy[i] = stoi(ch);
			}
			// ���� ��ü�� ������ ����� ����
			fin.getline(ch, 200);
			for (int i = 0; i < m_nBoneCount; i++)
			{
				fin.getline(ch, 200);
				sToken = strtok_s(ch, " ", &temp);   m_pd3dxmtxBoneOffsets[i]._11 = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxmtxBoneOffsets[i]._12 = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxmtxBoneOffsets[i]._13 = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxmtxBoneOffsets[i]._14 = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxmtxBoneOffsets[i]._21 = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxmtxBoneOffsets[i]._22 = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxmtxBoneOffsets[i]._23 = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxmtxBoneOffsets[i]._24 = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxmtxBoneOffsets[i]._31 = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxmtxBoneOffsets[i]._32 = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxmtxBoneOffsets[i]._33 = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxmtxBoneOffsets[i]._34 = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxmtxBoneOffsets[i]._41 = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxmtxBoneOffsets[i]._42 = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxmtxBoneOffsets[i]._43 = stof(sToken);
				sToken = strtok_s(NULL, " ", &temp); m_pd3dxmtxBoneOffsets[i]._44 = stof(sToken);
			}

			// ���⿡������ �ִϸ��̼��� ��´�.
			m_ppBoneAnimationData = new BoneAnimationData*[m_nAnimationClip];

			BoneAnimationData *pBoneAnimationData;
			for (int k = 0; k < m_nAnimationClip; k++)
			{
				pBoneAnimationData = new BoneAnimationData[m_nBoneCount];

				fin.getline(ch, 200);
				for (int i = 0; i < m_nBoneCount; i++)
				{
					fin.getline(ch, 200);
					sToken = strtok_s(ch, " ", &temp);
					sToken = strtok_s(NULL, " ", &temp);
					pBoneAnimationData[i].m_nFrameCount = stoi(sToken);
					pBoneAnimationData[i].m_pd3dxvTranslate = new XMFLOAT3[pBoneAnimationData[i].m_nFrameCount];
					pBoneAnimationData[i].m_pd3dxvScale = new XMFLOAT3[pBoneAnimationData[i].m_nFrameCount];
					pBoneAnimationData[i].m_pd3dxvQuaternion = new XMFLOAT4[pBoneAnimationData[i].m_nFrameCount];
					pBoneAnimationData[i].m_pfAniTime = new float[pBoneAnimationData[i].m_nFrameCount];

					for (int j = 0; j < pBoneAnimationData[i].m_nFrameCount; j++)
					{
						fin.getline(ch, 200);
						sToken = strtok_s(ch, " ", &temp);
						pBoneAnimationData[i].m_pfAniTime[j] = stof(sToken);
						sToken = strtok_s(NULL, " ", &temp);
						pBoneAnimationData[i].m_pd3dxvTranslate[j].x = stof(sToken);
						sToken = strtok_s(NULL, " ", &temp);
						pBoneAnimationData[i].m_pd3dxvTranslate[j].y = stof(sToken);
						sToken = strtok_s(NULL, " ", &temp);
						pBoneAnimationData[i].m_pd3dxvTranslate[j].z = stof(sToken);
						sToken = strtok_s(NULL, " ", &temp);
						pBoneAnimationData[i].m_pd3dxvScale[j].x = stof(sToken);
						sToken = strtok_s(NULL, " ", &temp);
						pBoneAnimationData[i].m_pd3dxvScale[j].y = stof(sToken);
						sToken = strtok_s(NULL, " ", &temp);
						pBoneAnimationData[i].m_pd3dxvScale[j].z = stof(sToken);
						sToken = strtok_s(NULL, " ", &temp);
						pBoneAnimationData[i].m_pd3dxvQuaternion[j].x = stof(sToken);
						sToken = strtok_s(NULL, " ", &temp);
						pBoneAnimationData[i].m_pd3dxvQuaternion[j].y = stof(sToken);
						sToken = strtok_s(NULL, " ", &temp);
						pBoneAnimationData[i].m_pd3dxvQuaternion[j].z = stof(sToken);
						sToken = strtok_s(NULL, " ", &temp);
						pBoneAnimationData[i].m_pd3dxvQuaternion[j].w = stof(sToken);
					}
				}
				m_ppBoneAnimationData[k] = pBoneAnimationData;
			}
		}
	}
	fin.close();

	if (0 != m_nBoneCount)
	{
		m_nStride = sizeof(CSceletonTexturedVertex); //�����ڵ� 
		CSceletonTexturedVertex *pVertices = new CSceletonTexturedVertex[m_nVertices];
		for (UINT i = 0; i < m_nVertices; i++) pVertices[i] = CSceletonTexturedVertex(m_pd3dxvPositions[i], m_pd3dxvNormals[i], m_pd3dxvTexCoords[i], m_pd3dxvBoneWeights[i], m_pd3dxvBoneIndices[i]);
		m_pd3dVertexBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, pVertices, m_nStride * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dVertexUploadBuffer);

		m_d3dVertexBufferView.BufferLocation = m_pd3dVertexBuffer->GetGPUVirtualAddress();
		m_d3dVertexBufferView.StrideInBytes = m_nStride;
		m_d3dVertexBufferView.SizeInBytes = m_nStride * m_nVertices;

		if (m_nIndices > 0)
		{
			m_nIndices = m_nIndices;

			m_pd3dIndexBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, m_pnIndices, sizeof(UINT) * m_nIndices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_INDEX_BUFFER, &m_pd3dIndexUploadBuffer);

			m_d3dIndexBufferView.BufferLocation = m_pd3dIndexBuffer->GetGPUVirtualAddress();
			m_d3dIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
			m_d3dIndexBufferView.SizeInBytes = sizeof(UINT) * m_nIndices;
		}
	}
	else
	{
		// 0512 �߰� .. �ִϸ��̼� ���� �޽��ϰ�� 
		m_nStride = sizeof(CIlluminatedTexturedVertex); //�����ڵ� 
		CIlluminatedTexturedVertex *pVertices = new CIlluminatedTexturedVertex[m_nVertices];
		for (UINT i = 0; i < m_nVertices; i++) pVertices[i] = CIlluminatedTexturedVertex(m_pd3dxvPositions[i], m_pd3dxvNormals[i], m_pd3dxvTexCoords[i]);
		m_pd3dVertexBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, pVertices, m_nStride * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dVertexUploadBuffer);

		m_d3dVertexBufferView.BufferLocation = m_pd3dVertexBuffer->GetGPUVirtualAddress();
		m_d3dVertexBufferView.StrideInBytes = m_nStride;
		m_d3dVertexBufferView.SizeInBytes = m_nStride * m_nVertices;

		if (m_nIndices > 0)
		{
			m_nIndices = m_nIndices;

			m_pd3dIndexBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, m_pnIndices, sizeof(UINT) * m_nIndices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_INDEX_BUFFER, &m_pd3dIndexUploadBuffer);

			m_d3dIndexBufferView.BufferLocation = m_pd3dIndexBuffer->GetGPUVirtualAddress();
			m_d3dIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
			m_d3dIndexBufferView.SizeInBytes = sizeof(UINT) * m_nIndices;
		}
	}


	//--------------------------------------------------------------------------------

	// Dx11 �ڵ� --------------------------------------------------------------------------------
	//m_nStride = sizeof(CVertex);

	//UINT ncbXMFLOAT3Bytes = ((sizeof(XMFLOAT3) + 255) & ~255); //256�� ���
	//UINT ncbXMFLOAT2Bytes = ((sizeof(XMFLOAT2) + 255) & ~255);
	//UINT ncbXMFLOAT4Bytes = ((sizeof(XMFLOAT4) + 255) & ~255);
	////m_pd3dcbLights = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	////m_pd3dcbLights->Map(0, NULL, (void **)&m_pcbMappedLights);

	//m_pd3dPositionBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, m_pd3dxvPositions, ncbXMFLOAT3Bytes, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dPositionUploadBuffer);
	//m_pd3dNormalBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, m_pd3dxvNormals, ncbXMFLOAT3Bytes, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dNormalUploadBuffer);
	//m_pd3dTexCoordBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, m_pd3dxvTexCoords, ncbXMFLOAT2Bytes, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dTexCoordUploadBuffer);
	//m_pd3dWeightBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, m_pd3dxvBoneWeights, ncbXMFLOAT4Bytes, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dWeightUploadBuffer);
	//m_pd3dBoneIndiceBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, m_pd3dxvBoneIndices, ncbXMFLOAT4Bytes, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dBoneIndiceUploadBuffer);
	//ID3D12Resource *pd3dBuffers[5] = { m_pd3dPositionBuffer, m_pd3dNormalBuffer, m_pd3dTexCoordBuffer, m_pd3dWeightBuffer, m_pd3dBoneIndiceBuffer };
	//UINT pnBufferStrides[5] = { sizeof(XMFLOAT3), sizeof(XMFLOAT3), sizeof(XMFLOAT2), sizeof(XMFLOAT4), sizeof(XMFLOAT4) };
	//UINT pnBufferOffsets[5] = { 0, 0, 0, 0, 0 };
	//AssembleToVertexBuffer(5, pd3dBuffers, pnBufferStrides, pnBufferOffsets);  //  04.08 ���ؽ� ���� ��ġ�� �ǰ����� 
																			   //11 ���� �ڵ��ε�.

	
	//m_pd3dVertexBuffer = ::CreateBufferResource(pd3dDevice, pd3dCommandList, pVertices, m_nStride * m_nVertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dVertexUploadBuffer);

	//m_d3dVertexBufferView.BufferLocation = m_pd3dVertexBuffer->GetGPUVirtualAddress();
	//m_d3dVertexBufferView.StrideInBytes = m_nStride;
	//m_d3dVertexBufferView.SizeInBytes = m_nStride * m_nVertices;

	//�ε������ۿ� �ε��� ���ε� ���۴� ���� DX12 �ڵ忡�� �������Ƿ� ȥ�� �� �浹�� ������ ����. �����ؼ� ����.
	// ���ʿ��� �浹�� ����� ������ �ּ��ϰ� ���ο� ���� �Ҵ��ϴ� �͵� ���� �� ���� 04.09
	//m_pd3dIndexBuffer = CreateBufferResource(pd3dDevice, pd3dCommandList, m_pnIndices, m_nIndices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_INDEX_BUFFER, &m_pd3dIndexUploadBuffer);

	//CreateRasterizerState(pd3dDevice); //�����Ͷ����� ���¸� ����µ� DX11���� �ִµ� DX12 ���� �߸𸣰���? �ϴ� �ּ�
	//CreateConstantBuffer(pd3dDevice);
	UINT ncbElementBytes = ((sizeof(VS_CB_SKINNED) + 255) & ~255);
	m_pd3dcbBones = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	// idle ������ ������ ���� üũ.
	if (m_nBoneCount)
		m_nFBXMaxFrameNum = m_ppBoneAnimationData[0][0].m_nFrameCount - 1;
}


CFBXMesh::~CFBXMesh()
{
//	if (m_pd3dWeightBuffer) m_pd3dWeightBuffer->Release();
	//if (m_pd3dBoneIndiceBuffer) m_pd3dBoneIndiceBuffer->Release();
	if (m_pd3dcbBones) m_pd3dcbBones->Release();

	if (m_pd3dxvPositions) delete[] m_pd3dxvPositions;
	if (m_pd3dxvNormals) delete[] m_pd3dxvNormals;
	if (m_pd3dxvTexCoords) delete[] m_pd3dxvTexCoords;
//	if (m_pd3dxvBoneWeights) delete[] m_pd3dxvBoneWeights;
//	if (m_pd3dxvBoneIndices) delete[] m_pd3dxvBoneIndices;
//	if (m_ppBoneAnimationData) delete[] m_ppBoneAnimationData;
//	if (m_pd3dxmtxFinalBone) delete[] m_pd3dxmtxFinalBone;
//	if (m_pd3dxmtxSQTTransform) delete[] m_pd3dxmtxSQTTransform;
//	if (m_pBoneHierarchy) delete[] m_pBoneHierarchy;
//	if (m_pd3dxmtxBoneOffsets) delete[] m_pd3dxmtxBoneOffsets;

}


void CFBXMesh::UpdateBoneTransform(ID3D12GraphicsCommandList *pd3dCommandList, int nAnimationNum, int nNowFrame)
{
	for (int i = 0; i < m_nBoneCount; i++)
	{
		
		MakeBoneMatrix(nNowFrame, nAnimationNum, i, *(m_pd3dxmtxSQTTransform + i));
	}


	// ���������� ���� �⺻ ����������� �����־� ���� ����� ������ش�.
	for (int i = 0; i < m_nBoneCount; i++)
	{
		XMFLOAT4X4 offset = m_pd3dxmtxBoneOffsets[i];
		XMFLOAT4X4 toRoot;

		// 12 �� ��ȯ
		XMStoreFloat4x4(&toRoot, m_pd3dxmtxSQTTransform[i]);
		m_pd3dxmtxFinalBone[i] = Matrix4x4::Multiply(offset, toRoot);

	}

	VS_CB_SKINNED* m_pcbMappedBones;
	m_pd3dcbBones->Map(0, NULL, (void **)&m_pcbMappedBones);


	for (int i = 0; i < m_nBoneCount; i++)
	{
		XMMATRIX temp = XMLoadFloat4x4(&m_pd3dxmtxFinalBone[i]);
		m_pcbMappedBones->m_d3dxmtxBone[i] = XMMatrixTranspose(temp);
	}
	m_pd3dcbBones->Unmap(0, NULL);

	D3D12_GPU_VIRTUAL_ADDRESS d3dcbBoneGpuVirtualAddress = m_pd3dcbBones->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(9, d3dcbBoneGpuVirtualAddress);
}

void CFBXMesh::MakeBoneMatrix(int nNowframe, int nAnimationNum, int nBoneNum, XMMATRIX& BoneMatrix)
{
	// XMAffine �Լ������� scale�� VECTOR3�� ������
	// D3DXAffine �Լ������� scale�� ����� ����Ѵ�.

	if (m_ppBoneAnimationData[nAnimationNum][nBoneNum].m_nFrameCount != 0)
	{

		//if (nNowframe >=0) //nNowframe = 0;
		//{
			XMFLOAT3 fScale = m_ppBoneAnimationData[nAnimationNum][nBoneNum].m_pd3dxvScale[nNowframe];
			XMVECTOR fScal = XMLoadFloat3(&fScale);


			XMFLOAT3 d3dxvTranslate = m_ppBoneAnimationData[nAnimationNum][nBoneNum].m_pd3dxvTranslate[nNowframe];
			XMVECTOR d3dxvTrans = XMLoadFloat3(&d3dxvTranslate);

			XMFLOAT4 d3dxvQuaternion = m_ppBoneAnimationData[nAnimationNum][nBoneNum].m_pd3dxvQuaternion[nNowframe];
			XMVECTOR d3dxvQuater = XMLoadFloat4(&d3dxvQuaternion);

			XMVECTOR d3dxvZero = { 0.0f, 0.0f, 0.0f };
			//D3DXMatrixAffineTransformation(&BoneMatrix, fScale, &d3dxvZero, &d3dxvQuaternion, &d3dxvTranslate);
			BoneMatrix = XMMatrixAffineTransformation(fScal, d3dxvZero, d3dxvQuater, d3dxvTrans);
		//}
		
	}
	else // �ش� ���� �ִϸ��̼� �������� �ƿ� ���� ��� ��������� ��������.
	{
		//D3DXMatrixIdentity(&BoneMatrix);
		BoneMatrix = XMMatrixIdentity();
	}
}



// ���� �ִϸ��̼��� ���� ���������� �ѱ��.
// �߰����� �ִϸ��̼� ������ ���� ������ �������� ��� true�� �����Ѵ�.
bool CFBXMesh::FBXFrameAdvance(float fTimeElapsed)
{
	m_fFBXAnimationTime += fTimeElapsed;

	if (m_fFBXAnimationTime > ANIFRAMETIME)	// 0.0333333f�ʰ� ������ 1������ �ø���.
	{
		if (m_nFBXNowFrameNum < m_nFBXMaxFrameNum - 1)
		{
			m_nFBXNowFrameNum++;
			m_fFBXAnimationTime = 0.0f;
			return false;
		}
		else
		{
			m_nFBXNowFrameNum = 0;
			m_fFBXAnimationTime = 0.0f;
			return true;
		}
	}
	else
		return false;
}

void CFBXMesh::SetAnimation(int nFBXAnimation)
{
	m_nFBXAnimationNum = nFBXAnimation;
	m_nFBXNowFrameNum = 0;
	m_nFBXMaxFrameNum = m_ppBoneAnimationData[nFBXAnimation][0].m_nFrameCount;
	m_fFBXAnimationTime = 0.0f;
}