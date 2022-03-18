#ifndef _LANDSCAPE_FX
#define _LANDSCAPE_FX

#include "value.fx"

struct VTX_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};


VTX_IN VS_Landscape(VTX_IN _in)
{
    VTX_IN output = (VTX_IN) 0.f;

    output.vPos = _in.vPos;
    output.vUV = _in.vUV;

    return output;
}

struct PatchParam
{
    float edgeParam[3] : SV_TessFactor;
    float insideParam : SV_InsideTessFactor;
};

// 덮개 쉐이더 (패치 상수  )
PatchParam HS_PatchConstant(InputPatch<VTX_IN, 3> _patch, uint _patchID : SV_PrimitiveID)
{
    PatchParam param = (PatchParam) 0.f;

    param.edgeParam[0] = 1.f;
    param.edgeParam[1] = 1.f;
    param.edgeParam[2] = 1.f;
    param.insideParam = 1.f;

    return param;
}

// Hull Shader
[domain("tri")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("HS_PatchConstant")]
[maxtessfactor(64.0)]
[partitioning("integer")]
//[partitioning("fractional_odd")]
VTX_IN HS_Landscape(InputPatch<VTX_IN, 3> _patch, uint _ctrPointID : SV_OutputControlPointID, uint _patchID : SV_PrimitiveID)
{
    VTX_IN output = (VTX_IN) 0.f;
    output= _patch[_ctrPointID];
    return output;
}


// Tessellator

struct DS_OUT
{
    float4 vPos : SV_Position;
    float2 vUV : TEXCOORD;
};
// Domain Shader
[domain("tri")]
DS_OUT DS_Landscape(float3 _vLocation : SV_DomainLocation, const OutputPatch<VTX_IN, 3> _patch, PatchParam _param)
{
    DS_OUT output = (DS_OUT) 0.f;

    // _vLocaltion : 생성된 정점, 혹은 원본 정점
    // _patch : 원본 정점 (보간값)
    

    float3 vLocalPos = _patch[0].vPos * _vLocation[0] + _patch[1].vPos * _vLocation[1] + _patch[2].vPos * _vLocation[2];
    float2 vUV = _patch[0].vUV * _vLocation[0] + _patch[1].vUV * _vLocation[1] + _patch[2].vUV * _vLocation[2];
    
    output.vPos = mul(float4(vLocalPos, 1.f), g_matWorldViewProj);
    output.vUV = vUV;

    return output;
}

float4 PS_Landscape(DS_OUT _in) : SV_Target
{
    float4 output = float4(1.f, 0.f, 1.f, 1.f);
    return output;
}

#endif