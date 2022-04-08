#ifndef _TESSELLATION_FX
#define _TESSELLATION_FX
#include "value.fx"

// Tessellation Test

struct VTX_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

VTX_IN VS_Tess(VTX_IN _in)
{
    VTX_IN output = (VTX_IN) 0.f;
    output.vPos = _in.vPos;
    output.vUV = _in.vUV;
    return output;
};

// Hull Shader
struct PatchParam // 패치 당 분할 레벨 값
{
    float EdgeParam[3] : SV_TessFactor;
    float InsideParam : SV_InsideTessFactor;
};

// 패치 상수 함수(Patch Constant Function) - 패치당 한번씩 실행되는 함수.
// 패치를 어떻게 분할 할 것인지를 반환
PatchParam HS_PatchConstant(InputPatch<VTX_IN, 3> _patch, uint _patchIdx : SV_PrimitiveID)
{
    PatchParam param = (PatchParam) 0.f;
    
    param.EdgeParam[0] = 16.f;
    param.EdgeParam[1] = 16.f;
    param.EdgeParam[2] = 16.f;
    param.InsideParam = 16.f;
    
    return param;
}

// Domain Shader
[domain("tri")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("HS_PatchConstant")]
[maxtessfactor(64.0)]
[partitioning("integer")]
//[partitioning("fractional_odd")]
VTX_IN HS_Tess(InputPatch<VTX_IN, 3> _patch, uint _idx : SV_OutputControlPointID, uint _patchIdx : SV_PrimitiveID)
{
    VTX_IN output = (VTX_IN) 0.f;
    output = _patch[_idx];
    return output;
}

// -> Tessellator

// Domain Shader
struct DS_OUT
{
    float4 vPos : SV_Position;
    float2 vUV : TEXCOORD;
};
[domain("tri")]
DS_OUT DS_Tess(float3 _vLocation : SV_DomainLocation, const OutputPatch<VTX_IN, 3> _patch, PatchParam _param)
{
    DS_OUT output = (DS_OUT) 0.f;
    
    float3 vLocalPos = _patch[0].vPos * _vLocation[0] + _patch[1].vPos * _vLocation[1] + _patch[2].vPos * _vLocation[2];
    float2 vUV = _patch[0].vUV * _vLocation[0] + _patch[1].vUV * _vLocation[1] + _patch[2].vUV * _vLocation[2];
    
    output.vPos = mul(float4(vLocalPos, 1.f), g_matWorldViewProj);
    output.vUV = vUV;
    
    return output;
}

float4 PS_Tess(DS_OUT _in) : SV_Target
{
    float4 output = float4(1.f, 0.f, 1.f, 1.f);
    return output;
}
#endif