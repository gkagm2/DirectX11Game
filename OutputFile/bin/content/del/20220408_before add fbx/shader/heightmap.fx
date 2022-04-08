#ifndef _HEIGHTMAP_FX
#define _HEIGHTMAP_FX

#include "value.fx"

RWTexture2D<float> HEIGHT_MAP : register(u0);
StructuredBuffer<TRaycastOut> LOCATION : register(t16); // 브러쉬 위치(최상단 기준)

#define WIDTH       g_int_0       // 브러쉬 Width
#define HEIGHT      g_int_1      // 브러쉬 Height

#define BRUSH_TEX   g_tex_0   // 브러쉬 텍스쳐
#define SCALE       g_vec2_0      // 브러쉬 크기
#define BRUSH_IDX   g_int_2   // 브러쉬 인덱스


[numthreads(32,32,1)]
void CS_HeightMap(int3 _iThreadID : SV_DispatchThreadID)
{
     //HEIGHT_MAP[_iThreadID.xy] += 10.f;
    //return;
    
    if (WIDTH <= _iThreadID.x || HEIGHT <= _iThreadID.y || !LOCATION[0].iSuccess)
    {
        return;
    }
    
    int2 vCenterPos = float2(WIDTH, HEIGHT) * LOCATION[0].vUV;
    int2 vScale = float2(WIDTH, HEIGHT) * SCALE;
    
    if (_iThreadID.x < vCenterPos.x - (vScale.x / 2) || vCenterPos.x + (vScale.x / 2) < _iThreadID.x
         || _iThreadID.y < vCenterPos.y - (vScale.y / 2) || vCenterPos.y + (vScale.y / 2) < _iThreadID.y)
    {
        return;
    }
    
    // brush texture 로 높이 설정
    int2 vLTPos = vCenterPos - (vScale / 2);
    float2 vUV = float2(_iThreadID.xy - vLTPos) / float2(vScale);
    
    //float4 vBrushColor = BRUSH_TEX.SampleLevel(sam, float3(vUV, BRUSH_IDX), 0);
    
    float4 vBrushColor = BRUSH_TEX.SampleLevel(Sample_Anisotropic, vUV, 0);
    HEIGHT_MAP[_iThreadID.xy] += g_fDeltaTime * vBrushColor.a;
    
    // cos 그래프로 높이 설정
    //float vDist = (distance(vCenterPos, _iThreadID.xy) / vScale) * 3.1415926535f;        
    //HEIGHT_MAP[_iThreadID.xy] += saturate(g_dt * cos(vDist));    
}

#endif