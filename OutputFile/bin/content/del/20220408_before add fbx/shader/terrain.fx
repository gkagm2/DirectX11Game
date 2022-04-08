#ifndef _Terrain_FX
#define _Terrain_FX

#include "function.fx"

//------------------------------------------
// Terrain Shader
// Deferred
#define FaceXCount g_int_0
#define FaceZCount g_int_1
#define HeightMapTex g_tex_0
#define WeightMapTEx g_tex_1
#define TileTexArr      g_texarr_0
#define TileCount       g_float_0
#define HeightMapResolution g_vec2_0
#define WeightMapResolution g_vec2_1

// g_tex_2 : color
// g_tex_3 : normal Tex
//------------------------------------------

StructuredBuffer<float4> WEIGHT_MAP : register(t17);


struct VTX_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VTX_OUT
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    float2 vFullUV : TEXCOORD1;
    float3 vViewPos : POSITION1;
};

VTX_OUT VS_Terrain(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;

    output.vPos = _in.vPos;
    output.vUV = _in.vUV;
    output.vFullUV = _in.vUV / float2(FaceXCount, FaceZCount);
    output.vViewPos = mul(float4(_in.vPos, 1.f), g_matWorldView).xyz;

    return output;
}

struct PatchParam
{
    float edgeParam[3] : SV_TessFactor;
    float insideParam : SV_InsideTessFactor;
};

// ���� ���̴� (��ġ ��� �Լ�)
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
    float2 vFullUV : TEXCOORD1;
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

    // _vLocaltion : ������ ����, Ȥ�� ���� ����
    // _patch : ���� ���� (������)
    float3 vLocalPos = _patch[0].vPos * _vLocation[0] + _patch[1].vPos * _vLocation[1] + _patch[2].vPos * _vLocation[2];
    output.vUV = _patch[0].vUV * _vLocation[0] + _patch[1].vUV * _vLocation[1] + _patch[2].vUV * _vLocation[2];
    output.vFullUV = _patch[0].vFullUV * _vLocation[0] + _patch[1].vFullUV * _vLocation[1] + _patch[2].vFullUV * _vLocation[2];
    
    // ���� ��ü ���� UV�� ��ȯ
    float2 vTerrainUVStep = float2(1.f / HeightMapResolution.x, 1.f / HeightMapResolution.y);
    float2 vTerrainUV = float2(output.vUV.x / (float) FaceXCount, output.vUV.y / (float) FaceZCount);
    
    float2 vTerrainUpUV = float2(vTerrainUV.x, vTerrainUV.y - vTerrainUVStep.y);
    float2 vTerrainDownUV = float2(vTerrainUV.x, vTerrainUV.y + vTerrainUVStep.y);
    float2 vTerrainLeftUV = float2(vTerrainUV.x - vTerrainUVStep.x, vTerrainUV.y);
    float2 vTerrainRightUV = float2(vTerrainUV.x + vTerrainUVStep.x, vTerrainUV.y);
    
    // �� �������� �ڱ� ��ġ�� �´� ���̰��� ���̸ʿ��� ���� �� ��, �ڽ��� ���� ���̷� ����
    vLocalPos.y = HeightMapTex.SampleLevel(Sample_Anisotropic, vTerrainUV, 0).r;
    output.vViewPos = mul(float4(vLocalPos, 1.f), g_matWorldView).xyz;
    float2 vTerrainLocalPosStep = float2(1.f / FaceXCount, 1.f / FaceZCount);

    float3 vLocalUpPos = float3(vLocalPos.x, HeightMapTex.SampleLevel(Sample_Anisotropic, vTerrainUpUV, 0).r, vLocalPos.z + vTerrainLocalPosStep.y);
    float3 vLocalDownPos = float3(vLocalPos.x, HeightMapTex.SampleLevel(Sample_Anisotropic, vTerrainDownUV, 0).r, vLocalPos.z - vTerrainLocalPosStep.y);
    float3 vLocalLeftPos = float3(vLocalPos.x - vTerrainLocalPosStep.x, HeightMapTex.SampleLevel(Sample_Anisotropic, vTerrainLeftUV, 0).r, vLocalPos.z);
    float3 vLocalRightPos = float3(vLocalPos.x + vTerrainLocalPosStep.x, HeightMapTex.SampleLevel(Sample_Anisotropic, vTerrainRightUV, 0).r, vLocalPos.z);
    
    // Tangent, Binormal, Normal ����        
    output.vViewTangent = normalize(mul(float4(vLocalRightPos - vLocalLeftPos, 0.f), g_matWorldView)).xyz;
    output.vViewBinormal = normalize(mul(float4(vLocalUpPos - vLocalDownPos, 0.f), g_matWorldView)).xyz;
    output.vViewNormal = normalize(cross(output.vViewBinormal, output.vViewTangent)).xyz;
    
    // ������ǥ�� ����
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
    
    //output.vColor = float4(0.8f, 0.8f, 0.8f, 1.f);    
    //output.vColor += TileTexArr.Sample(sam, float3(_in.vUV, 3));
    
    float3 vViewNormal = _in.vViewNormal;
    
    // Ÿ�� �迭�ؽ��İ� ������
    if (bTexArr_0)
    {
        float2 derivX = ddx(_in.vUV); // �����ȼ��� x�� ��̺а��� ���Ѵ�
        float2 derivY = ddy(_in.vUV); // �����ȼ��� y�� ��̺а��� ���Ѵ�
                        
        // Ÿ�� ����
        int2 iWeightIdx = (int2) (_in.vFullUV * WeightMapResolution);
        float4 vWeight = WEIGHT_MAP[iWeightIdx.y * (int) WeightMapResolution.x + iWeightIdx.x];
        float4 vColor = (float4) 0.f;
        
        int iMaxWeightIdx = 0;
        float fMaxWeight = 0.f;
        for (int i = 0; i < 4; ++i)
        {
            //vColor += TileTexArr.SampleGrad(sam, float3(_in.vUV, i), derivX, derivY) * vWeight[i];
            vColor += TileTexArr.Sample(Sample_Anisotropic, float3(_in.vUV, i)) * vWeight[i];
                        
            if (fMaxWeight < vWeight[i])
            {
                fMaxWeight = vWeight[i];
                iMaxWeightIdx = i;
            }
        }
        output.vColor = float4(vColor.rgb, 1.f);
    
        // Ÿ�� �븻
        float3 vTangentSpaceNormal = TileTexArr.SampleGrad(Sample_Anisotropic, float3(_in.vUV, iMaxWeightIdx + TileCount), derivX, derivY).xyz;
        //float3 vTangentSpaceNormal = TileTexArr.Sample(sam, float3(_in.vUV, iMaxWeightIdx + TileCount)).xyz;
        vTangentSpaceNormal = vTangentSpaceNormal * 2.f - 1.f;
        
        float3x3 matTBN = { _in.vViewTangent, _in.vViewBinormal, _in.vViewNormal };
        vViewNormal = normalize(mul(vTangentSpaceNormal, matTBN));
    }
   
    output.vViewPos = float4(_in.vViewPos, 1.f);
    output.vViewNormal = float4(vViewNormal, 1.f);
    
    return output;
}

#endif