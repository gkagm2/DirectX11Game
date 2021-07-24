#pragma once
#include "CPrefab.h"
#include "CMesh.h"
#include "CGraphicsShader.h"
#include "CComputeShader.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CPathManager.h"
#include "CCollider2D.h"
#include "CCollider3D.h"


class CResourceManager : public CSingleton<CResourceManager>
{
	SINGLETON(CResourceManager)
private:
	unordered_map<tstring, CResource*> m_umapResource[(UINT)E_ResourceType::End];
	vector<CMaterial*> m_vecCloneMtrl; // ����� ���͸������ ���� ����

public:
	void Init();

public:
	void CreateDefaultMesh();
	void CreateDefaultCircle2DMesh();
	void CreateDefaultCubeMesh3D();
	void CreateDefaultTexture();
	void CreateDefaultShader();
	void CreateDefaultMaterial();
	void CreateComputeShader();
	
	SharedPtr<CTexture> CreateTexture(const tstring& _strKey, UINT _iWidth, UINT _iHeight, DXGI_FORMAT _eFormat, UINT _iBindFlag);
	SharedPtr<CTexture> CreateTexture(const tstring& _strKey, ComPtr<ID3D11Texture2D> _pTexture2D);

public:
	// TODO : ���߿� ���̴� �ڵ��� �Լ��̸�..etc �� �о���� �� ��� �� ����.

	// strRelativePath���� ������ strKey���� RelativePath���� ���� �� ��.
	template<typename T>
	SharedPtr<T> LoadRes(const tstring& _strKey, const tstring& _strRelativePath = _T(""));

	template<typename T>
	void AddRes(const tstring& _strKey, T* _pRes);

	template<typename T>
	SharedPtr<T> FindRes(const tstring& _strKey);

	template<typename T>
	bool IsExistRes(const tstring& _strKey);


	void AddCloneMaterial(SharedPtr<CMaterial> _pMtrl) { m_vecCloneMtrl.push_back(_pMtrl.Get()); }

	void GetResourceNames(E_ResourceType _eType, vector<tstring>& _vecOut);
};

// ���ҽ� �߰� �� Ÿ�� ����
template<typename T>
E_ResourceType GetResourceType() {
	const type_info& info = typeid(T);

	const type_info& prefab = typeid(CPrefab);
	const type_info& mtrl = typeid(CMaterial);
	const type_info& mesh = typeid(CMesh);
	const type_info& graphicsShader = typeid(CGraphicsShader);
	const type_info& computeShader= typeid(CComputeShader);
	const type_info& texture = typeid(CTexture);

	E_ResourceType eResourceType = E_ResourceType::End;

	if (&info == &prefab)
		eResourceType = E_ResourceType::Prefab;
	else if (&info == &mtrl)
		eResourceType = E_ResourceType::Material;
	else if (&info == &mesh)
		eResourceType = E_ResourceType::Mesh;
	else if (&info == &graphicsShader)
		eResourceType = E_ResourceType::GraphicsShader;
	else if (&info == &computeShader)
		eResourceType = E_ResourceType::ComputeShader;
	else if (&info == &texture)
		eResourceType = E_ResourceType::Texture;

	return eResourceType;
}

template<typename T>
inline SharedPtr<T> CResourceManager::LoadRes(const tstring& _strKey, const tstring& _strRelativePath)
{
	E_ResourceType eType = GetResourceType<T>();

	T* pResource = FindRes<T>(_strKey).Get();
	if (nullptr != pResource)
		return pResource;

	tstring strRelativePath = _strRelativePath;
	if (_T("") == _strRelativePath)
		strRelativePath = _strKey;

	tstring strFilePath = CPathManager::GetInstance()->GetContentPath() + strRelativePath;

	pResource = new T;

	if (FAILED(((CResource*)pResource)->Load(strFilePath))) {
		assert(nullptr && _T("���ҽ� �ε� ����"));
		return nullptr;
	}

	pResource->SetRelativePath(strRelativePath);
	AddRes<T>(_strKey, pResource);

	return pResource;
}

template<typename T>
inline void CResourceManager::AddRes(const tstring& _strKey, T* _pRes)
{
	E_ResourceType eResourceType = GetResourceType<T>();
	bool bIsExistRes = IsExistRes<T>(_strKey);
	if (bIsExistRes)
		assert(nullptr && _T("���ҽ��� �߰��� �� ����. �̹� ���ҽ��� ������."));

	_pRes->SetKey(_strKey);
	m_umapResource[(UINT)eResourceType].insert(make_pair(_strKey, _pRes));
}

template<typename T>
inline SharedPtr<T> CResourceManager::FindRes(const tstring& _strKey)
{
	E_ResourceType eResourceType = GetResourceType<T>();

	if (eResourceType == E_ResourceType::End) {
		assert(nullptr && _T("Resource�� Ÿ���� �������� ����"));
		return nullptr;
	}

	auto iter = m_umapResource[(UINT)eResourceType].find(_strKey);
	auto endIter = m_umapResource[(UINT)eResourceType].end();

	T* pResource = nullptr;
	if (iter == endIter)
		return nullptr;
	else
		pResource = dynamic_cast<T*>(iter->second);

	assert(pResource && _T("Resource�� ã�� ����."));

	return pResource;
}

template<typename T>
inline bool CResourceManager::IsExistRes(const tstring& _strKey)
{
	bool isExist = true;
	E_ResourceType eResourceType = GetResourceType<T>();

	if (eResourceType == E_ResourceType::End) {
		assert(nullptr && _T("Resource�� Ÿ���� �������� ����"));
		return isExist = false;
	}
	auto iter = m_umapResource[(UINT)eResourceType].find(_strKey);
	auto endIter = m_umapResource[(UINT)eResourceType].end();

	T* pResource = nullptr;
	if (iter == endIter)
		isExist = false;
	else {
		pResource = dynamic_cast<T*>(iter->second);
		assert(pResource && _T("Resource�� ã�� ����."));
	}
	return isExist;
}
