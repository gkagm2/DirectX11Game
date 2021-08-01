#include "pch.h"
#include "CGameObjectEx.h"
#include <Engine\CComponent.h>

CGameObjectEx::CGameObjectEx()
{
}

CGameObjectEx::~CGameObjectEx()
{
}

void CGameObjectEx::FinalUpdate()
{
	for (UINT i = 0; i < (UINT)E_ComponentType::End; ++i) {
		CComponent* pComponent = GetComponent((E_ComponentType)i);
		if (nullptr != pComponent)
			pComponent->FinalUpdate();
	}

	const vector<CGameObject*>& vecChilds = GetChildsObject();
	for (UINT i = 0; i < vecChilds.size(); ++i)
		vecChilds[i]->FinalUpdate();
}