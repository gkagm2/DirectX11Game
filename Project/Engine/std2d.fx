// Using 5.0 version

// ��� ���� ��������	
cbuffer TRANSFORM : register(b0)
{
    row_major Matrix g_matWorld; // ���� v (L-)
    row_major Matrix g_matView;
    row_major Matrix g_matProjection;
}

// Texture register (t)
Texture2D g_tex_0 : register(t0);

// Sampler Stage
SamplerState g_sam_0 : register(s0); // Anisotropic
SamplerState g_sam_1 : register(s1); // Point

struct VTX_IN {
	// semantic : input layout�ʿ��� �ۼ��س��� ���� ���� ���� ������ �����ϴ� ����
	float3 vPosition : POSITION; // semantic(layout ����, ����)
	float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

struct VTX_OUT {
	float4 vPosition : SV_Position;
	float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

////////////////
// Vertex shader
////////////////

// vPos, vColor�� �Է¹޾Ƽ� ó�����ִ� �Լ�
VTX_OUT VTXShader(VTX_IN _in)
{
	VTX_OUT output = (VTX_OUT)0.f; // �ʱ�ȭ
	
    float4 vWorldPos = mul(float4(_in.vPosition, 1.0f), g_matWorld);
    float4 vViewPos = mul(vWorldPos, g_matView);
    float4 vProjPos = mul(vViewPos, g_matProjection);
	
    output.vPosition = vProjPos;
	output.vColor = _in.vColor;
    output.vUV = _in.vUV;
	return output;
}

///////////////
// Rasterizer
///////////////

///////////////
// Pixel shader
///////////////
float4 PIXShader(VTX_OUT _in) : SV_Target {
    float2 vOutUV = _in.vUV;
    float4 vOutColor = g_tex_0.Sample(g_sam_0, vOutUV);
	return vOutColor;
}