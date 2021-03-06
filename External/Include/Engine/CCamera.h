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
	float fWidth;  // 직교투영 가로
	float fHeight; // 직교투영 세로
};

class CFrustum;
class CCamera : public CComponent
{
private:
	CFrustum* m_pFrustum;
	bool m_bRenderFrustum;

	TRay m_tRay; // 마우스 방향으로 향하는 직선

	E_ProjectionType m_eProjectionType;	// 투영 방식
	
	////// 원근투영 (Perspective)  //////
	TFOVAxis		 m_tFOVAxis;		// field of view axis

	////// 직교투영 (Orthographic) //////
	float			 m_fSize;	// 투영 사이즈 screen size; 

	TClippingPlanes  m_tClippingPlanes; // near far 조절
	TViewportRect	 m_tViewportRect;	// 뷰포트 조절
	////////////////////////////////////////
	UINT			 m_iLayerCheck;		// 렌더링 할 레이어 비트
	////////////////////////////////////////
	Matrix			 m_matView;			// View 행렬
	Matrix			 m_matProjection;	// Prjection 행렬
	Matrix			 m_matViewInv;		// View 역행렬
	Matrix			 m_matProjectionInv;// Projection 역행렬
	////////////////////////////////////////

	// 렌더링 시점별로 분류된 오브젝트들
	vector<CGameObject*> m_vecDecal;
	vector<CGameObject*> m_vecDeferred;
	vector<CGameObject*> m_vecForward;
	vector<CGameObject*> m_vecParticle;
	vector<CGameObject*> m_vecCanvas;	// UI
	vector<CGameObject*> m_vecPostEffect;
	vector<CGameObject*> m_vecCollider2D;

	vector<CGameObject*> m_vecShadowDepth;


public:
	virtual void FinalUpdate() override;
	virtual void UpdateData() override; // 특별히 Final Update 내부에서 처리함 
	void Render();
	
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
	void SetViewportWidth(float _fWidth) { m_tViewportRect.fWidth = _fWidth; }
	void SetViewportHeight(float _fHeight) { m_tViewportRect.fHeight = _fHeight; }

	const TViewportRect& GetViewportRect() { return m_tViewportRect; }

	virtual Vector3 GetScreenToWorld2DPosition(const Vector2& _vPosition);
	virtual Vector2 GetWorldToScreen2DPosition(const Vector3& _vWorldPosition);

	float GetDistancePerFixel(); // 1 pixel당 거리 구하기
	 const Matrix& GetViewMatrix() { return m_matView; }
	 const Matrix& GetProjectionMatrix() { return m_matProjection; }
	 const Matrix& GetViewInverseMatrix() { return m_matViewInv; }
	 const Matrix& GetProjectionInverseMatrix() { return m_matProjectionInv; }
	 const TRay& GetRay() { return m_tRay; }
public:
	void SetLayerCheck(int _iLayerIdx, bool _bFlag) {
		if (_bFlag)
			m_iLayerCheck |= (1 << _iLayerIdx); // bit flag 1 설정
		else
			m_iLayerCheck &= ~(1 << _iLayerIdx); // bit flag 0 설정
	}
	void SetLayerCheckAll() { m_iLayerCheck = 0xffffffff; } // 모든 레이어 렌더링
	void SetLayerCheckAllUnActive() { m_iLayerCheck = 0; }	// 모든 레이어 렌더링 해제

	bool IsLayerChecked(int _iLayerIdx) {
		if (m_iLayerCheck & (1 << _iLayerIdx))
			return true;
		return false;
	}
protected:
	void CalculateViewMatrix();
	void CalculateProjectionMatrix();
	void CalculateRay();
	CFrustum* GetFrustum() { return m_pFrustum; }

public:
	void SetRenderFrustum(bool _bRender) { m_bRenderFrustum = _bRender; }
	bool IsRenderFrustum() { return m_bRenderFrustum; }

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

private:
	// For Rendering
	void _SortObjects(); // 물체들을 렌더링 시점별로 분류
	void _RenderDecal();
	void _RenderDeferred();
	void _RenderForward();
	void _RenderParticle();
	void _RenderPostEffect();
	void _RenderCollider2D();
	void _RenderCanvas(); // UI Canvas

	// 동적 그림자를 생성하기로 한 물체만 확인해서 분류
	void _SortObjects_ShadowDepth();
	void _RenderDynamic_ShadowDepth();

public:
	CLONE(CCamera);
	CCamera();
	CCamera(const CCamera& _origin);
	virtual ~CCamera() override;

	friend class CRenderManager;
};