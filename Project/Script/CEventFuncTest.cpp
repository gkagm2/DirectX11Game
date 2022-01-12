#include "pch.h"
#include "CEventFuncTest.h"

void CEventFuncTest::Awake()
{
	_tcprintf(_T("%s[Awake]\n"), GetGameObject()->GetName().c_str());
}

void CEventFuncTest::Start()
{
	_tcprintf(_T("%s[Start]\n"), GetGameObject()->GetName().c_str());
}

void CEventFuncTest::OnEnable()
{
	_tcprintf(_T("%s[OnEnable]\n"), GetGameObject()->GetName().c_str());
}

void CEventFuncTest::OnDisable()
{
	_tcprintf(_T("%s[OnDisable]\n"), GetGameObject()->GetName().c_str());
}

void CEventFuncTest::OnDestroy()
{
	_tcprintf(_T("%s[OnDestroy]\n"), GetGameObject()->GetName().c_str());
}