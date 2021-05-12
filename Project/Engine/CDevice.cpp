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

	// ��Ʈ���� ��� üũ
	UINT iLevel = 0;
	m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &iLevel);
	if (0 == iLevel) {
		MessageBox(nullptr, STR_MSG_MultiSampleSupportNotAvailable, STR_MSG_OccurError, MB_OK);
		return E_FAIL;
	}
	
	bIsFail = FAILED(CreateSwapChain()); // ����ü�� ����

	if (bIsFail) {
		MessageBox(nullptr, STR_MSG_FailSwapChain, STR_MSG_OccurError, MB_OK);
		return E_FAIL;
	}

	// Render Target View, DepthStencilView
	

	// Viewport ����

	// SamplerState ����
		
	return S_OK;
}

int CDevice::CreateSwapChain() {

#pragma region ����ü�� ����ü ����
	DXGI_SWAP_CHAIN_DESC tDesc = {};

	// ������ �ػ�
	tDesc.BufferDesc.Width = (UINT)_GetRenderResolution().x;
	tDesc.BufferDesc.Height = (UINT)_GetRenderResolution().y;

	// ����� ������� (�츣��)
	tDesc.BufferDesc.RefreshRate = _GetDefaultRefreshRate();

	// �ȼ� ���� (�ؽ��� ���� �ȼ� �ϳ����� ���� ������ ���Ѵ�.)
	tDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // RGBA 32bit (��ȣ���� ���� ����ȭ)

	// ����� �������� �� ��µǴ� ���
	tDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;  // ���÷��� ��ĵ���� ��� (Default)

	tDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; // (Default)  

	// tDesc.Flags;

	// ���� ��� �뵵 (���� Ÿ������ ����)
	tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; 
	// 11 Version������ 1�������� ���������� ������ �� ���� ���� ���۸����� ����ȴ�. 12 Version������ 2�� ������� ��.
	tDesc.BufferCount = _GetBufferCnt();
		
	// ���ۿ� �������� ����� ��¹��� ������ �ڵ�
	tDesc.OutputWindow = m_hWnd; 

	// â��� ����
	tDesc.Windowed = m_bWindowMode;

	// �� �÷��׸� ����Ͽ� ��Ʈ ��� ���� (bitblt) ���� �����ϰ� DXGI�� IDXGISwapChain1 :: Present1 �� ȣ�� �� �� �� ������ ������ �����ϵ��� ����
	tDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; 

	// ��Ƽ���� ����
	tDesc.SampleDesc.Count = 1; //1�̶� �ǹ̴� ���� �ȼ� ������ �״�� �������ڴٴ� ����.
	tDesc.SampleDesc.Quality = 0;
#pragma endregion

#pragma region Swap Cahin ���� ��ü(DXGIFactory) ���
	ComPtr<IDXGIDevice> pDevice = nullptr;
	ComPtr<IDXGIAdapter> pAdapter = nullptr;
	ComPtr<IDXGIFactory> pFactory = nullptr; // ����ü�� ���� ��ü

	// Ư�� Ÿ���� ���� ��ġġ �ʴ� ������ ID���� �ο��Ѵ�.
	/*
	ù��° �Ķ���Ϳ��ִ� Ŭ������ �ο��Ǿ��ִ� ���� ID����  �̿��Ͽ� IDXGIDevice Ÿ���� ��ü�� pDevice�� �ּ����·� �����´�.
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
	// RenderTargetView �����
	// Stencil

	return S_OK;
}
