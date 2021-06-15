#pragma once
#include "CCollider.h"
class CCollider3D : public CCollider
{

public:
	virtual void Render() override;
	virtual void FinalUpdate() override;

private:
	virtual void UpdateData() override;

public:
	CCollider3D();
	virtual ~CCollider3D() override;
};