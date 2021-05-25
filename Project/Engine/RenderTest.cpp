#include "pch.h"
#include "RenderTest.h"
#include "CDevice.h"
#include "CPathManager.h"

// Tip : (Blob : 문자열 데이터 메모리 덩어리)
ComPtr<ID3D11Buffer> g_pVB;

ComPtr<ID3D11VertexShader> g_VS; // 버텍스 쉐이더
ComPtr<ID3DBlob> g_VSBlob;		 // 베텍스 쉐이더 컴파일 코드

ComPtr<ID3D11PixelShader> g_PS;  // 픽셀 쉐이더
ComPtr<ID3DBlob> g_PSBlob;		 // 픽셀 쉐이더 컴파일 코드

ComPtr<ID3DBlob> g_ErrBlob;		 // 에러 메세지 저장 용도


void Render_Test::TestInit()
{
	// Triangle
	VTX arr[3] = {};
	arr[0].vPos = Vector3(-1.f, -1.f, 0.5f);
	arr[0].vColor = Vector4{ 1.f,1.f,1.f,1.f };

	arr[1].vPos = Vector3(0.f, 1.f, 0.5f);
	arr[1].vColor = Vector4{ 1.f,1.f,1.f,1.f };

	arr[2].vPos = Vector3(1.f, -1.f, 0.5f);
	arr[2].vColor = Vector4{ 1.f,1.f,1.f,1.f };

	D3D11_BUFFER_DESC tBufferDesc = {};
	tBufferDesc.ByteWidth = sizeof(VTX) * 3; // 크기
	tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// 생성 이후의 수정 여부 설정
	tBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	tBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // GPU에 다시 접근해서 수정할 수 있도록 write
	tBufferDesc.MiscFlags;
	tBufferDesc.StructureByteStride;

	D3D11_SUBRESOURCE_DATA tSubResData = {};
	tSubResData.pSysMem = arr; // 배열의 시작주소 (ByteWidth가 설정되어있어서 크기설정은 안해줘도 됨)

	DEVICE->CreateBuffer(&tBufferDesc, &tSubResData, g_pVB.GetAddressOf());

	////////////////////
	// VertexShader 생성
	////////////////////
	tstring strShaderFilePath =  CPathManager::GetInstance()->GetContentPath();
	strShaderFilePath += STR_FILE_PATH_Shader;

	int iFlag = D3DCOMPILE_DEBUG;

	if(FAILED(D3DCompileFromFile(strShaderFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VTXShader", "vs_5_0", iFlag, 0, g_VSBlob.GetAddressOf(), g_ErrBlob.GetAddressOf()))) {
		char* pErrorMessage = (char*)g_ErrBlob->GetBufferPointer();
		MessageBoxA(nullptr, pErrorMessage, STR_MSG_FailedToCreateVertexShader, MB_OK);
		assert(nullptr);
	}

	if (FAILED(DEVICE->CreateVertexShader(g_VSBlob->GetBufferPointer(), g_VSBlob->GetBufferSize(), nullptr, g_VS.GetAddressOf()))) {
		assert(nullptr);
	}
	
	////////////////////
	// PixelShader 생성
	////////////////////
	if (FAILED(D3DCompileFromFile(strShaderFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PIXShader", "ps_5_0", iFlag, 0, g_VSBlob.GetAddressOf(), g_ErrBlob.GetAddressOf()))) {
		char* pErrorMessage = (char*)g_ErrBlob->GetBufferPointer();
		MessageBoxA(nullptr, pErrorMessage, STR_MSG_FailedToCreatePixelShader, MB_OK);
		assert(nullptr);
	}
	
	if (FAILED(DEVICE->CreatePixelShader(g_PSBlob->GetBufferPointer(), g_PSBlob->GetBufferSize(), nullptr, g_PS.GetAddressOf()))) {
		assert(nullptr);
	}
}

void Render_Test::TestUpdate()
{
}

void Render_Test::TestRender()
{
	////////// 아래와 같은 파이프 라인대로 설정함 //////////
	// Input Assembler -> Vertex Shader -> Rasterizer -> PixelShader -> OutputMerge

	// Input Asselmber Stage 셋팅
	UINT iStride = 0;
	UINT iOffset = 0;
	CONTEXT->IASetVertexBuffers(0, 1, g_pVB.GetAddressOf(), &iStride, &iOffset);
	
	// 장치가 사용 할 VertexShader, PixelShader 세팅
	CONTEXT->VSSetShader(g_VS.Get(), nullptr, 0);
	CONTEXT->PSSetShader(g_PS.Get(), nullptr, 0);

	// Topology(위상구조) 설정
	CONTEXT->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 삼각형

	// InputLayout(Sementic) 설정


	// 파이프라인 시작
	//CONTEXT->Draw();
}