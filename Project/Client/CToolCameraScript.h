#pragma once
#include <Engine\CScript.h>
class CToolCameraScript : public CScript
{
private:
	float m_fSpeed;
	float m_fMouseScrollSpeed;
	float m_fDragSpeed;
	float m_fPrefSize;

	Vector2 m_vPrevPos;
	Vector2 m_vMoveOffset;
	Vector3 m_vPrevToolCamPos;

	bool m_bMovementActive;

public:
	virtual void Start() override;
	virtual void Update() override;

	void UpdateOrthographic(CCamera* _pCamera);
	void UpdatePerspective(CCamera* _pCamera);

public:
	void SetMovementActive(bool _vMovementActive) { m_bMovementActive = _vMovementActive; }
	bool IsMovementActive() { return m_bMovementActive; }

public:
	CLONE(CToolCameraScript);
	CToolCameraScript();
	virtual ~CToolCameraScript() override;
};