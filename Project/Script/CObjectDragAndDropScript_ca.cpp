#include "pch.h"
#include "CObjectDragAndDropScript_ca.h"

CObjectDragAndDropScript_ca::CObjectDragAndDropScript_ca() :
	CScript((UINT)SCRIPT_TYPE::OBJECTDRAGANDDROPSCRIPT_CA)
{
}

CObjectDragAndDropScript_ca::~CObjectDragAndDropScript_ca()
{
}

void CObjectDragAndDropScript_ca::Start()
{
}

void CObjectDragAndDropScript_ca::Update()
{
	// 오브젝트를 클릭했을 때
	/*if (InputKeyPress(E_Key::LBUTTON)) {
		
	}*/

	// Test Code
	if (InputKeyHold(E_Key::LBUTTON)) {
		Vector2 vPos = (Vector2)Transform()->GetPosition();
		vPos = MousePosition;
		Transform()->SetLocalPosition(Vector3(vPos.x ,vPos.y, 0.f));
	}
}

bool CObjectDragAndDropScript_ca::SaveToScene(FILE* _pFile)
{
	return false;
}

bool CObjectDragAndDropScript_ca::LoadFromScene(FILE* _pFile)
{
	return false;
}
