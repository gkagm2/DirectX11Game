#pragma once
#include "CTransform.h"
class CRectTransform : public CTransform
{
public:
	virtual void FinalUpdate() override;
	virtual void UpdateData() override;

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;
public:
	CLONE(CRectTransform);
	CRectTransform();
	virtual ~CRectTransform() override;
};