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

	// DX라이브러리 내부에서 문제가 있을 경우를 알림받기위해 설정해준다.
	// 릴리즈에서는 플래그를 넣어주면 안된다.
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
	

	// 스왑체인 생성
	bIsFail = FAILED(CreateSwapChain()); 

	if (bIsFail) {
		MessageBox(nullptr, STR_MSG_FailSwapChain, STR_MSG_OccurError, MB_OK);
		return E_FAIL;
	}
	////////////////////////////////////////
	// Render Target View, DepthStencilView
	///////////////////////////////////////
	// 뷰 생성
	if (FAILED(CreateView())) {
		MessageBox(nullptr, STR_MSG_FailCreateView, STR_MSG_OccurError, MB_OK);
		return E_FAIL;
	}

	// Viewport 설정
	CreateViewport();

	// ConstBuffer 생성
	CreateConstBuffer();

	// Sampler 생성
	CreateSampler();

	// Rasterizer State 생성
	CreateRasterizerState();

	// Blend, DepthStencil State 생성
	CreateBlendState();
	CreateDepthStencilState();

	return S_OK;
}

int CDevice::CreateSwapChain() {

#pragma region 스왑체인 구조체 설정
	DXGI_SWAP_CHAIN_DESC tDesc = {};

	// 렌더링 해상도
	tDesc.BufferDesc.Width = (UINT)GetRenderResolution().x;
	tDesc.BufferDesc.Height = (UINT)GetRenderResolution().y;

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
	// RenderTargetView 만들기
	/////////////////////////
	// 1. SwapChain으로부터 출력 버퍼 열기
	ComPtr<ID3D11Texture2D> pTex = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)pTex.GetAddressOf());

	// 2. 해당 버퍼를 CTexture로 전환해서 ResourceManager에 등록
	m_pRTTex = CResourceManager::GetInstance()->CreateTexture(STR_ResourceKey_RTTexture, pTex);

	//////////////////////////
	// DepthStencilView 만들기
	//////////////////////////
	// DepthStencil용 Texture2D 만들기
	m_pDSTex = CResourceManager::GetInstance()->CreateTexture(STR_ResourceKey_DSVTexture, (UINT)m_vRenderResolution.x, (UINT)m_vRenderResolution.y, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL);

	//////////////////////////
	// 출력 타겟 및 깊이버퍼 설정
	//////////////////////////
	m_pContext->OMSetRenderTargets(1, m_pRTTex->GetRTV().GetAddressOf(), m_pDSTex->GetDSV().Get()); // OM : output manager

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

	m_arrCB[(UINT)E_ConstBuffer::RectTransform] = new CConstBuffer;
	m_arrCB[(UINT)E_ConstBuffer::RectTransform]->Create(E_ConstBuffer::RectTransform, sizeof(TRectTransform));
}

void CDevice::CreateSampler()
{
	D3D11_SAMPLER_DESC tDesc = {};
	tDesc.Filter = D3D11_FILTER_ANISOTROPIC; // 이방성 필터링
	tDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	tDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	DEVICE->CreateSamplerState(&tDesc, m_pSamplerStates[0].GetAddressOf());

	tDesc = {}; // 확대 시 픽셀의 각진 느낌 그대로 유지
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
	// default는 D3D11_CULL_BACK으로 되어있으므로 nullptr로 줌.
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
	tDesc.IndependentBlendEnable = false; // false 설정 시 RenderTarget은 [0] 멤버만 설정

	// 0번 인덱스 하나만 설정하면 다 설정됨?
	tDesc.RenderTarget[0].BlendEnable = true;

	tDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	tDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; // 가산혼합
	tDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	tDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	tDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA; // 가산혼합
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
	// Less (기본)
	m_pDepthStencilStates[(UINT)E_DepthStencilState::Less] = nullptr;


	D3D11_DEPTH_STENCIL_DESC tDesc = {};

	// Less Equal;
	tDesc.DepthEnable = true;
	tDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	tDesc.StencilEnable = false;
	DEVICE->CreateDepthStencilState(&tDesc, m_pDepthStencilStates[(UINT)E_DepthStencilState::Less_Equal].GetAddressOf());

	// StencilEnable이 True일 경우 사용
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
	// ImGui에 의해 추가된 윈도우 쪽으로 렌더타겟이 바뀌게 되므로 출력 타겟을 다시 설정
	// 출력 타겟 및 깊이버퍼 설정
	//////////////////////////
	m_pContext->OMSetRenderTargets(1, m_pRTTex->GetRTV().GetAddressOf(), m_pDSTex->GetDSV().Get()); // OM : output manager


	float fArrColor[4] = { 0.1f, 0.1f, 0.1f, 1.f}; // 색상 (black)
	m_pContext->ClearRenderTargetView(m_pRTTex->GetRTV().Get(), fArrColor);

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