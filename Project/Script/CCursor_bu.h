#pragma once
#include "CCursorScript.h"
class CCursor_bu : public CScript
{
private:
	vector<TCursorInfo> m_vecCursorInfo;
	int m_iCurIdx; // 현재 커서의 인덱스


	void AddCursor(const TCursorInfo& _tCursorInfo) { m_vecCursorInfo.push_back(_tCursorInfo); }
	vector<TCursorInfo>& GetCursors() { return m_vecCursorInfo; }


public:
	virtual void Start() override;

public:
	CLONE(CCursor_bu);
	CCursor_bu();
	virtual ~CCursor_bu() override;
};