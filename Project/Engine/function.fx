#ifndef _FUNCTION
#define _FUNCTION
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
    float fReflectPow = 1.f;
    
    float3 vLightViewPos = mul(float4(tInfo.vLightPos.xyz, 1.f), g_matView).xyz;
    
    float attenuation = 1.f; // 감쇠 
    
    // 광원과의 거리에 따른 감쇠 비율

    if (direction_Type == tInfo.iLightType)
    {
        attenuation = 1.f;
        // View Space에서 light 방향 벡터
        float3 vLightViewDir = mul(float4(tInfo.vLightDir.xyz, 0.f), g_matView).xyz; // g_vLightDir는 월드상에서 방향 벡터이므로 view 행렬만 곱함.
        vLightViewDir = normalize(vLightViewDir);
        float3 vLightViewOpDir = -vLightViewDir; // 빛의 반대방향
        
        // --  램버트 코사인 법칙을 이용하여 난반사광을 구함. --
        // 난반사광(확산광) 계수
        fDiffusePow = saturate(dot(vLightViewOpDir, _vViewNormal));
        
        // 반사광 계수. 반사벡터 구함
        float3 vReflectDir = vLightViewDir + 2.f * dot(vLightViewOpDir, _vViewNormal) * _vViewNormal;
        vReflectDir = normalize(vReflectDir);
        
        // 카메라에서 해당 지점으로(픽셀) 향하는 벡터
        float3 vEyeToPosDir = normalize(_vViewPos);
        fReflectPow = saturate(dot(-vReflectDir, vEyeToPosDir));
        fReflectPow = pow(fReflectPow, reflect_Pow); // 제곱하여 하이라이트 범위 조절
    }
    else if (point_Type == tInfo.iLightType)
    {
        // View space에서 광원으로부터 표면에 오는 방향과 거리를 구함
        float3 vLightViewDir = _vViewPos - vLightViewPos;
        float fDistance = length(vLightViewDir);
        vLightViewDir = normalize(vLightViewDir);
        float3 vLightViewOpDir = -vLightViewDir; // 표면에서 빛으로 향하는 방향
        
        // 난방사광(확산광) 계수
        fDiffusePow = saturate(dot(vLightViewOpDir, _vViewNormal));
        
        // 반사광 계수, 반사벡터 
        float3 vReflectDir = vLightViewDir + 2.f * dot(vLightViewOpDir, _vViewNormal) * _vViewNormal;
        vReflectDir = normalize(vReflectDir);
     
        // 카메라에서 해당 지점으로(픽셀) 향하는 벡터
        float3 vEyeToPosDir = normalize(_vViewPos);
        fReflectPow = saturate(dot(-vReflectDir, vEyeToPosDir));
        fReflectPow = pow(fReflectPow, reflect_Pow);
        
        // 감쇠 구현
        //attenuation = saturate(1.f / (fConstant + fLinear * fDistance + fQuadratic * (fDistance * fDistance)));
        
        attenuation = saturate(cos((fDistance / tInfo.fRange)) * (PI / 2.f));
    }
    else if (spot_Type == tInfo.iLightType)
    {
        float3 vLightForwardDir = mul(float4(tInfo.vLightDir.xyz, 0.f), g_matView).xyz; // g_vLightDir는 월드상에서 방향 벡터이므로 view 행렬만 곱함.
        vLightForwardDir = normalize(vLightForwardDir);
        
        // View space에서 광원으로부터 표면에 오는 방향과 거리를 구함
        float3 vLightViewDir = _vViewPos - vLightViewPos;
        float fDistance = length(vLightViewDir);
        vLightViewDir = normalize(vLightViewDir);
        float3 vLightViewOpDir = -vLightViewDir; // 표면에서 빛으로 향하는 방향
        
        // 빛이 표면으로 향하는 방향과 빛이 째고 있는 방향을 구해서 내적함.
        float fRadian = dot(vLightViewDir, vLightForwardDir);
        float fAngle = acos(fRadian); // radian to degree
        
        if (fAngle < tInfo.fAngle * 0.5f)
        {
            // 난방사광(확산광) 계수
            fDiffusePow = saturate(dot(vLightViewOpDir, _vViewNormal));
        
            // 반사광 계수, 반사벡터 
            float3 vReflectDir = vLightViewDir + 2.f * dot(vLightViewOpDir, _vViewNormal) * _vViewNormal;
            vReflectDir = normalize(vReflectDir);

            
            //float cutOff = 12.5f; // radian
            
            //float theta = dot(vLightViewOpDir, -vLightForwardDir);
            //float epsilon = cutOff - 15.f;
            //float intensity = clamp((theta - 15.f) / epsilon, 0.0f, 1.0f);
            
            
            //tInfo.color.vDiffuse.xyz *= intensity;
            //tInfo.color.vSpeculer.xyz * +intensity;
            
            // 카메라에서 해당 지점으로(픽셀) 향하는 벡터
            float3 vEyeToPosDir = normalize(_vViewPos);
            fReflectPow = saturate(dot(-vReflectDir, vEyeToPosDir));
            fReflectPow = pow(fReflectPow, reflect_Pow);

        
            // 감쇠 구현
            //attenuation = 1.f / (fConstant + fLinear * fDistance + fQuadratic * (fDistance * fDistance));
            //if (fDistance > tInfo.fRange)
            attenuation = saturate(cos((fDistance / tInfo.fRange)) * (PI / 2.f));
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


#endif