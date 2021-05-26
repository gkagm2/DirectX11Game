#include "pch.h"
#include "RenderTest.h"
#include "CDevice.h"
#include "CPathManager.h"

// Tip : (Blob : ���ڿ� ������ �޸� ���)
ComPtr<ID3D11Buffer> g_pVB;		 // ���� ������ �����ϴ� ����
ComPtr<ID3D11Buffer> g_pIB;		 // �ε��� ������ �����ϴ� ����

ComPtr<ID3D11VertexShader> g_VS; // ���ؽ� ���̴�
ComPtr<ID3DBlob> g_VSBlob;		 // ���ؽ� ���̴� ������ �ڵ�

ComPtr<ID3D11PixelShader> g_PS;  // �ȼ� ���̴�
ComPtr<ID3DBlob> g_PSBlob;		 // �ȼ� ���̴� ������ �ڵ�

ComPtr<ID3DBlob> g_ErrBlob;		 // ���� �޼��� ���� �뵵

ComPtr<ID3D11InputLayout> g_Layout;// �Է� ����� �ܰ迡 ���� �Է� ���� �����и� �����ϴ� �Է� ���̾�� ��ü�� �����.


void Render_Test::TestInit()
{
	// Triangle

	////////////////////
	// ���ؽ� ���� �����
	////////////////////
	VTX vertexArr[4] = {};
	float fScale = 0.8f;
	vertexArr[0].vPos = Vector3(-1.f * fScale, -1.f * fScale, 0.5f);
	vertexArr[0].vColor = Vector4{ 1.f,0.f,0.f,1.f };

	vertexArr[1].vPos = Vector3(-1.f * fScale, 1.f * fScale, 0.5f);
	vertexArr[1].vColor = Vector4{ 0.f,1.f,0.f,1.f };

	vertexArr[2].vPos = Vector3(1.f * fScale, 1.f * fScale, 0.5f);
	vertexArr[2].vColor = Vector4{ 0.f,0.f,1.f,1.f };

	vertexArr[3].vPos = Vector3(1.f * fScale, -1.f * fScale, 0.5f);
	vertexArr[3].vColor = Vector4{ 0.f,0.f,1.f,1.f };

	D3D11_BUFFER_DESC tBufferDesc = {};
	tBufferDesc.ByteWidth = sizeof(VTX) * 4; // ũ��
	tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// ���� ������ ���� ���� ����
	tBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	tBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // GPU�� �ٽ� �����ؼ� ������ �� �ֵ��� write
	//tBufferDesc.MiscFlags;
	//tBufferDesc.StructureByteStride;

	D3D11_SUBRESOURCE_DATA tSubResData = {};
	tSubResData.pSysMem = vertexArr; // �迭�� �����ּ� (ByteWidth�� �����Ǿ��־ ũ�⼳���� �����൵ ��)

	DEVICE->CreateBuffer(&tBufferDesc, &tSubResData, g_pVB.GetAddressOf());

	////////////////////
	// �ε��� ���� �����
	////////////////////
	UINT arrIdx[6] = { 0 ,1, 3, 3, 1, 2 };
	/*
	1------2
	|��     |
	|  ��   |
	|    �� |
	0------3
	*/

	tBufferDesc = {};
	tBufferDesc.ByteWidth = sizeof(UINT) * 6; // ũ��
	tBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	// ���� ������ ���� ���� ����
	tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//tBufferDesc.MiscFlags;
	//tBufferDesc.StructureByteStride;

	tSubResData = {};
	tSubResData.pSysMem = arrIdx; // �迭�� �����ּ� (ByteWidth�� �����Ǿ��־ ũ�⼳���� �����൵ ��)

	DEVICE->CreateBuffer(&tBufferDesc, &tSubResData, g_pIB.GetAddressOf());

	////////////////////
	// VertexShader ����
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
	// PixelShader ����
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
	// InputLayout(Sementic) ����
	/////////////////////////////
	const UINT iEleCnt = 2;
	D3D11_INPUT_ELEMENT_DESC tLayoutDesc[iEleCnt] = {};
	tLayoutDesc[0].SemanticName = "POSITION";
	tLayoutDesc[0].SemanticIndex = 0;
	tLayoutDesc[0].AlignedByteOffset = 0;
	tLayoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT; // offset 0���� 12byte�ΰ��� �������� �˷��ش�.
	tLayoutDesc[0].InputSlot = 0;
	tLayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA; // �ø�ƽ�� �������� �����Ѵ� (��� �ܰ��)
	tLayoutDesc[0].InstanceDataStepRate = 0;

	tLayoutDesc[1].SemanticName = "COLOR";
	tLayoutDesc[1].SemanticIndex = 0;
	tLayoutDesc[1].AlignedByteOffset = 12;
	tLayoutDesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // offset 12���� 16byte�ΰ��� �������� �˷��ش�.
	tLayoutDesc[1].InputSlot = 0;
	tLayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA; // �ø�ƽ�� �������� �����Ѵ� (��� �ܰ��)
	tLayoutDesc[1].InstanceDataStepRate = 0;

	// param3 : �����ϵ� �ڵ��� �����ּ�, param4 : �ڵ��� ����
	DEVICE->CreateInputLayout(tLayoutDesc, iEleCnt, g_VSBlob->GetBufferPointer(), g_VSBlob->GetBufferSize(), g_Layout.GetAddressOf());
}

void Render_Test::TestUpdate()
{
}

void Render_Test::TestRender()
{
	////////// �Ʒ��� ���� ������ ���δ�� ������ //////////
	// Input Assembler -> Vertex Shader -> Rasterizer -> PixelShader -> OutputMerge

	// Input Asselmber Stage ����
	UINT iStride = sizeof(VTX); // ���� �ϳ��� �ִ� ������ (����)
	UINT iOffset = 0;
	CONTEXT->IASetVertexBuffers(0, 1, g_pVB.GetAddressOf(), &iStride, &iOffset);
	CONTEXT->IASetIndexBuffer(g_pIB.Get(), DXGI_FORMAT_R32_UINT, 0); // 4byte unsigned intŸ������

	// Topology(������) ����
	CONTEXT->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �ﰢ��

	// InputLayout(Semantic) ����
	CONTEXT->IASetInputLayout(g_Layout.Get());
	
	// ��ġ�� ��� �� VertexShader, PixelShader ����
	CONTEXT->VSSetShader(g_VS.Get(), nullptr, 0);
	CONTEXT->PSSetShader(g_PS.Get(), nullptr, 0);

	// ���������� ����
	// ���ؽ� ������ ����� ���
	//UINT iVertexCnt = 3;
	//UINT iStartVertexLocation = 0;
	// CONTEXT->Draw(iVertexCnt, iStartVertexLocation); 

	// �ε��� ���۸� ����� ���
	UINT iIndexCnt = 6;
	UINT iStartIndexLocation = 0;
	UINT iBaseVertexLocation = 0;
	CONTEXT->DrawIndexed(iIndexCnt, iStartIndexLocation, iBaseVertexLocation);
}
