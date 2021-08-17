#pragma once
class CDebugManager : CSingleton<CDebugManager>
{
	SINGLETON(CDebugManager)
private:
	vector<char*> m_vecEvent;

public:
	void Update();
public:
	void AddLog(char* _log);
	void ClearLog();
public:
	class CCore;
};