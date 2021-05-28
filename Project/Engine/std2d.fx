// Using 5.0 version

cbuffer POSITION : register(b0)
{
    float4 vOffsetPos;
}


struct VTX_IN {
	// semantic : input layout�ʿ��� �ۼ��س��� ���� ���� ���� ������ �����ϴ� ����
	float3 vPosition : POSITION; // semantic(layout ����, ����)
	float4 vColor : COLOR;
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
	
    _in.vPosition += vOffsetPos.xyz;
	
	output.vPosition = float4(_in.vPosition, 1.f);
	output.vColor = _in.vColor;
	return output;
}

///////////////
// Rasterizer
///////////////


///////////////
// Pixel shader
///////////////
float4 PIXShader(VTX_OUT _in) : SV_Target {
    float4 vOutColor = _in.vColor;
	return vOutColor;
}