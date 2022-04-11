#include "pch.h"
#include "CMesh.h"
#include "CDevice.h"
#include "CStructuredBuffer.h"
#include "CPathManager.h"
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

bool CMesh::Save(const tstring& _strRelativePath)
{
	tstring strFileName = CPathManager::GetInstance()->GetContentPath();
	strFileName += _strRelativePath;
	SetRelativePath(_strRelativePath);

	FILE* pFile = nullptr;
	errno_t err = _tfopen_s(&pFile, strFileName.c_str(), _T("wb"));

	assert(pFile);

	// 키값, 상대 경로
	SaveStringToFile(GetName(), pFile);
	SaveStringToFile(GetRelativePath(), pFile);

	// 정점 데이터 저장				
	UINT iByteSize = m_tVtxDesc.ByteWidth;
	FWrite(iByteSize, pFile);
	fwrite(m_pVtxSys, iByteSize, 1, pFile);
	//FWrite(m_pVtxSys, pFile, iByteSize, 1);

	// 인덱스 정보
	UINT iMtrlCount = (UINT)m_vecIdxInfo.size();
	FWrite(iMtrlCount, pFile);

	UINT iIdxBuffSize = 0;
	for (UINT i = 0; i < iMtrlCount; ++i) {
		FWrite(m_vecIdxInfo[i], pFile);
		fwrite(m_vecIdxInfo[i].pIdxSysMem, m_vecIdxInfo[i].iIdxCount * sizeof(UINT), 1, pFile);
	}

	// Animation3D 정보 
	UINT iCount = (UINT)m_vecAnimClip.size();
	FWrite(iCount, pFile);
	for (UINT i = 0; i < iCount; ++i) {
		SaveStringToFile(m_vecAnimClip[i].strAnimName, pFile);
		FWrite(m_vecAnimClip[i].dStartTime, pFile);
		FWrite(m_vecAnimClip[i].dEndTime, pFile);
		FWrite(m_vecAnimClip[i].dTimeLength, pFile);
		FWrite(m_vecAnimClip[i].eMode, pFile);
		FWrite(m_vecAnimClip[i].fUpdateTime, pFile);
		FWrite(m_vecAnimClip[i].iStartFrame, pFile);
		FWrite(m_vecAnimClip[i].iEndFrame, pFile);
		FWrite(m_vecAnimClip[i].iFrameLength, pFile);
	}

	iCount = (UINT)m_vecBones.size();
	FWrite(iCount, pFile);

	for (UINT i = 0; i < iCount; ++i) {
		SaveStringToFile(m_vecBones[i].strBoneName, pFile);
		FWrite(m_vecBones[i].iDepth, pFile);
		FWrite(m_vecBones[i].iParentIndx, pFile);
		FWrite(m_vecBones[i].matBone, pFile);
		FWrite(m_vecBones[i].matOffset, pFile);

		int iFrameCount = (int)m_vecBones[i].vecKeyFrame.size();
		FWrite(iFrameCount, pFile);

		for (int j = 0; j < m_vecBones[i].vecKeyFrame.size(); ++j)
			FWrite(m_vecBones[i].vecKeyFrame[j], pFile);
	}


	fclose(pFile);

	return true;
}

int CMesh::Load(const tstring& _strFilePath)
{
	FILE* pFile = nullptr;
	errno_t err = _tfopen_s(&pFile, _strFilePath.c_str(), _T("rb"));
	if (err) {
		assert(nullptr);
		return E_FAIL;
	}

	// 키값, 상대경로
	tstring strKey, strRelativePath;
	LoadStringFromFile(strKey, pFile);
	LoadStringFromFile(strRelativePath, pFile);

	SetKey(strKey);
	SetRelativePath(strRelativePath);

	// 정점데이터
	UINT iByteSize = 0;
	FRead(iByteSize, pFile);

	m_pVtxSys = (VTX*)malloc(iByteSize);
	fread(m_pVtxSys, 1, iByteSize, pFile);
	//FRead(m_pVtxSys, pFile, iByteSize, 1);


	D3D11_BUFFER_DESC tDesc = {};
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tDesc.ByteWidth = iByteSize;
	tDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA tSubData = {};
	tSubData.pSysMem = m_pVtxSys;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, &tSubData, m_pVB.GetAddressOf())))
		assert(nullptr);

	// 인덱스 정보
	UINT iMtrlCount = 0;
	FRead(iMtrlCount, pFile);

	for (UINT i = 0; i < iMtrlCount; ++i) {
		tIndexInfo info = {};
		FRead(info, pFile);

		UINT iByteWidth = info.iIdxCount * sizeof(UINT);

		void* pSysMem = malloc(iByteWidth);
		info.pIdxSysMem = pSysMem;
		fread(info.pIdxSysMem, iByteWidth, 1, pFile);
		//FRead(info.pIdxSysMem, pFile, iByteWidth, 1);

		tSubData.pSysMem = info.pIdxSysMem;

		if (FAILED(DEVICE->CreateBuffer(&info.tIBDesc, &tSubData, info.pIB.GetAddressOf())))
			assert(nullptr);

		m_vecIdxInfo.push_back(info);
	}

	// Animation3D 정보 읽기
	UINT iCount = 0;
	FRead(iCount, pFile);
	for (UINT i = 0; i < iCount; ++i) {
		tMTAnimClip tClip = {};

		LoadStringFromFile(tClip.strAnimName, pFile);
		FRead(tClip.dStartTime, pFile);
		FRead(tClip.dEndTime, pFile);
		FRead(tClip.dTimeLength, pFile);
		FRead(tClip.eMode, pFile);
		FRead(tClip.fUpdateTime, pFile);
		FRead(tClip.iStartFrame, pFile);
		FRead(tClip.iEndFrame, pFile);
		FRead(tClip.iFrameLength, pFile);

		m_vecAnimClip.push_back(tClip);
	}

	iCount = 0;
	FRead(iCount, pFile);
	m_vecBones.resize(iCount);

	UINT _iFrameCount = 0;
	for (int i = 0; i < iCount; ++i) {
		LoadStringFromFile(m_vecBones[i].strBoneName, pFile);
		FRead(m_vecBones[i].iDepth, pFile);
		FRead(m_vecBones[i].iParentIndx, pFile);
		FRead(m_vecBones[i].matBone, pFile);
		FRead(m_vecBones[i].matOffset, pFile);

		UINT iFrameCount = 0;
		FRead(iFrameCount, pFile);
		m_vecBones[i].vecKeyFrame.resize(iFrameCount);
		_iFrameCount = max(_iFrameCount, iFrameCount);
		for (UINT j = 0; j < iFrameCount; ++j)
			FRead(m_vecBones[i].vecKeyFrame[j], pFile);
	}

	// Animation 이 있는 Mesh 경우 Bone StructuredBuffer 만들기
	if (m_vecAnimClip.size() > 0 && m_vecBones.size() > 0) {
		tstring strBone = GetName();

		// BoneOffet 행렬
		vector<Matrix> vecOffset;
		vector<tFrameTrans> vecFrameTrans;
		vecFrameTrans.resize((UINT)m_vecBones.size() * _iFrameCount);

		for (size_t i = 0; i < m_vecBones.size(); ++i) {
			vecOffset.push_back(m_vecBones[i].matOffset);

			for (size_t j = 0; j < m_vecBones[i].vecKeyFrame.size(); ++j) {
				vecFrameTrans[(UINT)m_vecBones.size() * j + i]
					= tFrameTrans{ Vector4(m_vecBones[i].vecKeyFrame[j].vTranslate, 0.f)
					, Vector4(m_vecBones[i].vecKeyFrame[j].vScale, 0.f)
					, Vector4(m_vecBones[i].vecKeyFrame[j].qRot) };
			}
		}

		m_pBoneOffset = new CStructuredBuffer;
		m_pBoneOffset->Create(E_StructuredBufferType::ReadOnly, sizeof(Matrix), (UINT)vecOffset.size(), false, vecOffset.data());

		m_pBoneFrameData = new CStructuredBuffer;
		m_pBoneFrameData->Create(E_StructuredBufferType::ReadOnly, sizeof(tFrameTrans), (UINT)vecOffset.size() * (UINT)_iFrameCount, false, vecFrameTrans.data());
	}

	fclose(pFile);

	return S_OK;
}

void CMesh::Create(void* _pVtxSys, UINT _iVtxBufferSize, void* _pIdxSys, UINT _iIdxBufferSize, D3D11_USAGE _eIdxUsage)
{

	////////////////////
	// 버텍스 버퍼 만들기
	////////////////////
	m_iVtxCount = _iVtxBufferSize / sizeof(VTX);
	m_tVtxDesc = {};
	m_tVtxDesc.ByteWidth = _iVtxBufferSize; // 크기
	m_tVtxDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// 생성 이후의 수정 여부 설정
	m_tVtxDesc.Usage = D3D11_USAGE_DEFAULT;
	m_tVtxDesc.CPUAccessFlags = 0;

	//m_tVtxDesc.Usage = D3D11_USAGE_DYNAMIC;
	//m_tVtxDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // GPU에 다시 접근해서 수정할 수 있도록 write
	//tBufferDesc.MiscFlags;
	//tBufferDesc.StructureByteStride;

	D3D11_SUBRESOURCE_DATA tSubResData = {};
	tSubResData.pSysMem = _pVtxSys; // 배열의 시작주소 (ByteWidth가 설정되어있어서 크기설정은 안해줘도 됨)

	HRESULT hr = DEVICE->CreateBuffer(&m_tVtxDesc, &tSubResData, m_pVB.GetAddressOf());
	if (FAILED(hr)) {
		assert(nullptr);
		return;
	}
		

	// Alt + Tap을 누르다보면 GPU에서 메모리를 잃어버릴 수 있으므로
	// 버퍼 원형 데이터를 동적할당으로 관리한다.
	// 버텍스 정보 보관
	// system copy
	m_pVtxSys = new VTX[m_iVtxCount];
	memcpy(m_pVtxSys, _pVtxSys, _iVtxBufferSize);

	//////////////////////
	// GPU Memory에 Index Buffer 만들기
	//////////////////////
	tIndexInfo info = {};
	info.iIdxCount = _iIdxBufferSize / sizeof(UINT);
	info.tIBDesc.ByteWidth = _iIdxBufferSize; // GPU 에 생성할 버퍼 메모리 크기
	info.tIBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER; // Index Buffer 용도
	info.tIBDesc.CPUAccessFlags = 0;						// 수정 불가능 버퍼
	info.pIdxSysMem = new UINT[info.iIdxCount];

	// 생성 이후의 수정 여부 설정
	info.tIBDesc.Usage = _eIdxUsage;
	if (D3D11_USAGE_DYNAMIC == info.tIBDesc.Usage)
		info.tIBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// 인덱스 정보 보관
	memcpy(info.pIdxSysMem, _pIdxSys, _iIdxBufferSize);

	tSubResData = {};
	tSubResData.pSysMem = info.pIdxSysMem; // 배열의 시작주소 (ByteWidth가 설정되어있어서 크기설정은 안해줘도 됨)

	hr = DEVICE->CreateBuffer(&info.tIBDesc, &tSubResData, info.pIB.GetAddressOf());
	if (FAILED(hr)) {
		assert(nullptr);
		return;
	}

	m_vecIdxInfo.push_back(info);
}

void CMesh::UpdateData(UINT _iSubSet)
{
	// Input Asselmber Stage 셋팅
	UINT iStride = sizeof(VTX); // 정점 하나의 최대 사이즈 (간격)
	UINT iOffset = 0;

	CONTEXT->IASetVertexBuffers(0, 1, m_pVB.GetAddressOf(), &iStride, &iOffset);
	CONTEXT->IASetIndexBuffer(m_vecIdxInfo[_iSubSet].pIB.Get(), DXGI_FORMAT_R32_UINT, 0); // 4byte unsigned int타입으로
}

void CMesh::Render(UINT _iSubset)
{
	// 파이프라인 시작
	// 버텍스 버퍼를 사용할 경우
	//UINT iVertexCnt = 3;
	//UINT iStartVertexLocation = 0;
	// CONTEXT->Draw(iVertexCnt, iStartVertexLocation); 

	// 메쉬 세팅
	UpdateData(_iSubset);
	// 인덱스 버퍼를 사용
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

	// 인덱스 정보
	UINT iIdxBufferCount = (UINT)container->vecIdx.size();
	D3D11_BUFFER_DESC tIdxDesc = {};

	for (UINT i = 0; i < iIdxBufferCount; ++i)
	{
		tIdxDesc.ByteWidth = (UINT)container->vecIdx[i].size() * sizeof(UINT); // Index Format 이 R32_UINT 이기 때문
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

	// Animation 이 있는 Mesh 경우 structuredbuffer 만들어두기
	if (pMesh->IsAnimMesh())
	{
		// BoneOffet 행렬
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