#ifndef _DEBUG_FX
#define _DEBUG_FX

#include "value.fx"
// =======================
// Debug Object Shader
#define vObjColor g_vec4_0 
// =======================
float4 VS_Debug3D(float3 _vPos : POSITION) : SV_Position
{
    float4 vProjPos = (float4) 0.f;
        
    vProjPos = mul(float4(_vPos, 1.f), g_matWorldViewProj);

    return vProjPos;
}

float4 PS_Debug3D() : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    vColor = vObjColor;
    
    return vColor;
}


#endif
