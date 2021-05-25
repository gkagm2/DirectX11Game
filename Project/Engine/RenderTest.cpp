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

ComPtr<ID3D11InputLayout> g_Layout;// 입력 어셈블러 단계에 대한 입력 버퍼 데이털르 설명하는 입력 레이어웃 개체를 만든다.


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
	if (FAILED(D3DCompileFromFile(strShaderFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PIXShader", "ps_5_0", iFlag, 0, g_PSBlob.GetAddressOf(), g_ErrBlob.GetAddressOf()))) {
		char* pErrorMessage = (char*)g_ErrBlob->GetBufferPointer();
		MessageBoxA(nullptr, pErrorMessage, STR_MSG_FailedToCreatePixelShader, MB_OK);
		assert(nullptr);
	}
	
	if (FAILED(DEVICE->CreatePixelShader(g_PSBlob->GetBufferPointer(), g_PSBlob->GetBufferSize(), nullptr, g_PS.GetAddressOf()))) {
		assert(nullptr);
	}

	/////////////////////////////
	// InputLayout(Sementic) 설정
	/////////////////////////////
	const UINT iEleCnt = 2;
	D3D11_INPUT_ELEMENT_DESC tLayoutDesc[iEleCnt] = {};
	tLayoutDesc[0].SemanticName = "POSITION";
	tLayoutDesc[0].SemanticIndex = 0;
	tLayoutDesc[0].AlignedByteOffset = 0;
	tLayoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT; // offset 0부터 12byte인것을 포멧으로 알려준다.
	tLayoutDesc[0].InputSlot = 0;
	tLayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA; // 시멘틱이 정점마다 존재한다 (덩어리 단계로)
	tLayoutDesc[0].InstanceDataStepRate = 0;

	tLayoutDesc[1].SemanticName = "COLOR";
	tLayoutDesc[1].SemanticIndex = 0;
	tLayoutDesc[1].AlignedByteOffset = 12;
	tLayoutDesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // offset 12부터 16byte인것을 포멧으로 알려준다.
	tLayoutDesc[1].InputSlot = 0;
	tLayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA; // 시멘틱이 정점마다 존재한다 (덩어리 단계로)
	tLayoutDesc[1].InstanceDataStepRate = 0;

	// param3 : 컴파일된 코드의 시작주소, param4 : 코드의 길이
	DEVICE->CreateInputLayout(tLayoutDesc, iEleCnt, g_VSBlob->GetBufferPointer(), g_VSBlob->GetBufferSize(), g_Layout.GetAddressOf());
}

void Render_Test::TestUpdate()
{
}

void Render_Test::TestRender()
{
	////////// 아래와 같은 파이프 라인대로 설정함 //////////
	// Input Assembler -> Vertex Shader -> Rasterizer -> PixelShader -> OutputMerge

	// Input Asselmber Stage 셋팅
	UINT iStride = sizeof(VTX); // 정점 하나의 최대 사이즈 (간격)
	UINT iOffset = 0;
	CONTEXT->IASetVertexBuffers(0, 1, g_pVB.GetAddressOf(), &iStride, &iOffset);

	// Topology(위상구조) 설정
	CONTEXT->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 삼각형

	// InputLayout(Semantic) 설정
	CONTEXT->IASetInputLayout(g_Layout.Get());
	
	// 장치가 사용 할 VertexShader, PixelShader 세팅
	CONTEXT->VSSetShader(g_VS.Get(), nullptr, 0);
	CONTEXT->PSSetShader(g_PS.Get(), nullptr, 0);

	// 파이프라인 시작
	UINT iVertexCnt = 3;
	UINT iStartVertexLocation = 0;
	CONTEXT->Draw(iVertexCnt, iStartVertexLocation);
}