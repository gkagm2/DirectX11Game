#pragma once
#include "CObject.h"
#include "Ptr.h"
class CResource : public CObject
{
private:
	UINT m_iRefCount;			// 리소스를 참조 개수 (Ref : Reference)
	tstring m_strKey;			// 고유 키 값
	tstring m_strRelativePath;  // 상대경로

private:
	virtual int Load(const tstring& _strFilePath) = 0;

private:
	void AddRef() { ++m_iRefCount; }
	void SubRef() { --m_iRefCount; }

public:
	void SetKey(const tstring& _strKey) { m_strKey = _strKey; }
	const tstring& GetKey() { return m_strKey; }
	void SetRelativePath(const tstring& _strRelativePath) {
		m_strRelativePath = _strRelativePath; }
	const tstring& GetRelativePath() { return m_strRelativePath; }

public:
	CResource();
	virtual ~CResource() override;

	friend class CResourceManager;

	template<typename T>
	friend class SharedPtr;
};