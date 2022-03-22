#pragma once
#include "CComponent.h"
class CBoundingBox : public CComponent
{
private:
	SharedPtr<CMesh> m_pMesh;
	SharedPtr<CMaterial> m_pMtrl;
public:
	virtual void Render() override;

public:
	CLONE(CBoundingBox);
	CBoundingBox();
	virtual ~CBoundingBox() override;
};