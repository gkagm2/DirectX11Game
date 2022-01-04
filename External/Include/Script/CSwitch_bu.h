#pragma once
#include <Engine\CScript.h>
class CSwitch_bu : public CScript
{
private:
	CAnimator2D* m_pAnim;
	function<void()> m_ActivateCallBack;
	function<void()> m_DeActivateCallBack;
	bool m_bSwitchState; // 현재 스위치 상태

	// Animation Sprite에 설정할
	int m_iOffIdx;
	int m_iOnIdx;

public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update() override;

	virtual bool SaveToScene(FILE* _pFile)override;
	virtual bool LoadFromScene(FILE* _pFile)override;

public:
	template<typename T>
	void SetActivteCallBack(T* _pObjInst, void(T::* _pFunc)()) {
		m_ActivateCallBack = std::bind(_pFunc, _pObjInst);
	}
	template<typename T>
	void SetDeActivateCallBack(T* _pObjInst, void(T::* _pFunc)()) {
		m_DeActivateCallBack = std::bind(_pFunc, _pObjInst);
	}

	void SwitchActive(bool _bActive);

public:

	CLONE(CSwitch_bu);
	CSwitch_bu();
	virtual ~CSwitch_bu() override;
};