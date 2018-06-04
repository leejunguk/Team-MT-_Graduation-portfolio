#pragma once
#include "stdafx.h"
//#include "Mesh.h"
#include "Object.h"


// ��� ���۸� ���� ����ü
struct VS_CB_SKINNED
{
	XMMATRIX m_d3dxmtxBone[96];
};


struct BoneAnimationData
{
	int m_nFrameCount;
	float	*m_pfAniTime;
	XMFLOAT3 *m_pd3dxvScale;
	XMFLOAT3 *m_pd3dxvTranslate;
	XMFLOAT4 *m_pd3dxvQuaternion;
};

class CFBXMesh : public CMeshIlluminatedTextured
{
private:
	XMFLOAT3 *m_pd3dxvPositions;
	XMFLOAT3	*m_pd3dxvNormals;
	XMFLOAT2 *m_pd3dxvTexCoords;
	XMFLOAT4 *m_pd3dxvBoneWeights;
	XMFLOAT4 *m_pd3dxvBoneIndices;

	BoneAnimationData **m_ppBoneAnimationData;
	XMMATRIX *m_pd3dxmtxSQTTransform;
	XMFLOAT4X4 *m_pd3dxmtxFinalBone;

	int m_nBoneCount;
	int m_nAnimationClip;

	// i�� ������ �θ� ����(parentIndex)�� ��´�.
	// i�� ����� �ִϸ��̼� Ŭ���� i��° BoneAnimation �ν��Ͻ��� ����.
	UINT *m_pBoneHierarchy;
	XMFLOAT4X4 *m_pd3dxmtxBoneOffsets;

	ID3D12Resource *m_pd3dWeightBuffer;
	ID3D12Resource *m_pd3dBoneIndiceBuffer;
	// ���� �������
	ID3D12Resource *m_pd3dcbBones;

	float m_fFBXModelSize;		// ���� ������ (���� Animate���� �������ֱ� ����)
	float m_fFBXAnimationTime;	// ���� AnimationTime (���������� 0.0333333f �� ����)
	int m_nFBXAnimationNum;		// ���� ������ �ִϸ��̼��� ���� �����Ѵ�.
	int m_nFBXMaxFrameNum;		// ���� ������ �ִϸ��̼��� �ִ� ������ ��.
	int m_nFBXNowFrameNum;		// ���� �������� �ִϸ��̼��� ���� ������ ��.

public:
	CFBXMesh::CFBXMesh(ID3D12Device *pd3dDevice, char *pszFileName, float fSize, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual ~CFBXMesh();

	// �ش� �������� SR(Q)T ȸ���� �ݿ��� ����� ��ȯ
	void MakeBoneMatrix(int nNowframe, int nAnimationNum, int nBoneNum, XMMATRIX& BoneMatrix);
	void UpdateBoneTransform(ID3D12GraphicsCommandList *pd3dCommandList, int nAnimationNum, int nNowFrame);
	//dx12�� �°� ���� ��ģ��
	//�ٽ� ��ģ�� ,commandlist�� , pddevice �ʿ� 
	// ���� ��� ���� ����
	//void CreateConstantBuffer(ID3D12Device *pd3dDevice);

	float GetFBXModelSize() { return m_fFBXModelSize; }
	float GetFBXAnimationTime() { return m_fFBXAnimationTime; }
	int GetFBXAnimationNum() { return m_nFBXAnimationNum; }
	int GetFBXNowFrameNum() { return m_nFBXNowFrameNum; }
	int GetFBXMaxFrameNum() { return m_nFBXMaxFrameNum; }
	//void SetFBXAnimationTime(float fFBXAnimationTime) { m_fFBXAnimationTime = fFBXAnimationTime; }
	bool FBXFrameAdvance(float fTimeElapsed);
	void SetAnimation(int nFBXAnimationNum);
};

