#include "pch.h"
#include "CameraGUI.h"
#include <Engine\CCamera.h>
#include <Engine\CGameObject.h>

CameraGUI::CameraGUI() :
	ComponentGUI(E_ComponentType::Camera)
{
	_InitComboBox();
}

CameraGUI::~CameraGUI()
{
}

void CameraGUI::Update()
{
	if (false == Start())
		return;

	CCamera* pCam = GetTargetObject()->Camera();

	E_ProjectionType eCamType = pCam->GetProjectionType();


	pCam->SetProjectionType(eCamType);

	// ī�޶� Ÿ��
	string strCameraType;
	TStringToString(CameraProjectionTypeToStr(eCamType), strCameraType);
	ImGui::Text("State : %s", strCameraType.c_str());

	// �޺��� ǥ���ϱ�
	static int iCurItem = (int)eCamType;
	ImGui::Combo("Camera Type", &iCurItem, m_strList.data(), m_strList.size());
	eCamType = (E_ProjectionType)iCurItem;

	pCam->SetProjectionType(eCamType);


	End();
}

void CameraGUI::_InitComboBox()
{
	// Combo Box�� ����Ʈ ����
	string strState;
	for (UINT i = 0; i < PROJECTION_TYPE_COUNT; ++i) {
		TStringToString(CameraProjectionTypeToStr((E_ProjectionType)i), strState);
		for (UINT j = 0; j < strState.size(); ++j)
			m_strList.push_back(strState[j]);
		m_strList.push_back('\0');
	}
	m_strList.push_back('\0');
}