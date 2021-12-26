#pragma once

class ClientManager : public CSingleton<ClientManager>
{
	SINGLETON(ClientManager);
private:
	vector<CTexture*> m_vecTex;
	class CTexture* m_pCurCursorTex;
	class CGameObjectEx* m_pCursor;
	class CGameObjectEx* m_pCanvas;
	class CGameObjectEx* m_pCam;

	vector<CGameObjectEx*> m_vecObjs;

public:
	void SetCurCursorTex(UINT _idx);

public:
	void Init();
	void Progress();

private:
	void Init_Butcher();

private:
	void _AddGameObjectEx(CGameObjectEx* _pObj) { m_vecObjs.push_back(_pObj);}
};