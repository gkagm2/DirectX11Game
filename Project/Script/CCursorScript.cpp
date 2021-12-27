#include "pch.h"
#include "CCursorScript.h"
#include <Engine\CObjectManager.h>

CCursorScript::CCursorScript() :
	CScript((UINT)SCRIPT_TYPE::CURSORSCRIPT),
	m_iCurIdx(0)
{
	assert(nullptr && _T("��ӹ޾� ����ϱ�"));
}

CCursorScript::CCursorScript(SCRIPT_TYPE _eType) :
	CScript((UINT)_eType),
	m_iCurIdx(0)
{
}

CCursorScript::~CCursorScript()
{
}

void CCursorScript::Awake()
{
	if (!CanvasRenderer()) {
		GetGameObject()->AddComponent<CCanvasRenderer>();
	}
	
	// �ڽ� ������Ʈ�� �ֱ�
	CGameObject* pImageUI = CObjectManager::GetInstance()->CreateImageUI();

	//CGameObject* pUICamera = CSceneManager::GetInstance()->GetCurScene()->FindGameObject(STR_OBJ_NAME_UICamera, NUM_LAYER_UI);

	//// UI ī�޶� ������ ���� �����Ѵ�.
	//if (!pUICamera)
	//	pUICamera = _CreateDefaultUICamera();

	//CGameObject* pUICanvas = FIND_GameObject_Layer(STR_OBJ_NAME_UICanvas, NUM_LAYER_UI);
	//if (!pUICanvas)
	//	pUICanvas = _CreateCanvas();
	//CObject::CreateGameObjectEvn(pImageUI, NUM_LAYER_UI);
	//CObject::AddChildGameObjectEvn(pUICanvas, pImageUI);
}

void CCursorScript::LateUpdate()
{
	CCamera* pCam = CRenderManager::GetInstance()->GetMainCamera();
	if (!pCam)
		return;
	if (E_ProjectionType::Orthographic == pCam->GetProjectionType()) {
		const Vector2& vMousePos = MousePosition;
		Vector3 vWorldPos = pCam->GetScreenToWorld2DPosition(vMousePos);
		vWorldPos.z += 10.f;

		RectTransform()->SetLocalPosition(vWorldPos);
	}
}

void CCursorScript::SetCurCursor(int _iCursorIdx)
{
	m_iCurIdx = _iCursorIdx;
	if (m_vecCursorInfo.size() > 0 && m_iCurIdx < m_vecCursorInfo.size()) {
		const TCursorInfo& info = m_vecCursorInfo[m_iCurIdx];
		// Material���ٰ� �ֱ�
		m_pCursorMtrl->SetData(E_ShaderParam::Texture_0, m_vecCursorInfo[m_iCurIdx].pCursorTex.Get());
	}
}