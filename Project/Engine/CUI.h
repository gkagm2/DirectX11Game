#pragma once
#include "CObject.h"
#include "CBehaviour.h"

typedef 

class CUI : public CBehaviour
{
private:
	// Collider
	SharedPtr<CMesh> m_pColMesh;
	SharedPtr<CMaterial> m_pColMtrl;

	Vector2 m_vOffsetPosition;
	Vector2 m_vOffsetScale;

	Matrix m_matColWorld;
	bool m_bActiveClickEvent;
protected:
	bool m_bIsOn;
	bool m_bIsDown; // 마우스를 눌렀는가


private:
	// Click 시 실행 할 콜백용 함수
protected:
	std::function<void()> m_PointerDownCallBack;
	std::function<void()> m_PointerUpCallBack;
	std::function<void()> m_ClickCallBack;


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
	// TODO : 이걸 다른걸로 뺄까?
	// EventTrigger
	virtual void OnPointerDown();
	virtual void OnPointerUp();
	virtual void OnPointerClick();

public:
	template<typename T>
	void SetPointerDownCallBack(T* _pObj, void(T::* _Func)()) {
		m_PointerDownCallBack = std::bind(_Func, _pObj);
	}
	template<typename T>
	void SetPointerUpCallBack(T* _pObj, void(T::* _Func)()) {
		m_PointerUpCallBack = std::bind(_pObj, _Func);
	}
	template<typename T>
	void SetClickCallBack(T* _pObj, void(T::* _Func)()) {
		m_ClickCallBack = std::bind(_Func, _pObj);
	}

	void ActiveClickEvent(bool _bActive) { m_bActiveClickEvent = _bActive; }
	bool IsActiveClickEvent() { return m_bActiveClickEvent; }

public:
	virtual Vector2 GetMin(); // Screen 좌표로 구함
	virtual Vector2 GetMax(); // Screen 좌표로 구함

	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

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