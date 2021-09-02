#pragma once
#include "CObject.h"
#include "CBehaviour.h"

class CUI : public CBehaviour
{
private:
	// Collider
	SharedPtr<CMesh> m_pColMesh;
	SharedPtr<CMaterial> m_pColMtrl;

	Vector2 m_vOffsetPosition;
	Vector2 m_vOffsetScale;

	Matrix m_matColWorld;

protected:
	bool m_bIsOn;
	bool m_bIsDown; // ���콺�� �����°�

public:
	virtual void Awake() override {};
	virtual void Start() override {};
	virtual void PrevUpdate() override {};
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void FinalUpdate();
	virtual void Render() override;
	virtual void UpdateData() override;

public:
	// TODO : �̰� �ٸ��ɷ� ����?
	// EventTrigger
	virtual void OnPointerDown();
	virtual void OnPointerUp();
	virtual void OnPointerClick();

	bool IsPointerOn(const Vector2& _vPointerPosition);

public:
	virtual Vector2 GetMin(); // Screen ��ǥ�� ����
	virtual Vector2 GetMax(); // Screen ��ǥ�� ����

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