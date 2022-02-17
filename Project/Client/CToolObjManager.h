#pragma once
class CGameObjectEx;
class CToolObjManager : public CSingleton<CToolObjManager>
{
	SINGLETON(CToolObjManager)
private:
	vector<CGameObjectEx*> m_vecToolObjs;

	map<tstring, CGraphicsShader*>  m_mapShader;
	map<tstring, CMaterial*>        m_mapMtrl;

private:
	void _AddGameObjectEx(CGameObjectEx* _pObjEx) { m_vecToolObjs.push_back(_pObjEx); }
	void _CreateToolMaterial();
	void _CreateToolObject();

public:
	void Init();
	void Progress();
};