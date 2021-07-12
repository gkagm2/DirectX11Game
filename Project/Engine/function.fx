#ifndef _FUNCTION
#define _FUNCTION
#include "value.fx"

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
            int2 idx = _uv + int2(i - 2, j - 2);
            fOut += _tex[idx] * gaussian5x5[i * 5 + j];
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