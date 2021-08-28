#pragma once
#include "CObject.h"
#include "CBehaviour.h"

enum class E_UIPivot {
	leftTop,
	left,
	leftBottom,
	top,
	center,
	botton,
	rightTop,
	right,
	rightBottom
};

class CUI : public CBehaviour
{
protected:
	vector<CUI*> m_vecChildUI;
	CUI* m_pParentUI;

	bool m_bIsOn;
	bool m_bIsDown; // ���콺�� �����°�

	E_UIPivot m_ePivotState;
	Vector3 m_vFinalPosition;

public:
	virtual void Awake() override {};
	virtual void Start() override {};
	virtual void PrevUpdate() override {};
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void FinalUpdate();
	virtual void Render() override;

public:
	// TODO : �̰� �ٸ��ɷ� ����?
	// EventTrigger
	virtual void OnPointerDown();
	virtual void OnPointerUp();
	virtual void OnPointerClick();

	bool IsPointerOn(const Vector2& _vPointerPosition);

public:
	void AddChildUI(CUI* _pChildUI) {
		_pChildUI->m_pParentUI = this;
		m_vecChildUI.push_back(_pChildUI);
	}
	
	vector<CUI*>& GetChildsUI() { return m_vecChildUI; }
	CUI* GetParentUI() { return m_pParentUI; }

	void SetPivotState(E_UIPivot _ePivot) { m_ePivotState = _ePivot; }
	E_UIPivot GetPivotState(E_UIPivot _ePivot) { return m_ePivotState; }
	Vector3 GetFinalPosition() { return m_vFinalPosition; }

	// TODO : Min, Max ���ϱ� ����
	virtual Vector3 GetMin();
	virtual Vector3 GetMax();

public:
	virtual CUI* Clone() = 0;

protected:
	CUI(const CUI& _origin);

public:
	CUI(E_ComponentType _eComponentType);
	virtual ~CUI() override;

public:
	friend class CUIManager;
};