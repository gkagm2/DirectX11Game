#pragma once
#include "CComponent.h"

struct TFOVAxis {
	// use degree
	float fVertical;
	float fHorizontal;
};

struct TClippingPlanes {
	float fNear;
	float fFar;
	TClippingPlanes() : fNear(1.f), fFar(1000.f){}
	TClippingPlanes(float _fNear, float _fFar) : fNear(_fNear), fFar(_fFar){}
};

struct TViewportRect {
	float fX;
	float fY;
	float fWidth;
	float fHeight;
};

class CCamera : public CComponent
{
private:
	E_ProjectionType m_eProjectionType;	// 투영 방식
	
	// 원근투영 (Perspective)
	TFOVAxis m_tFOVAxis;				// field of view axis

	// 직교투영 (Orthographic)
	float m_fSize;						// 투영 사이즈 screen size;

	TClippingPlanes m_tClippingPlanes;  // near far 조절
	TViewportRect m_tViewportRect;		// 뷰포트 조절

	Matrix m_matView;					// View 행렬
	Matrix m_matProjection;				// Prjection 행렬

public:
	virtual void LateUpdate() override;

public:
	void SetProjectionType(E_ProjectionType _eType) { m_eProjectionType = _eType; }
	E_ProjectionType GetProjectionType() { return m_eProjectionType; }

	void SetFOVAxis(const TFOVAxis& _tFOVAxis) { m_tFOVAxis = _tFOVAxis; }
	void SetClippingPlanes(const TClippingPlanes& _tClippingPlanes) { m_tClippingPlanes = _tClippingPlanes; }
	void SetViewportRect(const TViewportRect& _tViewportRect) { m_tViewportRect = _tViewportRect; }

private:
	void _CalculateViewMatrix();
	void _CalculateProjectionMatrix();

public:
	CCamera();
	virtual ~CCamera() override;
};