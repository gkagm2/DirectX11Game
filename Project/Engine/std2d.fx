// Using 5.0 version

// 상수 버퍼 레지스터	
cbuffer TRANSFORM : register(b0)
{
    row_major Matrix g_matWorld; // 행기반 v (L-)
    row_major Matrix g_matView;
    row_major Matrix g_matProjection;
}

// Texture register (t)

struct VTX_IN {
	// semantic : input layout쪽에서 작성해놓은 정적 내부 구조 정보를 설명하는 역할
	float3 vPosition : POSITION; // semantic(layout 연동, 설명)
	float4 vColor : COLOR;
};

struct VTX_OUT {
	float4 vPosition : SV_Position;
	float4 vColor : COLOR;
};

////////////////
// Vertex shader
////////////////

// vPos, vColor를 입력받아서 처리해주는 함수
VTX_OUT VTXShader(VTX_IN _in)
{
	VTX_OUT output = (VTX_OUT)0.f; // 초기화
	
    float4 vWorldPos = mul(float4(_in.vPosition, 1.0f), g_matWorld);
    float4 vViewPos = mul(vWorldPos, g_matView);
    float4 vProjPos = mul(vViewPos, g_matProjection);
	
    output.vPosition = vProjPos;
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