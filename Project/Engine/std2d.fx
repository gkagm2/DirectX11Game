// Using 5.0 version

struct VTX_IN {
	// semantic : input layout쪽에서 작성해놓은 정적 내부 구조 정보를 설명하는 역할
	float3 vPosition : POSITION; // semantic(layout 연동, 설명)
	//float4 vColor : COLOR;4
};

struct VTX_OUT {
	float4 vPosition : SV_Position;
	float4 vColor : COLOR;
};

////////////////
// Vertex shader
////////////////

// vPos, vColor를 입력받아서 처리해주는 함수
VTX_OUT VTXShader(VTX_IN _in) {
	VTX_OUT output = (VTX_OUT)0.f; // 초기화
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