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
#include "CSound.h"

#include "CKeyManager.h"

class CResourceManager : public CSingleton<CResourceManager>
{
	SINGLETON(CResourceManager)
private:
	unordered_map<tstring, CResource*> m_umapResource[(UINT)E_ResourceType::End];
	vector<CMaterial*> m_vecCloneMtrl; // 복사된 메터리얼들을 담을 벡터
	unordered_map<tstring, CTexture*> m_umapDefaultTex; // 삭제되면 안되는 기본 텍쳐
	bool m_bFixed; // Resource가 추가, 수정되었는지 여부
	SharedPtr<CTexture> m_pPostEffectTargetTex;
public:
	void Init();
	void Update() { m_bFixed = false; }

public:
	void CreateDefaultMesh();
	void CreateDefaultCircle2DMesh();
	void CreateDefaultMesh3D();
	void CreateDefaultTexture();
	void CreateDefaultShader();
	void CreateDefaultMaterial();
	void CreateComputeShader();

	void InitSound();
	void InitInputLayout();
	
public:
	SharedPtr<CTexture> CreateTexture(const tstring& _strKey, UINT _iWidth, UINT _iHeight, DXGI_FORMAT _eFormat, UINT _iBindFlag); // param(_iBindFlag) : D3D11_BIND_FLAG
	SharedPtr<CTexture> CreateTexture(const tstring& _strKey, ComPtr<ID3D11Texture2D> _pTexture2D);

	// strRelativePath값이 없으면 strKey값을 RelativePath값과 동일 시 함.
	template<typename T>
	SharedPtr<T> LoadRes(const tstring& _strKey, const tstring& _strRelativePath = _T(""));

	template<typename T>
	void AddRes(const tstring& _strKey, T* _pRes);

	template<typename T>
	SharedPtr<T> FindRes(const tstring& _strKey);

	


	template<typename T>
	bool IsExistRes(const tstring& _strKey);

	void AddCloneMaterial(SharedPtr<CMaterial> _pMtrl) { m_vecCloneMtrl.push_back(_pMtrl.Get()); }

	void GetResourceKeys(E_ResourceType _eType, vector<tstring>& _vecOut);
	const unordered_map<tstring, CResource*>& GetResources(E_ResourceType _eType) { return m_umapResource[(UINT)_eType]; }

	bool IsFixed() { return m_bFixed; }


public:
	
	void DeleteCopiedMaterialEvn(const tstring& _strKey);
	void ChangeResourceKeyEvn(const tstring& _strKey, const tstring& _strKeyChange, E_ResourceType _eResourceType);
	void DeleteCustomResourceEvn(const tstring& _strKey, E_ResourceType _eResourceType);

	void RenewResourcesFromDir(E_ResourceType _eType);

private:
	
	void _DeleteCopiedMaterial(const tstring& _strKey);
	bool _ChangeResourceKey(const tstring& _strOldKey, const tstring& _strNewKey, E_ResourceType _eResourceType);
	bool _DeleteCustomResource(const tstring& _strKey, E_ResourceType _eResourceType);

public:
	template<typename TYPE>
	void LoadResourcesFromDir(const tstring& _strRelativeDirPath, const tstring& _strFilter);

	SharedPtr<CTexture> GetPostEffectTargetTex() { return m_pPostEffectTargetTex; }
	

	friend class CEventManager;
};

// 리소스 추가 시 타입 지정
template<typename T>
E_ResourceType GetResourceType() {
	const type_info& info = typeid(T);

	const type_info& prefab = typeid(CPrefab);
	const type_info& mtrl = typeid(CMaterial);
	const type_info& mesh = typeid(CMesh);
	const type_info& graphicsShader = typeid(CGraphicsShader);
	const type_info& computeShader= typeid(CComputeShader);
	const type_info& texture = typeid(CTexture);
	const type_info& sound = typeid(CSound);

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
	else if (&info == &sound)
		eResourceType = E_ResourceType::Sound;

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
		assert(nullptr && _T("리소스 로딩 실패"));
		m_bFixed = false;
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
		assert(nullptr && _T("리소스를 추가할 수 없음. 이미 리소스가 존재함."));

	m_bFixed = true;
	_pRes->SetKey(_strKey);
	m_umapResource[(UINT)eResourceType].insert(make_pair(_strKey, _pRes));
}

template<typename T>
inline SharedPtr<T> CResourceManager::FindRes(const tstring& _strKey)
{
	E_ResourceType eResourceType = GetResourceType<T>();

	if (eResourceType == E_ResourceType::End) {
		assert(nullptr && _T("Resource의 타입을 지정하지 않음"));
		return nullptr;
	}

	auto iter = m_umapResource[(UINT)eResourceType].find(_strKey);
	auto endIter = m_umapResource[(UINT)eResourceType].end();

	T* pResource = nullptr;
	if (iter == endIter)
		return nullptr;
	else
		pResource = dynamic_cast<T*>(iter->second);

	assert(pResource && _T("Resource를 찾지 못함."));

	return pResource;
}

template<typename T>
inline bool CResourceManager::IsExistRes(const tstring& _strKey)
{
	bool isExist = true;
	E_ResourceType eResourceType = GetResourceType<T>();

	if (eResourceType == E_ResourceType::End) {
		assert(nullptr && _T("Resource의 타입을 지정하지 않음"));
		isExist = false;
		return isExist;
	}
	auto iter = m_umapResource[(UINT)eResourceType].find(_strKey);
	auto endIter = m_umapResource[(UINT)eResourceType].end();

	T* pResource = nullptr;
	if (iter == endIter)
		isExist = false;
	else {
		pResource = dynamic_cast<T*>(iter->second);
		assert(pResource && _T("Resource를 찾지 못함."));
	}
	return isExist;
}

template<typename TYPE>
inline void CResourceManager::LoadResourcesFromDir(const tstring& _strRelativeDirPath, const tstring& _strFilter)
{
	vector<tstring> vecPath;
	tstring contentPath = CPathManager::GetInstance()->GetContentPath();
	contentPath += _strRelativeDirPath;

	// 1. 파일 경로 안에 있는 모든 리소스 이름을 가져온다.
	vecPath = CPathManager::GetInstance()->GetFilesInDirectory(contentPath, _strFilter);

	// 2. 모든 리소스 이름을 가져와 그 이름을 로딩한다.
	for (int i = 0; i < vecPath.size(); ++i) {
		tstring path = vecPath[i];
		CResourceManager::GetInstance()->LoadRes<TYPE>(vecPath[i], path);
	}
}