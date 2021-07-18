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
	E_ProjectionType m_eProjectionType;	// ���� ���
	
	// �������� (Perspective)
	TFOVAxis m_tFOVAxis;				// field of view axis

	// �������� (Orthographic)
	float m_fSize;						// ���� ������ screen size;

	TClippingPlanes m_tClippingPlanes;  // near far ����
	TViewportRect m_tViewportRect;		// ����Ʈ ����

	Matrix m_matView;					// View ���
	Matrix m_matProjection;				// Prjection ���

	UINT m_iLayerCheck;					// ������ �� ���̾� ��Ʈ

public:
	virtual void FinalUpdate() override;
	virtual void Render() override;
	virtual void Update() override;
	
public:
	void SetProjectionType(E_ProjectionType _eType) { m_eProjectionType = _eType; }
	E_ProjectionType GetProjectionType() { return m_eProjectionType; }

	void SetFOVAxis(const TFOVAxis& _tFOVAxis) { m_tFOVAxis = _tFOVAxis; }
	void SetClippingPlanes(const TClippingPlanes& _tClippingPlanes) { m_tClippingPlanes = _tClippingPlanes; }
	void SetViewportRect(const TViewportRect& _tViewportRect) { m_tViewportRect = _tViewportRect; }

	void SetLayerCheck(int _iLayerIdx, bool _bFlag) {
		if (_bFlag)
			m_iLayerCheck |= (_iLayerIdx << 1); // bit flag 1 ����
		else
			m_iLayerCheck &= ~(_iLayerIdx << 1); // bit flag 0 ����
	}
	void SetLayerCheckAll() { m_iLayerCheck = 0xffffffff; }

private:
	void _CalculateViewMatrix();
	void _CalculateProjectionMatrix();

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CCamera);
	CCamera();
	virtual ~CCamera() override;
};