#include "pch.h"
#include "CDevice.h"

CDevice::CDevice() :
	m_hWnd(nullptr),
	m_bWindowMode(false)
{
}

CDevice::~CDevice() {

}

int CDevice::Init(HWND _hOutputWnd, bool _bWindowMode)
{
	// DX���̺귯�� ���ο��� ������ ���� ��츦 �˸��ޱ����� �������ش�.
	// ��������� �÷��׸� �־��ָ� �ȵȴ�.
	UINT iFlag = 0;
#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif
	
	D3D_FEATURE_LEVEL eFeatureLevel = D3D_FEATURE_LEVEL_11_0;
	int bIsFail = FAILED(D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		iFlag,
		0, // FeatureLevels
		0,
		D3D11_SDK_VERSION,
		&m_pDevice,
		&eFeatureLevel, // ������ Feature Level�� �����.
		&m_pContext));

	if (bIsFail) {
		MessageBox(nullptr, STR_MSG_FailedToCreateDeviceObject, STR_MSG_OccurError, MB_OK);
		return E_FAIL;
	}
		
	return S_OK;
}