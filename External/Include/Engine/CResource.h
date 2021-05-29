#pragma once
#include "CObject.h"

class CResource : public CObject
{
private:
	UINT m_iRefCount;			// 리소스를 참조 개수 (Ref : Reference)
	tstring m_strKey;			// 고유 키 값
	tstring m_strRelativePath;  // 상대경로

public:
	virtual void Load(const tstring& _strFilePath) = 0;

private:
	void AddRef() { ++m_iRefCount; }
	void SubRef() { --m_iRefCount; }

public:
	CResource();
	virtual ~CResource() override;
};