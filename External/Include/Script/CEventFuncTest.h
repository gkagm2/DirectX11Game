#pragma once
#include <Engine\CScript.h>
class CEventFuncTest :public CScript
{
	virtual void Awake() override;
	virtual void Start() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void OnDestroy() override;
public:
	CLONE(CEventFuncTest);
	CEventFuncTest() :CScript((UINT)SCRIPT_TYPE::EVENTFUNCTEST){}
	~CEventFuncTest(){}
};