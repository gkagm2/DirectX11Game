// Using 5.0 version

struct VTX_IN {
	// semantic : input layout�ʿ��� �ۼ��س��� ���� ���� ���� ������ �����ϴ� ����
	float3 vPos : POSITION; // semantic(layout ����, ����)
	float4 vColor : COLOR;
};

struct VTX_OUT {
	float3 vPos : SV_Position;
	float4 vColor : COLOR;
};

////////////////
// Vertex shader
////////////////

// vPos, vColor�� �Է¹޾Ƽ� ó�����ִ� �Լ�
VTX_OUT VTXShader(VTX_IN _in) {
	VTX_OUT output = (VTX_OUT)0.f; // �ʱ�ȭ
	output.vPosition = (float4)_in.vPos;
	return output;
}

///////////////
// Rasterizer
///////////////


///////////////
// Pixel shader
///////////////
float4 PIXShader(VTX_OUT _in) : SV_Target{
	float4 vOutColor = (float)0.f;
	return vOutColor;
}