#ifndef _FUNCTION
#define _FUNCTION
#include "value.fx"



TLightColor CalLight3D(int _iLightIdx, float3 _vViewPos, float3 _vViewNormal)
{
    TLightColor tLight = (TLightColor) 0.f;
    
    // View Space에서 light 방향 벡터
    float3 vLightViewDir = mul(float4(g_Light3DBuffer[_iLightIdx].vLightDir.xyz, 0.f), g_matView).xyz; // g_vLightDir는 월드상에서 방향 벡터이므로 view 행렬만 곱함.
    vLightViewDir = normalize(vLightViewDir);
    float3 vLightViewOpDir = -vLightViewDir; // 빛의 반대방향
    
    // --  램버트 코사인 법칙을 이용하여 난반사광을 구함. --
    // 난반사광(확산광) 계수
    float3 fDiffusePow = saturate(dot(vLightViewOpDir, _vViewNormal));
    
    // 반사광 계수. 반사벡터 구함
    float3 vReflectDir = vLightViewDir + 2.f * dot(vLightViewOpDir, _vViewNormal) * _vViewNormal;
    vReflectDir = normalize(vReflectDir);
    
    // 카메라에서 해당 지점으로(픽셀) 향하는 벡터
    float3 vEyeToPosDir = normalize(_vViewPos);
    float fReflectPow = saturate(dot(-vReflectDir, vEyeToPosDir));
    fReflectPow = pow(fReflectPow, 20); // 제곱하여 하이라이트 범위 조절
    
    tLight.vDiffuse.xyz = g_Light3DBuffer[_iLightIdx].color.vDiffuse.xyz * fDiffusePow;
    tLight.vSpeculer.xyz = g_Light3DBuffer[_iLightIdx].color.vSpeculer.xyz * fReflectPow;
    tLight.vAmbient.xyz = g_Light3DBuffer[_iLightIdx].color.vAmbient.xyz;
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


#endif