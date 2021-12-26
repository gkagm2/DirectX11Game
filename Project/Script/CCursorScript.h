#pragma once
#include <Engine\CScript.h>


struct TCursorInfo {
	SharedPtr<CTexture> pCursorTex;
	Vector3 vScale;
	Vector3 vOffset;
};

// ��ӹ޾� ����ϱ�
class CCursorScript : public CScript
{
	vector<TCursorInfo> m_vecCursorInfo;
	int m_iCurIdx; // ���� Ŀ���� �ε���

	SharedPtr<CMaterial> m_pCursorMtrl;

public:
	virtual void Awake() override;
	virtual void LateUpdate() override;

	void AddCursor(const TCursorInfo& _tCursorInfo) { m_vecCursorInfo.push_back(_tCursorInfo); }
	vector<TCursorInfo>& GetCursors() { return m_vecCursorInfo; }

	void SetCurCursor(int _iCursorIdx);

public:
	CLONE(CCursorScript);
	CCursorScript();
	CCursorScript(SCRIPT_TYPE _eType);
	virtual ~CCursorScript() override;
};