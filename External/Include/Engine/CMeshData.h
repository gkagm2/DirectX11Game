#pragma once

#include "CFBXLoader.h"

#include "Ptr.h"
#include "CMaterial.h"
#include "CMesh.h"

class CGameObject;

class CMeshData :
	public CResource
{
private:
	SharedPtr<CMesh>				m_pMesh;
	vector<SharedPtr<CMaterial>>	m_vecMtrl;

public:
	static CMeshData* LoadFromFBX(const wstring& _strFilePath);
	virtual int Load(const tstring& _strFilePath) override { return S_OK; }

	CGameObject* Instantiate();

	CLONE_DISABLE(CMeshData);

public:
	CMeshData();
	virtual ~CMeshData();
};

