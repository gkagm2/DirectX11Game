#pragma once
#include "CObject.h"

class CResource : public CObject
{
private:
	UINT m_iRefCount;			// ���ҽ��� ���� ���� (Ref : Reference)
	tstring m_strKey;			// ���� Ű ��
	tstring m_strRelativePath;  // �����

public:
	virtual void Load(const tstring& _strFilePath) = 0;

private:
	void AddRef() { ++m_iRefCount; }
	void SubRef() { --m_iRefCount; }

public:
	CResource();
	virtual ~CResource() override;
};