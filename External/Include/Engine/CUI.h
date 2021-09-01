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
	bool m_bIsDown; // ���콺�� �����°�

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