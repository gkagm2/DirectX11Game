#pragma once
#include "CRenderer.h"
#include "CMesh.h"
#include "CMaterial.h"

class CMeshRenderer : public CRenderer
{
public:
	CLONE(CMeshRenderer);
	CMeshRenderer();
	virtual ~CMeshRenderer() override;
};