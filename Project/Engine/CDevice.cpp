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
	// DX라이브러리 내부에서 문제가 있을 경우를 알림받기위해 설정해준다.
	// 릴리즈에서는 플래그를 넣어주면 안된다.
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
		&eFeatureLevel, // 지정한 Feature Level로 만든다.
		&m_pContext));

	if (bIsFail) {
		MessageBox(nullptr, STR_MSG_FailedToCreateDeviceObject, STR_MSG_OccurError, MB_OK);
		return E_FAIL;
	}
		
	return S_OK;
}