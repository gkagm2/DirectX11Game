#pragma once
#include "CEntity.h"

class CResource : public CEntity
{
private:
	UINT m_iRefCount;			// ���ҽ��� ���� ���� (Ref : Reference)
	tstring m_strKey;			// ���� Ű ��
	tstring m_strRelativePath;  // �����

public:
	virtual void Load(const tstring _strFilePath) = 0;

public:
	CResource();
	virtual ~CResource() override;
};