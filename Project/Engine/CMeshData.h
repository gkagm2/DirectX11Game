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
	CGameObject* Instantiate();

public:
	virtual bool Save(const tstring& _strRelativePath) override;
	virtual int Load(const tstring& _strFilePath) override;

public:
	CLONE_DISABLE(CMeshData);
	CMeshData();
	virtual ~CMeshData();
};

