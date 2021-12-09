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


	Vector3 m_vRotOrtho;
	Vector3 m_vRotPerspec;

public:
	virtual void Start() override;
	virtual void Update() override;

private:
	void _UpdateOrthographic(CCamera* _pCamera);
	void _UpdatePerspective(CCamera* _pCamera);

public:
	virtual bool SaveToScene(FILE* _pFile) override;
	virtual bool LoadFromScene(FILE* _pFile) override;

public:
	void ChangeProjectionType(E_ProjectionType _eType);

	void SetMovementActive(bool _vMovementActive) { m_bMovementActive = _vMovementActive; }
	bool IsMovementActive() { return m_bMovementActive; }

public:
	CLONE(CToolCameraScript);
	CToolCameraScript();
	virtual ~CToolCameraScript() override;
};