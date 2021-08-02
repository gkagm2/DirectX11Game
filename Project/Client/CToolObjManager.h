#pragma once
class CGameObjectEx;
class CToolObjManager : public CSingleton<CToolObjManager>
{
	SINGLETON(CToolObjManager)
private:
	vector<CGameObjectEx*> m_vecToolObjs;

private:
	void _AddGameObjectEx(CGameObjectEx* _pObjEx) { m_vecToolObjs.push_back(_pObjEx); }

public:
	void Init();
	void Progress();
};