#pragma once
#include "CObject.h"
#include "Ptr.h"
class CResource : public CObject
{
private:
	UINT m_iRefCount;			// ���ҽ��� ���� ���� (Ref : Reference)
	tstring m_strKey;			// ���� Ű ��
	tstring m_strRelativePath;  // �����

	E_ResourceType m_eResourceType;

public:
	virtual bool Save(const tstring& _strRelativePath) { return true; }
private:
	virtual int Load(const tstring& _strFilePath) = 0; // 0 : S_OK, 1 : fail

	virtual void UpdateData() override {}
private:
	void AddRef() { ++m_iRefCount; }
	void SubRef() { --m_iRefCount; }

public:
	void SetKey(const tstring& _strKey) { m_strKey = _strKey; }
	const tstring& GetKey() { return m_strKey; }
	void SetRelativePath(const tstring& _strRelativePath) {
		m_strRelativePath = _strRelativePath; }
	const tstring& GetRelativePath() { return m_strRelativePath; }

	E_ResourceType GetResourceType() { return m_eResourceType; }

public:
	CResource(E_ResourceType _eType);
	virtual ~CResource() override;

	friend class CResourceManager;

	template<typename T>
	friend class SharedPtr;
};