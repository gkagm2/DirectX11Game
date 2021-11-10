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
	TFOVAxis		 m_tFOVAxis;				// field of view axis

	// �������� (Orthographic)
	float			 m_fSize;						// ���� ������ screen size;

	TClippingPlanes  m_tClippingPlanes;  // near far ����
	TViewportRect	 m_tViewportRect;		// ����Ʈ ����

	////////////////////////////////////////
	Matrix			 m_matView;					// View ���
	Matrix			 m_matProjection;				// Prjection ���

	UINT			 m_iLayerCheck;					// ������ �� ���̾� ��Ʈ

	// ������ �������� �з��� ������Ʈ��
	vector<CGameObject*> m_vecForward;
	vector<CGameObject*> m_vecParticle;
	vector<CGameObject*> m_vecCanvas; // UI
	vector<CGameObject*> m_vecPostEffect;
	vector<CGameObject*> m_vecCollider2D;

public:
	virtual void FinalUpdate() override;
	
public:
	
	void SetProjectionType(E_ProjectionType _eType) { m_eProjectionType = _eType; }
	E_ProjectionType GetProjectionType() { return m_eProjectionType; }

public: // Perspective
	void SetFOVAxis(const TFOVAxis& _tFOVAxis) { m_tFOVAxis = _tFOVAxis; }
	const TFOVAxis& GetFOVAxis() { return m_tFOVAxis; }

public: // Orthographic
	void SetSize(float _fSize) {
		m_fSize = CMyMath::Clamp(_fSize, 0.001f, FLT_MAX);
	}
	float GetSize() { return m_fSize; }
	void SetClippingPlanes(const TClippingPlanes& _tClippingPlanes) { m_tClippingPlanes = _tClippingPlanes; }
	const TClippingPlanes& GetClippingPlanes() { return m_tClippingPlanes; }
	void SetViewportRect(const TViewportRect& _tViewportRect) { m_tViewportRect = _tViewportRect; }
	const TViewportRect& GetViewportRect() { return m_tViewportRect; }

	virtual const Vector3& GetScreenToWorld2DPosition(const Vector2& _vPosition);
	virtual const Vector2& GetWorldToScreen2DPosition(const Vector3& _vWorldPosition);

	float GetDistancePerFixel(); // 1 pixel�� �Ÿ� ���ϱ�

public:
	void SetLayerCheck(int _iLayerIdx, bool _bFlag) {
		if (_bFlag)
			m_iLayerCheck |= (1 << _iLayerIdx); // bit flag 1 ����
		else
			m_iLayerCheck &= ~(1 << _iLayerIdx); // bit flag 0 ����
	}
	void SetLayerCheckAll() { m_iLayerCheck = 0xffffffff; } // ��� ���̾� ������
	void SetLayerCheckAllUnActive() { m_iLayerCheck = 0; }	// ��� ���̾� ������ ����

	bool IsLayerChecked(int _iLayerIdx) {
		if (m_iLayerCheck & (1 << _iLayerIdx))
			return true;
		return false;
	}
protected:
	void CalculateViewMatrix();
	void CalculateProjectionMatrix();
	const Matrix& GetViewMatrix() { return m_matView; }
	const Matrix& GetProjectionMatrix() { return m_matProjection; }

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

private:
	// For Rendering
	void _SortObjects(); // ��ü���� ������ �������� �з�
	void _RenderForward();
	void _RenderParticle();
	void _RenderPostEffect();
	void _RenderCollider2D();
	void _RenderCanvas(); // UI Canvas
public:
	CLONE(CCamera);
	CCamera();
	virtual ~CCamera() override;

	friend class CRenderManager;
};