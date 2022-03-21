#ifndef _LIGHT_FX
#define _LIGHT_FX
#include "value.fx"
#include "function.fx"

///////////////////////////////////
// Direction Light Render Shader
// mesh : RectMesh
#define ViewNormalTargetTex     g_tex_0
#define ViewPosTargetTex        g_tex_1
#define ShadowDepthTargetTex    g_tex_2 // dir 광원 시점에서 물체의 깊이를 그린 타겟 텍스쳐
#define iLightIdx               g_int_0
#define DirCamViewProjMat       g_mat_0 // 광원 카메라로 투영하기 위한 View * Proj 행렬
///////////////////////////////////


struct VS_IN
{
    float3 vPos : POSITION;
};

struct VS_OUT
{
    float4 vPos : SV_Position; // SV_Position으로 하면 보간된 값이 들어오는게 아니라 본인의 픽셀 좌표값이 들어옴.
};

struct PS_OUT
{
    float4  vDiffuseLight : SV_Target0;
    float4  vSpecularLight : SV_Target1;
    float   fShadowPow : SV_Target2;
};

VS_OUT VS_DirLight(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    output.vPos = float4(_in.vPos.xy * 2.f, 0.f, 1.f); // 스크린 좌표 전체
    return output;
}

PS_OUT PS_DirLight(VS_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;
 
    float2 vUV = _in.vPos.xy / g_vResolution.xy;
    float3 vViewPos = ViewPosTargetTex.Sample(Sample_Anisotropic, vUV).xyz;
    if (0.f == vViewPos.z)
    {
        clip(-1);
    }

    float3 vViewNormal = ViewNormalTargetTex.Sample(Sample_Anisotropic, vUV).xyz;
    TLightColor color = CalLight3D(iLightIdx, vViewPos, vViewNormal);
    
    output.vDiffuseLight.xyz = color.vDiffuse.xyz + color.vAmbient.xyz;
    output.vSpecularLight.xyz = color.vSpeculer.xyz;
    output.vDiffuseLight.w = 1.f;
    output.vSpecularLight.w = 1.f;
    
    if (dot(color.vDiffuse.xyz, color.vDiffuse.xyz) != 0.f)
    {
        // 해당 지점이 그림자가 생겨야 할지 판단하기    
        float3 vWorldPos = mul(float4(vViewPos, 1.f), g_matViewInv).xyz; // 빛을 받을 지점의 월드상의 좌표    
        float4 vDirCamProj = mul(float4(vWorldPos, 1.f), DirCamViewProjMat);
        float fCurDepth = vDirCamProj.z / vDirCamProj.w;
    
        // w 로 나눠서 Proj 좌표를 얻은 뒤에, 투영좌표에 해당하는 UV 값으로 환산
        vDirCamProj.xy /= vDirCamProj.w;
        float2 vDepthTargetUV = float2((vDirCamProj.x + 1.f) / 2.f, (-vDirCamProj.y + 1.f) / 2.f);
            
        float fTargetDepth = 0.f;

        // 광원 시점에서 해당 지점이 보이는 경우
        if (0.f <= vDepthTargetUV.x && 1.f >= vDepthTargetUV.x &&
            0.f <= vDepthTargetUV.y && 1.f >= vDepthTargetUV.y)
        {
            // 구한 UV 를 이용해서 광원 쉐도우 뎁스 타겟에서 저장되어있는 깊이 값을 추출
            fTargetDepth = ShadowDepthTargetTex.Sample(Sample_Anisotropic, vDepthTargetUV).r;
        }
        
        if (0.f == fTargetDepth)
            return output;
    
        if (fTargetDepth + 0.002f < fCurDepth)
        {
            // 그림자       
            output.fShadowPow = 0.8f;
        }
    }
    return output;
}

// ===============================
// Point Light Render Shader
// mesh : SphereMesh
// ViewNormalTargetTex  g_tex_0
// ViewPosTargetTex     g_tex_1
// iLightIdx            g_int_0
// ===============================
VS_OUT VS_PointLight(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    output.vPos = mul(float4(_in.vPos, 1.f), g_matWorldViewProj);
    return output;
}

PS_OUT PS_PointLight(VS_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;
    
    // Point Target으로 부터 해당 지점에 존재하는 물체의 위치를 확인한다.
    float2 vUV = _in.vPos.xy / g_vResolution.xy;
    float3 vViewPos = ViewPosTargetTex.Sample(Sample_Point, vUV).xyz;
    
    if (0.f == vViewPos.z)
        clip(-1);
    
    // Decal Test
    float3 vWorldPos = mul(float4(vViewPos, 1.f), g_matViewInv).xyz;
    float3 vLocalPos = mul(float4(vWorldPos, 1.f), g_matWorldInv).xyz;
    
    // 로컬 영역에서 내부에 있지 않았으면 볼륨메쉬 외부에 있는 좌표임.
    if (length(vLocalPos) > 0.5f) // Sphere Mesh의 Local Space에서의 반지름
        clip(-1);
    
    float3 vViewNormal = ViewNormalTargetTex.Sample(Sample_Point, vUV).xyz;
    
    TLightColor color = CalLight3D(iLightIdx, vViewPos, vViewNormal);
    
    output.vDiffuseLight.xyz = color.vDiffuse.xyz + color.vAmbient.xyz;
    output.vSpecularLight.xyz = color.vSpeculer.xyz;
    output.vDiffuseLight.w = 1.f;
    output.vSpecularLight.w = 1.f;
    return output;
}

VS_OUT VS_SpotLight(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    output.vPos = mul(float4(_in.vPos, 1.f), g_matWorldViewProj);
    return output;
}

PS_OUT PS_SpotLight(VS_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;
    
    // Point Target으로 부터 해당 지점에 존재하는 물체의 위치를 확인한다.
    float2 vUV = _in.vPos.xy / g_vResolution.xy;
    float3 vViewPos = ViewPosTargetTex.Sample(Sample_Point, vUV).xyz;
    
    if (0.f == vViewPos.z)
        clip(-1);
    
    // Decal Test
    float3 vWorldPos = mul(float4(vViewPos, 1.f), g_matViewInv).xyz;
    float3 vLocalPos = mul(float4(vWorldPos, 1.f), g_matWorldInv).xyz;
    
    // 로컬 영역에서 내부에 있지 않았으면 볼륨메쉬 외부에 있는 좌표임.
    if (length(vLocalPos) > 0.5f) // Sphere Mesh의 Local Space에서의 반지름
        clip(-1);
    
    float3 vViewNormal = ViewNormalTargetTex.Sample(Sample_Point, vUV).xyz;
    
    TLightColor color = CalLight3D(iLightIdx, vViewPos, vViewNormal);
    
    output.vDiffuseLight.xyz = color.vDiffuse.xyz + color.vAmbient.xyz;
    output.vSpecularLight.xyz = color.vSpeculer.xyz;
    output.vDiffuseLight.w = 1.f;
    output.vSpecularLight.w = 1.f;
    
    return output;
}
#endif