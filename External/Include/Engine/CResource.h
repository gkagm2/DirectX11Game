#pragma once
#include "CObject.h"

class CResource : public CObject
{
private:
	UINT m_iRefCount;			// ���ҽ��� ���� ���� (Ref : Reference)
	tstring m_strKey;			// ���� Ű ��
	tstring m_strRelativePath;  // �����

private:
	virtual int Load(const tstring& _strFilePath) = 0;

private:
	void AddRef() { ++m_iRefCount; }
	void SubRef() { --m_iRefCount; }

private:
	void SetKey(const tstring& _strKey) { m_strKey = _strKey; }
	const tstring& GetKey() { return m_strKey; }
	void SetRelativePath(const tstring& _strRelativePath) {
		m_strRelativePath = _strRelativePath; }
	const tstring& GetRelativePath() { return m_strRelativePath; }

public:
	CResource();
	virtual ~CResource() override;

	friend class CResourceManager;
};