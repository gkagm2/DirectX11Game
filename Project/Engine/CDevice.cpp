#include "pch.h"
#include "CDevice.h"

CDevice::CDevice() :
	m_tDefaultRefreshRate{ 60,1 }, // 60 / 
	m_iBufferCnt{ 1 },
	m_vRenderResolution{},
	m_hWnd(nullptr),
	m_bWindowMode(false),
	m_pDevice(nullptr),
	m_pContext(nullptr),
	m_pSwapChain(nullptr),
	m_pRTTex(nullptr),
	m_pRTV(nullptr),
	m_pDSV(nullptr),
	m_pDSTex(nullptr),
	m_pSample(nullptr)
{
}

CDevice::~CDevice() {

}

int CDevice::Init(HWND _hOutputWnd, const Vector2& _vRenderResolution, bool _bWindowMode)
{
	m_hWnd = _hOutputWnd;
	m_vRenderResolution = _vRenderResolution;
	m_bWindowMode = _bWindowMode;

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

	// 노트북일 경우 체크
	UINT iLevel = 0;
	m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &iLevel);
	if (0 == iLevel) {
		MessageBox(nullptr, STR_MSG_MultiSampleSupportNotAvailable, STR_MSG_OccurError, MB_OK);
		return E_FAIL;
	}
	
	bIsFail = FAILED(CreateSwapChain()); // 스왑체인 생성

	if (bIsFail) {
		MessageBox(nullptr, STR_MSG_FailSwapChain, STR_MSG_OccurError, MB_OK);
		return E_FAIL;
	}

	// Render Target View, DepthStencilView
	

	// Viewport 설정

	// SamplerState 생성
		
	return S_OK;
}

int CDevice::CreateSwapChain() {

#pragma region 스왑체인 구조체 설정
	DXGI_SWAP_CHAIN_DESC tDesc = {};

	// 렌더링 해상도
	tDesc.BufferDesc.Width = (UINT)_GetRenderResolution().x;
	tDesc.BufferDesc.Height = (UINT)_GetRenderResolution().y;

	// 모니터 재생비율 (헤르츠)
	tDesc.BufferDesc.RefreshRate = _GetDefaultRefreshRate();

	// 픽셀 포멧 (텍스쳐 안의 픽셀 하나에에 대한 포멧을 말한다.)
	tDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // RGBA 32bit (부호없는 정수 정규화)

	// 모니터 렌더링될 때 출력되는 방식
	tDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;  // 디스플레이 스캔라인 모드 (Default)

	tDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; // (Default)  

	// tDesc.Flags;

	// 버퍼 사용 용도 (렌터 타겟으로 설정)
	tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; 
	// 11 Version에서는 1로적지만 내부적으로 렌더링 할 때는 더블 버퍼링으로 적용된다. 12 Version에서는 2로 적어줘야 함.
	tDesc.BufferCount = _GetBufferCnt();
		
	// 버퍼에 렌더링될 장면을 출력받을 윈도우 핸들
	tDesc.OutputWindow = m_hWnd; 

	// 창모드 설정
	tDesc.Windowed = m_bWindowMode;

	// 이 플래그를 사용하여 비트 블록 전송 (bitblt) 모델을 지정하고 DXGI가 IDXGISwapChain1 :: Present1 을 호출 한 후 백 버퍼의 내용을 삭제하도록 지정
	tDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; 

	// 멀티샘플 설정
	tDesc.SampleDesc.Count = 1; //1이란 의미는 찍은 픽셀 색상을 그대로 가져오겠다는 것임.
	tDesc.SampleDesc.Quality = 0;
#pragma endregion

#pragma region Swap Cahin 생성 객체(DXGIFactory) 얻기
	ComPtr<IDXGIDevice> pDevice = nullptr;
	ComPtr<IDXGIAdapter> pAdapter = nullptr;
	ComPtr<IDXGIFactory> pFactory = nullptr; // 스왑체인 생성 객체

	// 특정 타입의 절대 겹치치 않는 고유한 ID값을 부여한다.
	/*
	첫번째 파라미터에있는 클래스에 부여되어있는 고유 ID값을  이용하여 IDXGIDevice 타입의 객체를 pDevice로 주소형태로 가져온다.
	*/
	m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDevice);
	pDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pAdapter);
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);

	if (FAILED(pFactory->CreateSwapChain(m_pDevice, &tDesc, &m_pSwapChain))) {
		MessageBox(nullptr, STR_MSG_FailCreateSwapChain, STR_MSG_FailDeviceInitializing, MB_OK);
		return E_FAIL;
	}
#pragma endregion

	return S_OK;
}

int CDevice::CreateView()
{
	// RenderTargetView 만들기
	// Stencil

	return S_OK;
}
