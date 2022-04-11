#ifndef _FUNCTION_FX
#define _FUNCTION_FX
#include "value.fx"

static const int direction_Type = 0;
static const int point_Type = 1;
static const int spot_Type = 2;
static const int reflect_Pow = 20;

static const float fConstant = 1.f;
static const float fLinear = 0.09f;
static const float fQuadratic = 0.032f;

TLightColor CalLight3D(int _iLightIdx, float3 _vViewPos, float3 _vViewNormal)
{
    TLightColor tLight = (TLightColor) 0.f;
    TLightInfo tInfo = g_Light3DBuffer[_iLightIdx];
    
    float fDiffusePow = 0.f;
    float fReflectPow = 0.f;
    
    float3 vLightViewPos = mul(float4(tInfo.vLightPos.xyz, 1.f), g_matView).xyz;
    
    float attenuation = 1.f; // 광원과의 거리에 따른 감쇠 비율

    if (direction_Type == tInfo.iLightType)
    {
        // 뷰스페이스 에서의 광원의 방향    
        float3 vLightViewDir = mul(float4(tInfo.vLightDir.xyz, 0.f), g_matView).xyz; // vLightDir는 월드상에서 방향 벡터이므로 view 행렬만 곱함.
        vLightViewDir = -normalize(vLightViewDir);
        
        // --  램버트 코사인 법칙을 이용하여 난반사광을 구함. --
        // 난반사광(확산광) 계수
        fDiffusePow = saturate(dot(vLightViewDir, _vViewNormal));
        
        // 반사광 계수, 반사벡터 구하기
        float3 vReflect = -vLightViewDir + 2.f * dot(vLightViewDir, _vViewNormal) * _vViewNormal;
        vReflect = normalize(vReflect);
    
        // 카메라에서 해당 지점으로(픽셀) 향하는 벡터
        float3 vEye = -normalize(_vViewPos);
        fReflectPow = saturate(dot(vReflect, vEye));
        fReflectPow = pow(fReflectPow, reflect_Pow); // 제곱하여 하이라이트 범위 조절
    }
    else if (point_Type == tInfo.iLightType)
    {
        // View Space 에서 광원으로 부터 표면에 오는 방향
        float3 vLightViewDir = _vViewPos - vLightViewPos;
        float fDistance = length(vLightViewDir);
        vLightViewDir = -normalize(vLightViewDir);
        
        // 난반사광 계수
        fDiffusePow = saturate(dot(vLightViewDir, _vViewNormal));
        
        // 반사광 계수
        // 반사벡터 구하기
        float3 vReflect = -vLightViewDir + 2.f * dot(vLightViewDir, _vViewNormal) * _vViewNormal;
        vReflect = normalize(vReflect);
    
        // 카메라에서 해당 지점으로(픽셀) 향하는 벡터
        float3 vEye = -normalize(_vViewPos);
        fReflectPow = saturate(dot(vReflect, vEye));
        fReflectPow = pow(fReflectPow, reflect_Pow);
                        
        attenuation = saturate(cos(saturate(fDistance / tInfo.fRange) * (PI / 2.f)));
    }
    else if (spot_Type == tInfo.iLightType)
    {
        float3 vLightForwardDir = mul(float4(tInfo.vLightDir.xyz, 0.f), g_matView).xyz; // g_vLightDir는 월드상에서 방향 벡터이므로 view 행렬만 곱함.
        vLightForwardDir = normalize(vLightForwardDir);
        
       // View Space 에서 광원으로 부터 표면에 오는 방향
        float3 vLightViewDir = _vViewPos - vLightViewPos;
        float fDistance = length(vLightViewDir);
        vLightViewDir = -normalize(vLightViewDir);
        
        // 빛이 표면으로 향하는 방향과 빛이 째고 있는 방향을 구해서 내적함.
        float fRadian = dot(vLightViewDir, vLightForwardDir);
        float fAngle = acos(fRadian); // radian to degree
        
        // 난반사광 계수
        fDiffusePow = saturate(dot(vLightViewDir, _vViewNormal));
        
        // 반사광 계수
        // 반사벡터 구하기
        float3 vReflect = -vLightViewDir + 2.f * dot(vLightViewDir, _vViewNormal) * _vViewNormal;
        vReflect = normalize(vReflect);
        
         // 카메라에서 해당 지점으로(픽셀) 향하는 벡터
        float3 vEye = -normalize(_vViewPos);
        fReflectPow = saturate(dot(vReflect, vEye));
        fReflectPow = pow(fReflectPow, reflect_Pow);
        attenuation = saturate(cos(saturate(fDistance / tInfo.fRange) * (PI / 2.f)));
        if (fAngle > tInfo.fAngle * 0.5f)
        { 
            attenuation = 0.f;
        }
    }
    
    tLight.vDiffuse.xyz = tInfo.color.vDiffuse.xyz * fDiffusePow * attenuation;
    tLight.vSpeculer.xyz = tInfo.color.vSpeculer.xyz * fReflectPow * attenuation;
    tLight.vAmbient.xyz = tInfo.color.vAmbient.xyz * attenuation;
        
    return tLight;
}
static float gaussian5x5[25] =
{
    0.003765, 0.015019, 0.023792, 0.015019, 0.003765,
    0.015019, 0.059912, 0.094907, 0.059912, 0.015019,
    0.023792, 0.094907, 0.150342, 0.094907, 0.023792,
    0.015019, 0.059912, 0.094907, 0.059912, 0.015019,
    0.003765, 0.015019, 0.023792, 0.015019, 0.003765,
};

float gaussian5x5Sample(in int2 _uv, in Texture2D _tex)
{
    float4 fOut = (float4) 0.f;
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            fOut += _tex[_uv + int2(i - 2, j - 2)] * gaussian5x5[i * 5 + j];
        }
    }
    return fOut.x;
}

// 0 ~ 1 사이 랜덤
float Rand(in float _fKey)
{
    // 랜덤 생성 위치, 방향    
    float2 vUV = float2(_fKey + g_fAccTime, g_fAccTime);
    vUV.y += sin(vUV.x * 2 * 3.141592);
    
    if (vUV.x > 0)
        vUV.x = frac(vUV.x);
    else
        vUV.x = 1.f - abs(vUV.x);
    if (vUV.y > 0)
        vUV.y = frac(vUV.y);
    else
        vUV.y = 1.f - abs(vUV.y);
    vUV = vUV * g_vNoiseResolution;
    
    return gaussian5x5Sample(vUV, g_TexNoise);
}

// 카메라와 패치사이의 거리에 따른 테셀레이션 레벨 계산
float GetTessFactor(float3 _vViewPos, float _fMinLevel, float _fMaxLevel, float _fMaxDist, float _fMinDist)
{
    if (_fMinLevel > _fMaxLevel || _fMaxDist > _fMinDist)
        return 1.f;
    
    // 카메라와의 거리
    float fDist = length(_vViewPos);

    float fRatio = (fDist - _fMaxDist) / (_fMinDist - _fMaxDist);
    float fLevelStep = _fMaxLevel - _fMinLevel - 1.f;
    
    float fTessLv = _fMaxLevel - (fLevelStep * fRatio);
    
    return clamp(fTessLv, _fMinLevel, _fMaxLevel);
}


int IntersectsLay(float3 _vertices[3], float3 _vStart, float3 _vDir, out float3 _vCrossPoint, out float _fResult)
{
    float3 edge[2] = { (float3) 0.f, (float3) 0.f };
    edge[0] = _vertices[1].xyz - _vertices[0].xyz;
    edge[1] = _vertices[2].xyz - _vertices[0].xyz;

    float3 normal = normalize(cross(edge[0], edge[1]));
    float b = dot(normal, _vDir);

    float3 w0 = _vStart - _vertices[0].xyz;
    float a = -dot(normal, w0);
    float t = a / b;

    _fResult = t;

    float3 p = _vStart + t * _vDir;
    
    _vCrossPoint = p;
    
    float uu, uv, vv, wu, wv, inverseD;
    uu = dot(edge[0], edge[0]);
    uv = dot(edge[0], edge[1]);
    vv = dot(edge[1], edge[1]);

    float3 w = p - _vertices[0].xyz;
    wu = dot(w, edge[0]);
    wv = dot(w, edge[1]);
    inverseD = uv * uv - uu * vv;
    inverseD = 1.0f / inverseD;

    float u = (uv * wv - vv * wu) * inverseD;
    if (u < 0.0f || u > 1.0f)
    {
        return 0;
    }

    float v = (uv * wu - uu * wv) * inverseD;
    if (v < 0.0f || (u + v) > 1.0f)
    {
        return 0;
    }
        
    return 1;
}

matrix GetBoneMat(int _iBoneIdx, int _iRowIdx)
{
    return g_arrBoneMat[(g_iBoneCount * _iRowIdx) + _iBoneIdx];
}

void Skinning(inout float3 _vPos, inout float3 _vTangent, inout float3 _vBinormal, inout float3 _vNormal
                        , inout float4 _vWeight, inout float4 _vIndices
                        , int _iRowIdx)
{
    tSkinningInfo info = (tSkinningInfo) 0.f;
    
    if (_iRowIdx == -1)
        return;

    for (int i = 0; i < 4; ++i)
    {
        if (0.f == _vWeight[i])
            continue;

        matrix matBone = GetBoneMat((int) _vIndices[i], _iRowIdx);

        info.vPos += (mul(float4(_vPos, 1.f), matBone) * _vWeight[i]).xyz;
        info.vTangent += (mul(float4(_vTangent, 0.f), matBone) * _vWeight[i]).xyz;
        info.vBinormal += (mul(float4(_vBinormal, 0.f), matBone) * _vWeight[i]).xyz;
        info.vNormal += (mul(float4(_vNormal, 0.f), matBone) * _vWeight[i]).xyz;
    }
    
    _vPos = info.vPos;
    _vTangent = normalize(info.vTangent);
    _vBinormal = normalize(info.vBinormal);
    _vNormal = normalize(info.vNormal);
}

#endif