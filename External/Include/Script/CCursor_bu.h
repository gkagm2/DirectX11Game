#pragma once
#include "CCursorScript.h"
class CCursor_bu : public CScript
{
private:
	vector<TCursorInfo> m_vecCursorInfo;
	int m_iCurIdx; // ???? Ŀ???? ?ε???

	CGameObject* m_pCursor;

	void AddCursor(const TCursorInfo& _tCursorInfo) { m_vecCursorInfo.push_back(_tCursorInfo); }
	vector<TCursorInfo>& GetCursors() { return m_vecCursorInfo; }

	// ?»??? ??ǥ?κ????? Offset
	void SetCursor(int _idx, const Vector2 _vHotSpotOffset);

public:
	virtual void Awake() override;
	virtual void Start() override;

public:
	CLONE(CCursor_bu);
	CCursor_bu();
	virtual ~CCursor_bu() override;
};