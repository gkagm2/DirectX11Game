#pragma once
#include <Engine\CScript.h>
class CInteractiveObj_bu : public CScript
{
protected:
	function<void()> m_ActivateCallBack;
	function<void()> m_DeActivateCallBack;
	bool m_bIsActivate;

public:
	virtual void Awake() override{}
	virtual void Start() override{}
	virtual void Update() override{}

	virtual bool SaveToScene(FILE* _pFile)override { return true; }
	virtual bool LoadFromScene(FILE* _pFile)override { return true; }

public:
	template<typename T>
	void SetActivateCallBack(T* _pObjInst, void(T::* _pFunc)()) {
		m_ActivateCallBack = std::bind(_pFunc, _pObjInst);
	}
	template<typename T>
	void SetDeActivateCallBack(T* _pObjInst, void(T::* _pFunc)()) {
		m_DeActivateCallBack = std::bind(_pFunc, _pObjInst);
	}

	virtual void Interaction(bool _bActive) {}
	bool IsActive() { return m_bIsActivate; }

public:
	CLONE(CInteractiveObj_bu);
	CInteractiveObj_bu();
	CInteractiveObj_bu(UINT _iScriptType);
	virtual ~CInteractiveObj_bu() override;
};