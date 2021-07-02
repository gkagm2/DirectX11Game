#include "pch.h"
#include "CDevice.h"
#include "CCore.h"
#include "CConstBuffer.h"
#include "CResourceManager.h"

CDevice::CDevice() :
	m_arrCB{},
	m_pSamplerStates{},
	m_pRasterizerStates{},
	m_pBlendStates{},
	m_pDepthStencilStates{},
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

	g_globalConst.vResolution = m_vRenderResolution;

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

	// ConstBuffer ����
	CreateConstBuffer();

	// Sampler ����
	CreateSampler();

	// Rasterizer State ����
	CreateRasterizerState();

	// Blend, DepthStencil State ����
	CreateBlendState();
	CreateDepthStencilState();

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
	m_pDSTex = CResourceManager::GetInstance()->CreateTexture(STR_ResourceKey_DSVTexture, (UINT)m_vRenderResolution.x, (UINT)m_vRenderResolution.y, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL);

	//////////////////////////
	// ��� Ÿ�� �� ���̹��� ����
	//////////////////////////
	m_pContext->OMSetRenderTargets(1, m_pRTV.GetAddressOf(), m_pDSTex->GetDSV().Get()); // OM : output manager

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
	m_arrCB[(UINT)E_ConstBuffer::Transform] = new CConstBuffer;
	m_arrCB[(UINT)E_ConstBuffer::Transform]->Create(E_ConstBuffer::Transform, sizeof(TTransform));
	
	m_arrCB[(UINT)E_ConstBuffer::Material_Param] = new CConstBuffer;
	m_arrCB[(UINT)E_ConstBuffer::Material_Param]->Create(E_ConstBuffer::Material_Param, sizeof(TMaterialParam));

	m_arrCB[(UINT)E_ConstBuffer::Animation2D_Data] = new CConstBuffer;
	m_arrCB[(UINT)E_ConstBuffer::Animation2D_Data]->Create(E_ConstBuffer::Animation2D_Data, sizeof(TAnimation2DData));

	m_arrCB[(UINT)E_ConstBuffer::Global] = new CConstBuffer;
	m_arrCB[(UINT)E_ConstBuffer::Global]->Create(E_ConstBuffer::Global, sizeof(TGlobalConst));
}

void CDevice::CreateSampler()
{
	D3D11_SAMPLER_DESC tDesc = {};
	tDesc.Filter = D3D11_FILTER_ANISOTROPIC; // �̹漺 ���͸�
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
	CONTEXT->CSSetSamplers(0, 2, arrSamplerState);
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

void CDevice::CreateBlendState()
{
	D3D11_BLEND_DESC tDesc = {};
	tDesc.AlphaToCoverageEnable = false;
	tDesc.IndependentBlendEnable = false; // false ���� �� RenderTarget�� [0] ����� ����

	// 0�� �ε��� �ϳ��� �����ϸ� �� ������?
	tDesc.RenderTarget[0].BlendEnable = true;

	tDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	tDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; // ����ȥ��
	tDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	tDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	tDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA; // ����ȥ��
	tDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

	tDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	// AlphaBlend
	DEVICE->CreateBlendState(&tDesc, m_pBlendStates[(UINT)E_BlendState::AlphaBlend].GetAddressOf());
	CONTEXT->OMSetBlendState(m_pBlendStates[(UINT)E_BlendState::AlphaBlend].Get(), Vector4(0.f,0.f,0.f,0.f), 0xffffffff);

	// AlphaBlend Coverage
	tDesc.AlphaToCoverageEnable = true;
	DEVICE->CreateBlendState(&tDesc, m_pBlendStates[(UINT)E_BlendState::AlphaBlend_Coverage].GetAddressOf());
	CONTEXT->OMSetBlendState(m_pBlendStates[(UINT)E_BlendState::AlphaBlend_Coverage].Get(), Vector4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
}

void CDevice::CreateDepthStencilState()
{
	// Less (�⺻)
	m_pDepthStencilStates[(UINT)E_DepthStencilState::Less] = nullptr;


	D3D11_DEPTH_STENCIL_DESC tDesc = {};

	// Less Equal;
	tDesc.DepthEnable = true;
	tDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	tDesc.StencilEnable = false;
	DEVICE->CreateDepthStencilState(&tDesc, m_pDepthStencilStates[(UINT)E_DepthStencilState::Less_Equal].GetAddressOf());

	// StencilEnable�� True�� ��� ���
	/*tDesc.BackFace;
	tDesc.FrontFace;
	tDesc.StencilReadMask;
	tDesc.StencilWriteMask;*/
	

	// Greater
	tDesc = {};
	tDesc.DepthEnable = true;
	tDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	tDesc.StencilEnable = false;
	DEVICE->CreateDepthStencilState(&tDesc, m_pDepthStencilStates[(UINT)E_DepthStencilState::Greater].GetAddressOf());

	// No Test
	tDesc = {};
	tDesc.DepthEnable = true;
	tDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	tDesc.StencilEnable = false;
	DEVICE->CreateDepthStencilState(&tDesc, m_pDepthStencilStates[(UINT)E_DepthStencilState::No_Test].GetAddressOf());

	// No Write
	tDesc = {};
	tDesc.DepthEnable = true;
	tDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
	tDesc.StencilEnable = false;
	DEVICE->CreateDepthStencilState(&tDesc, m_pDepthStencilStates[(UINT)E_DepthStencilState::No_Write].GetAddressOf());

	// No Test No Write
	tDesc = {};
	tDesc.DepthEnable = false;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
	tDesc.StencilEnable = false;
	DEVICE->CreateDepthStencilState(&tDesc, m_pDepthStencilStates[(UINT)E_DepthStencilState::No_Test_No_Write].GetAddressOf());
}

void CDevice::ClearTarget()
{
	float fArrColor[4] = { 0.2f, 0.3f, 0.2f, 1.f}; // ����
	m_pContext->ClearRenderTargetView(m_pRTV.Get(), fArrColor);

	float fDepth = 1.0f;
	UINT8 iStencil = 0;
	m_pContext->ClearDepthStencilView(m_pDSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, fDepth, iStencil);
}

void CDevice::Present()
{
	int iSyncInterval = 0;
	int iFlags = 0;
	m_pSwapChain->Present(iSyncInterval, iFlags);
}