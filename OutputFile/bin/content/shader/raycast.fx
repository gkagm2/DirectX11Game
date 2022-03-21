#ifndef _RAYCAST_FX
#define _RAYCAST_FX
#include "value.fx"
#include "function.fx"

RWStructuredBuffer<TRaycastOut> g_tOutput : register(u0);

//--------------------------------
// Raycast Compute Shader
#define HeightMapTex g_tex_0
#define CamPos g_vec4_0
#define CamDir g_vec4_1

#define FaceX g_int_0
#define FaceZ g_int_1
//--------------------------------


[numthreads(32, 32, 1)]
void CS_Raycast(int3 _iThreadID : SV_DispatchThreadID)
{
    int2 id = _iThreadID.xy;
    
    float3 vPos[3] = { (float3) 0.f, (float3) 0.f, (float3) 0.f };
    
    if (0 == id.x % 2)
    {
        // ¾Æ·¡ÂÊ »ï°¢Çü        
        // 2
        // | \
        // 0--1        
        vPos[0].x = id.x / 2;
        vPos[0].z = id.y;
        
        vPos[1].x = vPos[0].x + 1;
        vPos[1].z = id.y;
        
        vPos[2].x = vPos[0].x;
        vPos[2].z = id.y + 1;
    }
    else
    {
        // À­ÂÊ »ï°¢Çü
        // 1--0
        //  \ |
        //    2  
        vPos[0].x = (id.x / 2) + 1;
        vPos[0].z = id.y + 1;
        
        vPos[1].x = vPos[0].x - 1;
        vPos[1].z = id.y + 1;
        
        vPos[2].x = vPos[0].x;
        vPos[2].z = id.y;
    }
    
    for (int i = 0; i < 3; ++i)
    {
        float2 uv = float2(saturate(vPos[i].x / (float) FaceX), saturate(1.f - vPos[i].z / (float) FaceZ));
        vPos[i].y = HeightMapTex.SampleLevel(Sample_Anisotropic, uv, 0).x;
    }
    
    float3 vCrossPoint = (float3) 0.f;
    float fDist = 0.f;
    
    if (IntersectsLay(vPos, CamPos.xyz, CamDir.xyz, vCrossPoint, fDist))
    {
        int iDist = (int) (999999.f * fDist);
        int iDistOut = 0;
        
        InterlockedMin(g_tOutput[0].iDist, iDist, iDistOut);
        
        if (iDistOut > iDist)
        {
            // ½ÇÆÐ
            return;
        }
        
        g_tOutput[0].vUV = float2(saturate(vCrossPoint.x / (float) FaceX), saturate(1.f - vCrossPoint.z / (float) FaceZ));
        g_tOutput[0].iSuccess = 1;
    }
}

#endif