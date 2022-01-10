#pragma once
#include "CInteractiveObj_bu.h"
class CElevator_bu : public CInteractiveObj_bu
{
public:
	enum class E_ElevationType {
		Once,
		Loop,
	};
private:
	float m_fMoveTime;
	bool m_bDirFlag;
	float m_fSpeed;

	CGameObject* m_pStartPosObj;
	CGameObject* m_pEndPosObj;
	Vector3 m_vStartPos;
	Vector3 m_vEndPos;
	E_ElevationType m_eType;

public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update() override;
	virtual void Interaction(bool _bActive) override;

public:
	void Move();
	void Stop();

	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	CLONE(CElevator_bu);
	CElevator_bu();
	CElevator_bu(const CElevator_bu& _origin);
	virtual ~CElevator_bu() override;
};