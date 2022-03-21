#ifndef _Terrain_FX
#define _Terrain_FX

#include "value.fx"
#include "function.fx"
//------------------------------------------
// Terrain Shader
// Deferred
#define FaceXCount g_int_0
#define FaceZCount g_int_1
#define HeightMapTex g_tex_0
#define WeightMapTEx g_tex_1
#define HeightMapResolution g_vec2_0

// g_tex_2 : color
// g_tex_3 : normal Tex
//------------------------------------------

struct VTX_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VTX_OUT
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    float3 vViewPos : POSITION1;
};

VTX_OUT VS_Terrain(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;

    output.vPos = _in.vPos;
    output.vUV = _in.vUV;
    output.vViewPos = mul(float4(_in.vPos, 1.f), g_matWorldView).xyz;

    return output;
}

struct PatchParam
{
    float edgeParam[3] : SV_TessFactor;
    float insideParam : SV_InsideTessFactor;
};

// 덮개 쉐이더 (패치 상수 함수)
PatchParam HS_PatchConstant(InputPatch<VTX_OUT, 3> _patch, uint _patchID : SV_PrimitiveID)
{
    PatchParam param = (PatchParam) 0.f;

    float3 vViewSidePos = (_patch[0].vViewPos + _patch[2].vViewPos) * 0.5f;
    float3 vViewUpDownPos = (_patch[1].vViewPos + _patch[2].vViewPos) * 0.5f;
    float3 vViewSlidePos = (_patch[0].vViewPos + _patch[1].vViewPos) * 0.5f;
    
    param.edgeParam[0] = GetTessFactor(vViewUpDownPos, 1.f, 16.f, 1000.f, 5000.f);
    param.edgeParam[1] = GetTessFactor(vViewSidePos, 1.f, 16.f, 1000.f, 5000.f);
    param.edgeParam[2] = GetTessFactor(vViewSlidePos, 1.f, 16.f, 1000.f, 5000.f);
    param.insideParam = param.edgeParam[2];

    return param;
}

// Hull Shader
[domain("tri")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("HS_PatchConstant")]
[maxtessfactor(64.0)]
//[partitioning("integer")]
[partitioning("fractional_odd")]
VTX_OUT HS_Terrain(InputPatch<VTX_OUT, 3> _patch, uint _ctrPointID : SV_OutputControlPointID, uint _patchID : SV_PrimitiveID)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    output= _patch[_ctrPointID];
    return output;
}


// Tessellator


struct DS_OUT
{
    float4 vPos : SV_Position;
    float2 vUV : TEXCOORD;
    float3 vViewPos : POSITION;
    
    float3 vViewTangent : TANGENT;
    float3 vViewBinormal: BINORAML;
    float3 vViewNormal  : NORMAL;
};

// Domain Shader
[domain("tri")]
DS_OUT DS_Terrain(float3 _vLocation : SV_DomainLocation, const OutputPatch<VTX_OUT, 3> _patch, PatchParam _param)
{
    DS_OUT output = (DS_OUT) 0.f;

    // _vLocaltion : 생성된 정점, 혹은 원본 정점
    // _patch : 원본 정점 (보간값)
    float3 vLocalPos = _patch[0].vPos * _vLocation[0] + _patch[1].vPos * _vLocation[1] + _patch[2].vPos * _vLocation[2];
    float2 vUV = _patch[0].vUV * _vLocation[0] + _patch[1].vUV * _vLocation[1] + _patch[2].vUV * _vLocation[2];
    output.vViewPos = _patch[0].vViewPos * _vLocation[0] + _patch[1].vViewPos * _vLocation[1] + _patch[1].vViewPos * _vLocation[1];
    output.vUV = vUV;

    // 지형 전체 기준 UV로 전환
    float2 vTerrainUVStep = 1.f / HeightMapResolution;
    float2 vTerrainUV = float2(vUV.x / (float) FaceXCount, vUV.y / (float) FaceZCount);
    
    float2 vTerrainUpUV = float2(vTerrainUV.x, vTerrainUV.y - vTerrainUVStep.y);
    float2 vTerrainDownUV = float2(vTerrainUV.x, vTerrainUV.y + vTerrainUVStep.y);
    float2 vTerrainLeftUV = float2(vTerrainUV.x - vTerrainUVStep.x, vTerrainUV.y);
    float2 vTerrainRightUV = float2(vTerrainUV.x + vTerrainUVStep.x, vTerrainUV.y);
    
    // 각 정점들이 자기 위치에 맞는 높이값을 높이맵에서 추출 한 후, 자신의 로컬 높이로 지정
    vLocalPos.y = HeightMapTex.SampleLevel(Sample_Anisotropic, vTerrainUV, 0).r;
  
    float2 vTerrainLocalPosStep = float2(1.f / FaceXCount, 1.f / FaceZCount);

    float3 vLocalUpPos = float3(vLocalPos.x, HeightMapTex.SampleLevel(Sample_Anisotropic, vTerrainUpUV, 0).r, vLocalPos.z + vTerrainLocalPosStep.y);
    float3 vLocalDownPos = float3(vLocalPos.x, HeightMapTex.SampleLevel(Sample_Anisotropic, vTerrainDownUV, 0).r, vLocalPos.z - vTerrainLocalPosStep.y);
    float3 vLocalLeftPos = float3(vLocalPos.x - vTerrainLocalPosStep.x, HeightMapTex.SampleLevel(Sample_Anisotropic, vTerrainLeftUV, 0).r, vLocalPos.z);
    float3 vLocalRightPos = float3(vLocalPos.x + vTerrainLocalPosStep.x, HeightMapTex.SampleLevel(Sample_Anisotropic, vTerrainRightUV, 0).r, vLocalPos.z);
    
    // Tangent, Binormal, Normal 재계산        
    output.vViewTangent = normalize(mul(float4(vLocalRightPos - vLocalLeftPos, 0.f), g_matWorldView)).xyz;
    output.vViewBinormal = normalize(mul(float4(vLocalUpPos - vLocalDownPos, 0.f), g_matWorldView)).xyz;
    output.vViewNormal = normalize(cross(output.vViewBinormal, output.vViewTangent)).xyz;
    
    // 투영좌표계 연산
    output.vPos = mul(float4(vLocalPos, 1.f), g_matWorldViewProj);

    return output;
}

struct PS_OUT
{
    float4 vColor       : SV_Target0;
    float4 vViewNormal  : SV_Target1;
    float4 vViewPos     : SV_Target2;
    float4 vData        : SV_Target3;
};

PS_OUT PS_Terrain(DS_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;
    output.vColor = float4(0.7f, 0.7f, 0.7f, 1.f);
    output.vViewPos = float4(_in.vViewPos, 1.f);
    output.vViewNormal = float4(_in.vViewNormal, 1.f);
    return output;
}

#endif