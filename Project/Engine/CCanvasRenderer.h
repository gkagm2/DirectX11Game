#pragma once
#include "CRenderer.h"
class CCanvasRenderer : public CRenderer
{
public:
	virtual void Render() override;

public:
	CLONE(CCanvasRenderer);
	CCanvasRenderer();
	virtual ~CCanvasRenderer() override;
};