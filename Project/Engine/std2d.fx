// Using 5.0 version

struct VTX_IN {
	// semantic : input layout�ʿ��� �ۼ��س��� ���� ���� ���� ������ �����ϴ� ����
	float3 vPosition : POSITION; // semantic(layout ����, ����)
	//float4 vColor : COLOR;4
};

struct VTX_OUT {
	float4 vPosition : SV_Position;
	float4 vColor : COLOR;
};

////////////////
// Vertex shader
////////////////

// vPos, vColor�� �Է¹޾Ƽ� ó�����ִ� �Լ�
VTX_OUT VTXShader(VTX_IN _in) {
	VTX_OUT output = (VTX_OUT)0.f; // �ʱ�ȭ
	output.vPosition = float4(_in.vPosition, 1.f);
	//output.vColor = _in.vColor;
	return output;
}

///////////////
// Rasterizer
///////////////


///////////////
// Pixel shader
///////////////
float4 PIXShader(VTX_OUT _in) : SV_Target {
	float4 vOutColor = float4(0.5f, 0.5f, 0.5f, 1.f);
	return vOutColor;
}