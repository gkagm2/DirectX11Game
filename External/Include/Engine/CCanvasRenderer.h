#pragma once
#include "CRenderer.h"
class CCanvasRenderer : public CRenderer
{
public:
	virtual void Render() override;
public:
	CCanvasRenderer();
	virtual ~CCanvasRenderer() override;
};