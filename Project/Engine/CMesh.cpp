#include "pch.h"
#include "CMesh.h"
#include "CDevice.h"
#include "CStructuredBuffer.h"
#pragma warning (disable:6387)

CMesh::CMesh() :
	CResource(E_ResourceType::Mesh),
	m_pVB(nullptr),
	m_tVtxDesc{},
	m_iVtxCount{ 0 },
	m_pBoneFrameData{ nullptr },
	m_pBoneOffset{ nullptr }
{


}

CMesh::~CMesh()
{
	SAFE_DELETE(m_pVtxSys);

	for (size_t i = 0; i < m_vecIdxInfo.size(); ++i) {
		SAFE_DELETE(m_vecIdxInfo[i].pIdxSysMem);
	}

	// Animation3D Info
	SAFE_DELETE(m_pBoneFrameData);
	SAFE_DELETE(m_pBoneOffset);
}

void CMesh::Create(void* _pVtxSys, UINT _iVtxBufferSize, void* _pIdxSys, UINT _iIdxBufferSize, D3D11_USAGE _eIdxUsage)
{

	////////////////////
	// ���ؽ� ���� �����
	////////////////////
	m_iVtxCount = _iVtxBufferSize / sizeof(VTX);
	m_tVtxDesc = {};
	m_tVtxDesc.ByteWidth = _iVtxBufferSize; // ũ��
	m_tVtxDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// ���� ������ ���� ���� ����
	m_tVtxDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tVtxDesc.CPUAccessFlags = 0;

	//m_tVtxDesc.Usage = D3D11_USAGE_DYNAMIC;
	//m_tVtxDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // GPU�� �ٽ� �����ؼ� ������ �� �ֵ��� write
	//tBufferDesc.MiscFlags;
	//tBufferDesc.StructureByteStride;

	D3D11_SUBRESOURCE_DATA tSubResData = {};
	tSubResData.pSysMem = _pVtxSys; // �迭�� �����ּ� (ByteWidth�� �����Ǿ��־ ũ�⼳���� �����൵ ��)

	HRESULT hr = DEVICE->CreateBuffer(&m_tVtxDesc, &tSubResData, m_pVB.GetAddressOf());
	if (FAILED(hr)) {
		assert(nullptr);
		return;
	}
		

	// Alt + Tap�� �����ٺ��� GPU���� �޸𸮸� �Ҿ���� �� �����Ƿ�
	// ���� ���� �����͸� �����Ҵ����� �����Ѵ�.
	// ���ؽ� ���� ����
	// system copy
	m_pVtxSys = new VTX[m_iVtxCount];
	memcpy(m_pVtxSys, _pVtxSys, _iVtxBufferSize);

	//////////////////////
	// GPU Memory�� Index Buffer �����
	//////////////////////
	tIndexInfo info = {};
	info.iIdxCount = _iIdxBufferSize / sizeof(UINT);
	info.tIBDesc.ByteWidth = _iIdxBufferSize; // GPU �� ������ ���� �޸� ũ��
	info.tIBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER; // Index Buffer �뵵
	info.tIBDesc.CPUAccessFlags = 0;						// ���� �Ұ��� ����
	info.pIdxSysMem = new UINT[info.iIdxCount];

	// ���� ������ ���� ���� ����
	info.tIBDesc.Usage = _eIdxUsage;
	if (D3D11_USAGE_DYNAMIC == info.tIBDesc.Usage)
		info.tIBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// �ε��� ���� ����
	memcpy(info.pIdxSysMem, _pIdxSys, _iIdxBufferSize);

	tSubResData = {};
	tSubResData.pSysMem = info.pIdxSysMem; // �迭�� �����ּ� (ByteWidth�� �����Ǿ��־ ũ�⼳���� �����൵ ��)

	hr = DEVICE->CreateBuffer(&info.tIBDesc, &tSubResData, info.pIB.GetAddressOf());
	if (FAILED(hr)) {
		assert(nullptr);
		return;
	}

	m_vecIdxInfo.push_back(info);
}

void CMesh::UpdateData(UINT _iSubSet)
{
	// Input Asselmber Stage ����
	UINT iStride = sizeof(VTX); // ���� �ϳ��� �ִ� ������ (����)
	UINT iOffset = 0;

	CONTEXT->IASetVertexBuffers(0, 1, m_pVB.GetAddressOf(), &iStride, &iOffset);
	CONTEXT->IASetIndexBuffer(m_vecIdxInfo[_iSubSet].pIB.Get(), DXGI_FORMAT_R32_UINT, 0); // 4byte unsigned intŸ������
}

void CMesh::Render(UINT _iSubset)
{
	// ���������� ����
	// ���ؽ� ���۸� ����� ���
	//UINT iVertexCnt = 3;
	//UINT iStartVertexLocation = 0;
	// CONTEXT->Draw(iVertexCnt, iStartVertexLocation); 

	// �޽� ����
	UpdateData(_iSubset);
	// �ε��� ���۸� ���
	UINT iStartIndexLocation = 0;
	UINT iBaseVertexLocation = 0;
	CONTEXT->DrawIndexed(m_vecIdxInfo[_iSubset].iIdxCount, iStartIndexLocation, iBaseVertexLocation);
}

void CMesh::Render_Particle(UINT _iCount)
{
	UpdateData(0);
	CONTEXT->DrawIndexedInstanced(m_vecIdxInfo[0].iIdxCount, _iCount, 0, 0, 0);
}

void CMesh::Reset()
{
	D3D11_MAPPED_SUBRESOURCE tSub = {};
	CONTEXT->Map(m_pVB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	memcpy(tSub.pData, m_pVtxSys, m_tVtxDesc.ByteWidth);
	CONTEXT->Unmap(m_pVB.Get(), 0);
}


CMesh* CMesh::CreateFromContainer(CFBXLoader& _loader)
{
	const tContainer* container = &_loader.GetContainer(0);

	UINT iVtxCount = (UINT)container->vecPos.size();

	D3D11_BUFFER_DESC tVtxDesc = {};

	tVtxDesc.ByteWidth = sizeof(VTX) * iVtxCount;
	tVtxDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	tVtxDesc.Usage = D3D11_USAGE_DEFAULT;
	if (D3D11_USAGE_DYNAMIC == tVtxDesc.Usage)
		tVtxDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA tSub = {};
	tSub.pSysMem = malloc(tVtxDesc.ByteWidth);
	VTX* pSys = (VTX*)tSub.pSysMem;
	for (UINT i = 0; i < iVtxCount; ++i)
	{
		pSys[i].vPos = container->vecPos[i];
		pSys[i].vUV = container->vecUV[i];
		pSys[i].vColor = Vector4(1.f, 0.f, 1.f, 1.f);
		pSys[i].vNormal = container->vecNormal[i];
		pSys[i].vTangent = container->vecTangent[i];
		pSys[i].vBinormal = container->vecBinormal[i];
		pSys[i].vWeights = container->vecWeights[i];
		pSys[i].vIndices = container->vecIndices[i];
	}

	ComPtr<ID3D11Buffer> pVB = nullptr;
	HRESULT hr = DEVICE->CreateBuffer(&tVtxDesc, &tSub, pVB.GetAddressOf());
	if (FAILED(hr))
		return nullptr;

	CMesh* pMesh = new CMesh;
	pMesh->m_pVB = pVB;
	pMesh->m_tVtxDesc = tVtxDesc;
	pMesh->m_pVtxSys = pSys;

	// �ε��� ����
	UINT iIdxBufferCount = (UINT)container->vecIdx.size();
	D3D11_BUFFER_DESC tIdxDesc = {};

	for (UINT i = 0; i < iIdxBufferCount; ++i)
	{
		tIdxDesc.ByteWidth = (UINT)container->vecIdx[i].size() * sizeof(UINT); // Index Format �� R32_UINT �̱� ����
		tIdxDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		tIdxDesc.Usage = D3D11_USAGE_DEFAULT;
		if (D3D11_USAGE_DYNAMIC == tIdxDesc.Usage)
			tIdxDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		void* pSysMem = malloc(tIdxDesc.ByteWidth);
		memcpy(pSysMem, container->vecIdx[i].data(), tIdxDesc.ByteWidth);
		tSub.pSysMem = pSysMem;

		ComPtr<ID3D11Buffer> pIB = nullptr;
		if (FAILED(DEVICE->CreateBuffer(&tIdxDesc, &tSub, pIB.GetAddressOf())))
		{
			return NULL;
		}

		tIndexInfo info = {};
		info.tIBDesc = tIdxDesc;
		info.iIdxCount = (UINT)container->vecIdx[i].size();
		info.pIdxSysMem = pSysMem;
		info.pIB = pIB;

		pMesh->m_vecIdxInfo.push_back(info);
	}

	// Animation3D
	if (!container->bAnimation)
		return pMesh;

	vector<tBone*>& vecBone = _loader.GetBones();
	UINT iFrameCount = 0;
	for (UINT i = 0; i < vecBone.size(); ++i)
	{
		tMTBone bone = {};
		bone.iDepth = vecBone[i]->iDepth;
		bone.iParentIndx = vecBone[i]->iParentIndx;
		bone.matBone = GetMatrixFromFbxMatrix(vecBone[i]->matBone);
		bone.matOffset = GetMatrixFromFbxMatrix(vecBone[i]->matOffset);
		bone.strBoneName = vecBone[i]->strBoneName;

		for (UINT j = 0; j < vecBone[i]->vecKeyFrame.size(); ++j)
		{
			tMTKeyFrame tKeyframe = {};
			tKeyframe.dTime = vecBone[i]->vecKeyFrame[j].dTime;
			tKeyframe.iFrame = j;
			tKeyframe.vTranslate.x = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetT().mData[0];
			tKeyframe.vTranslate.y = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetT().mData[1];
			tKeyframe.vTranslate.z = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetT().mData[2];

			tKeyframe.vScale.x = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetS().mData[0];
			tKeyframe.vScale.y = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetS().mData[1];
			tKeyframe.vScale.z = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetS().mData[2];

			tKeyframe.qRot.x = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetQ().mData[0];
			tKeyframe.qRot.y = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetQ().mData[1];
			tKeyframe.qRot.z = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetQ().mData[2];
			tKeyframe.qRot.w = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetQ().mData[3];

			bone.vecKeyFrame.push_back(tKeyframe);
		}

		iFrameCount = max(iFrameCount, (UINT)bone.vecKeyFrame.size());

		pMesh->m_vecBones.push_back(bone);
	}

	vector<tAnimClip*>& vecAnimClip = _loader.GetAnimClip();

	for (UINT i = 0; i < vecAnimClip.size(); ++i)
	{
		tMTAnimClip tClip = {};

		tClip.strAnimName = vecAnimClip[i]->strName;
		tClip.dStartTime = vecAnimClip[i]->tStartTime.GetSecondDouble();
		tClip.dEndTime = vecAnimClip[i]->tEndTime.GetSecondDouble();
		tClip.dTimeLength = tClip.dEndTime - tClip.dStartTime;

		tClip.iStartFrame = (int)vecAnimClip[i]->tStartTime.GetFrameCount(vecAnimClip[i]->eMode);
		tClip.iEndFrame = (int)vecAnimClip[i]->tEndTime.GetFrameCount(vecAnimClip[i]->eMode);
		tClip.iFrameLength = tClip.iEndFrame - tClip.iStartFrame;
		tClip.eMode = vecAnimClip[i]->eMode;

		pMesh->m_vecAnimClip.push_back(tClip);
	}

	// Animation �� �ִ� Mesh ��� structuredbuffer �����α�
	if (pMesh->IsAnimMesh())
	{
		// BoneOffet ���
		vector<Matrix> vecOffset;
		vector<tFrameTrans> vecFrameTrans;
		vecFrameTrans.resize((UINT)pMesh->m_vecBones.size() * iFrameCount);

		for (size_t i = 0; i < pMesh->m_vecBones.size(); ++i)
		{
			vecOffset.push_back(pMesh->m_vecBones[i].matOffset);

			for (size_t j = 0; j < pMesh->m_vecBones[i].vecKeyFrame.size(); ++j)
			{
				vecFrameTrans[(UINT)pMesh->m_vecBones.size() * j + i]
					= tFrameTrans{ Vector4(pMesh->m_vecBones[i].vecKeyFrame[j].vTranslate, 0.f)
					, Vector4(pMesh->m_vecBones[i].vecKeyFrame[j].vScale, 0.f)
					, pMesh->m_vecBones[i].vecKeyFrame[j].qRot };
			}
		}

		pMesh->m_pBoneOffset = new CStructuredBuffer;
		pMesh->m_pBoneOffset->Create(E_StructuredBufferType::ReadOnly, sizeof(Matrix), (UINT)vecOffset.size(), false, vecOffset.data());

		pMesh->m_pBoneFrameData = new CStructuredBuffer;
		pMesh->m_pBoneFrameData->Create(E_StructuredBufferType::ReadOnly, sizeof(tFrameTrans), (UINT)vecOffset.size() * iFrameCount
		, false, vecFrameTrans.data());
	}

	return pMesh;
}