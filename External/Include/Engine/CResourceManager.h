#pragma once
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
	vector<CMaterial*> m_vecCloneMtrl; // 복사된 메터리얼들을 담을 벡터

public:
	void Init();

public:
	void CreateDefaultMesh();
	void CreateDefaultCubeMesh3D();
	void CreateDefaultShader();
	void CreateDefaultMaterial();

public:
	// TODO : 나중에 쉐이더 코드의 함수이름..etc 을 읽어들일 때 사용 할 것임.
	template<typename T>
	SharedPtr<T> Load(const tstring& _strKey, const tstring& _strRelativePath);

	template<typename T>
	void AddRes(const tstring& _strKey, T* _pRes);

	template<typename T>
	SharedPtr<T> FindRes(const tstring& _strKey);

	void AddCloneMaterial(SharedPtr<CMaterial> _pMtrl) { m_vecCloneMtrl.push_back(_pMtrl.Get()); }
};

// 리소스 추가 시 타입 지정
template<typename T>
E_ResourceType GetResourceType() {
	const type_info& info = typeid(T);

	const type_info& mtrl = typeid(CMaterial);
	const type_info& mesh = typeid(CMesh);
	const type_info& graphicsShader = typeid(CGraphicsShader);
	const type_info& computeShader= typeid(CComputeShader);
	const type_info& texture = typeid(CTexture);

	E_ResourceType eResourceType = E_ResourceType::End;

	if (&info == &mtrl)
		eResourceType = E_ResourceType::Material;
	else if (&info == &mesh)
		eResourceType = E_ResourceType::Mesh;
	else if (&info == &graphicsShader || &info == &computeShader)
		eResourceType = E_ResourceType::Shader;
	else if (&info == &texture)
		eResourceType = E_ResourceType::Texture;

	return eResourceType;
}

template<typename T>
inline SharedPtr<T> CResourceManager::Load(const tstring& _strKey, const tstring& _strRelativePath)
{
	E_ResourceType eType = GetResourceType<T>();

	/*T* pResource = FindRes<T>(_strKey).Get();
	if (nullptr != pResource) {
		MessageBox(nullptr, STR_MSG_FailDuplicateResourceKey, STR_MSG_FailedToLoadResource, MB_OK);
		assert(nullptr);
		return nullptr;
	}
	pResource = new T;
	*/

	T* pResource = new T;
	tstring strFilePath = CPathManager::GetInstance()->GetContentPath() + _strRelativePath;

	if (FAILED(((CResource*)pResource)->Load(strFilePath))) {
		assert(nullptr && _T("리소스 로딩 실패"));
		return nullptr;
	}

	pResource->SetKey(_strKey);
	pResource->SetRelativePath(_strRelativePath);
	AddRes<T>(_strKey, pResource);

	return pResource;
}

template<typename T>
inline void CResourceManager::AddRes(const tstring& _strKey, T* _pRes)
{
	E_ResourceType eResourceType = GetResourceType<T>();
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
		pResource = Load<T>(_strKey, _strKey).Get();
	else
		pResource = dynamic_cast<T*>(iter->second);

	assert(pResource && _T("Resource를 찾지 못함."));

	return pResource;
}