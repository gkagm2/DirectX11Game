#pragma once
#include "CInteractiveObj_bu.h"
class CDoor_bu : public CInteractiveObj_bu
{
private:
	Vector3 m_vOpenPos;
	Vector3 m_vClosePos;

	float m_fOpenHeight;
	float m_fDoorMoveTime;

	bool m_bIsDoorOpen;
public:
	virtual void Start() override;
	virtual void Update() override;

public:
	void OpenDoor();
	void CloseDoor();

	virtual void Interaction(bool _bActive) override;

public:
	CLONE(CDoor_bu);
	CDoor_bu();
	virtual ~CDoor_bu() override;
};