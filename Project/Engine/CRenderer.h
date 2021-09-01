#pragma once
#include "CComponent.h"

// Return UINT
#define COLOR_RGBA(r,g,b,a) (UINT)(((((BYTE)a << 24 ) | (BYTE)b << 16) | (BYTE)g << 8) | (BYTE)r)

// Return BYTE
#define COLOR_R_FROM_RGBA(r) ((BYTE)r)
#define COLOR_G_FROM_RGBA(g) ((BYTE)(g >> 8))
#define COLOR_B_FROM_RGBA(b) ((BYTE)(b >> 16))
#define COLOR_A_FROM_RGBA(a) ((BYTE)(a >> 24))

class CRenderer : public CComponent
{
public:
	CRenderer() = delete;
	CRenderer(E_ComponentType _eType);
	virtual ~CRenderer() override;
};