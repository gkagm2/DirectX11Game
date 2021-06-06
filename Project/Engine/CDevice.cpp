#include "pch.h"
#include "CDevice.h"
#include "CCore.h"
#include "CConstBuffer.h"

CDevice::CDevice() :
	m_arrCB{},
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
	m_tViewPort{}
{
}

CDevice::~CDevice() {
	Safe_Delete_Array(m_arrCB);
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
	iFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//D3D_DRIVER_TYPE eDriverTypes[] = {
	//	D3D_DRIVER_TYPE_HARDWARE,
	//	D3D_DRIVER_TYPE_WARP,
	//	D3D_DRIVER_TYPE_REFERENCE
	//};
	//UINT iNumDriverTypes = ARRAYSIZE(eDriverTypes);

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
	

	// ����ü�� ����
	bIsFail = FAILED(CreateSwapChain()); 

	if (bIsFail) {
		MessageBox(nullptr, STR_MSG_FailSwapChain, STR_MSG_OccurError, MB_OK);
		return E_FAIL;
	}
	////////////////////////////////////////
	// Render Target View, DepthStencilView
	///////////////////////////////////////
	// �� ����
	if (FAILED(CreateView())) {
		MessageBox(nullptr, STR_MSG_FailCreateView, STR_MSG_OccurError, MB_OK);
		return E_FAIL;
	}

	// Viewport ����
	CreateViewport();
	// SamplerState ����


	// ConstBuffer ����
	CreateConstBuffer();

	// Sampler ����
	CreateSampler();

	// Rasterizer State ����
	CreateRasterizerState();

	return S_OK;
}

int CDevice::CreateSwapChain() {

#pragma region ����ü�� ����ü ����
	DXGI_SWAP_CHAIN_DESC tDesc = {};

	// ������ �ػ�
	tDesc.BufferDesc.Width = (UINT)GetRenderResolution().x;
	tDesc.BufferDesc.Height = (UINT)GetRenderResolution().y;

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

	// �� �÷��׸� ����Ͽ� ��Ʈ ���� ���� (bitblt) ���� �����ϰ� DXGI�� IDXGISwapChain1 :: Present1 �� ȣ�� �� �� �� ������ ������ �����ϵ��� ����
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

	if (FAILED(pFactory->CreateSwapChain(m_pDevice.Get(), &tDesc, m_pSwapChain.GetAddressOf()))) {
		MessageBox(nullptr, STR_MSG_FailCreateSwapChain, STR_MSG_FailDeviceInitializing, MB_OK);
		return E_FAIL;
	}
#pragma endregion

	return S_OK;
}

int CDevice::CreateView()
{
	//////////////////////////
	// RenderTargetView �����
	/////////////////////////
	// 1. SwapChain���κ��� ��� ���� ����
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)m_pRTTex.GetAddressOf());
	// 2. ��� ���۷� RenderTargetView Create
	m_pDevice->CreateRenderTargetView(m_pRTTex.Get(), 0, m_pRTV.GetAddressOf());

	//////////////////////////
	// DepthStencilView �����
	//////////////////////////
	// DepthStencil�� Texture2D �����
	D3D11_TEXTURE2D_DESC tTexDesc = {};
	tTexDesc.Width = (UINT)m_vRenderResolution.x;
	tTexDesc.Height = (UINT)m_vRenderResolution.y;
	tTexDesc.MipLevels = 1; // ���� �ϳ��� ����
	tTexDesc.ArraySize = 1;
	tTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // �ȼ�����

	tTexDesc.SampleDesc.Count = 1;
	tTexDesc.SampleDesc.Quality = 0;

	tTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tTexDesc.Usage = D3D11_USAGE_DEFAULT;
	//tTexDesc.CPUAccessFlags;
	//tTexDesc.MiscFlags;

	m_pDevice->CreateTexture2D(&tTexDesc, nullptr, m_pDSTex.GetAddressOf()); // 2param : �ʱ� ���� �ֱ�

	// 2. ������ DepthStencil Texture�� DepthStencilView�� �����Ѵ�.
	m_pDevice->CreateDepthStencilView(m_pDSTex.Get(), 0, m_pDSV.GetAddressOf());

	// ��� Ÿ�� �� ���̹��� ����
	m_pContext->OMSetRenderTargets(1, m_pRTV.GetAddressOf(), m_pDSV.Get()); // OM : output manager

	return S_OK;
}

void CDevice::CreateViewport()
{
	m_tViewPort.TopLeftX = 0.f;
	m_tViewPort.TopLeftY = 0.f;
	m_tViewPort.Width = CCore::GetInstance()->GetWindowResolution().x;
	m_tViewPort.Height = CCore::GetInstance()->GetWindowResolution().y;
	m_tViewPort.MinDepth = 0.f;
	m_tViewPort.MaxDepth = 1.f;

	m_pContext->RSSetViewports(1, &m_tViewPort); // RS : Rasterizer stage
}

void CDevice::CreateConstBuffer()
{
	m_arrCB[(UINT)E_ConstBuffer::transform] = new CConstBuffer;
	m_arrCB[(UINT)E_ConstBuffer::transform]->Create(E_ConstBuffer::transform, sizeof(TTransform));
}

void CDevice::CreateSampler()
{
	D3D11_SAMPLER_DESC tDesc = {};
	tDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	tDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	DEVICE->CreateSamplerState(&tDesc, m_pSamplerStates[0].GetAddressOf());

	tDesc = {}; // Ȯ�� �� �ȼ��� ���� ���� �״�� ����
	tDesc.Filter = D3D11_FILTER_MINIMUM_MIN_MAG_MIP_POINT;
	tDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	DEVICE->CreateSamplerState(&tDesc, m_pSamplerStates[1].GetAddressOf());

	ID3D11SamplerState* arrSamplerState[2] = { m_pSamplerStates[0].Get(), m_pSamplerStates[1].Get() };

	CONTEXT->VSSetSamplers(0, 2, arrSamplerState);
	CONTEXT->HSSetSamplers(0, 2, arrSamplerState);
	CONTEXT->DSSetSamplers(0, 2, arrSamplerState);
	CONTEXT->GSSetSamplers(0, 2, arrSamplerState);
	CONTEXT->PSSetSamplers(0, 2, arrSamplerState);
	CONTEXT->CSSetSamplers(0, 2, arrSamplerState); // compute shader������ 
}

void CDevice::CreateRasterizerState()
{
	// default�� D3D11_CULL_BACK���� �Ǿ������Ƿ� nullptr�� ��.
	m_pRasterizerStates[(UINT)E_RasterizerState::CullBack] = nullptr;

	D3D11_RASTERIZER_DESC tDesc = {};
	tDesc.FillMode = D3D11_FILL_SOLID;
	tDesc.CullMode = D3D11_CULL_FRONT;
	DEVICE->CreateRasterizerState(&tDesc, m_pRasterizerStates[(UINT)E_RasterizerState::CullFront].GetAddressOf());
	
	tDesc.FillMode = D3D11_FILL_SOLID;
	tDesc.CullMode = D3D11_CULL_NONE;
	DEVICE->CreateRasterizerState(&tDesc, m_pRasterizerStates[(UINT)E_RasterizerState::CullNone].GetAddressOf());

	tDesc.FillMode = D3D11_FILL_WIREFRAME;
	tDesc.CullMode = D3D11_CULL_NONE;
	DEVICE->CreateRasterizerState(&tDesc, m_pRasterizerStates[(UINT)E_RasterizerState::Wireframe].GetAddressOf());
}

void CDevice::SetRasterizerState(E_RasterizerState _eRasterizerState)
{
	CONTEXT->RSSetState(m_pRasterizerStates[(UINT)_eRasterizerState].Get());
}

void CDevice::ClearTarget()
{
	float fArr[4] = { 0.f, 0.5f, 0.f, 1.f}; // black color
	m_pContext->ClearRenderTargetView(m_pRTV.Get(), fArr);

	float fDepth = 1.0f;
	UINT8 iStencil = 0;
	m_pContext->ClearDepthStencilView(m_pDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, fDepth, iStencil);
}

void CDevice::Present()
{
	int iSyncInterval = 0;
	int iFlags = 0;
	m_pSwapChain->Present(iSyncInterval, iFlags);
}