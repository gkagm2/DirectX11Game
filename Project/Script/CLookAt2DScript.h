#pragma once
#include <Engine\CScript.h>
class CLookAt2DScript : public CScript
{
public:
	enum class E_ForwardType {
		X,
		Y
	};
private:
	CGameObject* m_pTargetObj;
	E_ForwardType m_eForwardType; // 기본 앞방향은 X축으로 설정

public:
	virtual void Start() override;
	virtual void Update() override;

	virtual bool SaveToScene(FILE* _pFile)override;
	virtual bool LoadFromScene(FILE* _pFile)override;

public:
	void SetForwardType(E_ForwardType _eType) { m_eForwardType = _eType; }
	E_ForwardType GetForwardType() { return m_eForwardType; }

private:
	void LookAt();

public:
	CLONE(CLookAt2DScript);
	CLookAt2DScript();
	virtual ~CLookAt2DScript() override;
};