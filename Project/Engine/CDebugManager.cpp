#include "pch.h"
#include "CDebugManager.h"

void CDebugManager::Update()
{
}

void CDebugManager::AddLog(char* _log, ...)
{
	m_vecEvent.push_back(_log);
	
}

void CDebugManager::ClearLog()
{
	m_vecEvent.clear();
}