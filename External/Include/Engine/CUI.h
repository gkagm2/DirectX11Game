#pragma once
#include "CObject.h"
#include "CBehaviour.h"

class CUI : public CBehaviour
{
private:
	SharedPtr<CMesh> m_pColMesh;
	SharedPtr<CMaterial> m_pColMtrl;
protected:
	bool m_bIsOn;
	bool m_bIsDown; // 마우스를 눌렀는가

public:
	virtual void Awake() override {};
	virtual void Start() override {};
	virtual void PrevUpdate() override {};
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void FinalUpdate();
	virtual void Render() override;

public:
	// TODO : 이걸 다른걸로 뺄까?
	// EventTrigger
	virtual void OnPointerDown();
	virtual void OnPointerUp();
	virtual void OnPointerClick();

	bool IsPointerOn(const Vector2& _vPointerPosition);

public:
	virtual Vector2 GetMin(); // Screen 좌표로 구함
	virtual Vector2 GetMax(); // Screen 좌표로 구함

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