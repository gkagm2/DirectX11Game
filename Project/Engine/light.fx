#ifndef _LIGHT_FX
#define _LIGHT_FX
#include "value.fx"
#include "function.fx"

///////////////////////////////////
// Direction Light Render Shader
// mesh : RectMesh
#define ViewNormalTargetTex     g_tex_0
#define ViewPosTargetTex        g_tex_1
#define ShadowDepthTargetTex    g_tex_2 // dir ���� �������� ��ü�� ���̸� �׸� Ÿ�� �ؽ���
#define iLightIdx               g_int_0
#define DirCamViewProjMat       g_mat_0 // ���� ī�޶�� �����ϱ� ���� View * Proj ���
///////////////////////////////////


struct VS_IN
{
    float3 vPos : POSITION;
};

struct VS_OUT
{
    float4 vPos : SV_Position; // SV_Position���� �ϸ� ������ ���� �����°� �ƴ϶� ������ �ȼ� ��ǥ���� ����.
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
    output.vPos = float4(_in.vPos.xy * 2.f, 0.f, 1.f); // ��ũ�� ��ǥ ��ü
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
        // �ش� ������ �׸��ڰ� ���ܾ� ���� �Ǵ��ϱ�    
        float3 vWorldPos = mul(float4(vViewPos, 1.f), g_matViewInv).xyz; // ���� ���� ������ ������� ��ǥ    
        float4 vDirCamProj = mul(float4(vWorldPos, 1.f), DirCamViewProjMat);
        float fCurDepth = vDirCamProj.z / vDirCamProj.w;
    
        // w �� ������ Proj ��ǥ�� ���� �ڿ�, ������ǥ�� �ش��ϴ� UV ������ ȯ��
        vDirCamProj.xy /= vDirCamProj.w;
        float2 vDepthTargetUV = float2((vDirCamProj.x + 1.f) / 2.f, (-vDirCamProj.y + 1.f) / 2.f);
        
        // ���� �������� �ش� ������ ������ �ʴ� ���
        if (vDepthTargetUV.x < 0.f || 1.f < vDepthTargetUV.x
            || vDepthTargetUV.y < 0.f || 1.f < vDepthTargetUV.y)
        {
            return output;
        }
    
        // ���� UV �� �̿��ؼ� ���� ������ ���� Ÿ�ٿ��� ����Ǿ��ִ� ���� ���� ����
        float fTargetDepth = ShadowDepthTargetTex.Sample(Sample_Anisotropic, vDepthTargetUV).r;
    
        if (fTargetDepth + 0.002f < fCurDepth)
        {
            // �׸���       
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
    
    // Point Target���� ���� �ش� ������ �����ϴ� ��ü�� ��ġ�� Ȯ���Ѵ�.
    float2 vUV = _in.vPos.xy / g_vResolution.xy;
    float3 vViewPos = ViewPosTargetTex.Sample(Sample_Point, vUV).xyz;
    
    if (0.f == vViewPos.z)
        clip(-1);
    
    // Decal Test
    float3 vWorldPos = mul(float4(vViewPos, 1.f), g_matViewInv).xyz;
    float3 vLocalPos = mul(float4(vWorldPos, 1.f), g_matWorldInv).xyz;
    
    // ���� �������� ���ο� ���� �ʾ����� �����޽� �ܺο� �ִ� ��ǥ��.
    if (length(vLocalPos) > 0.5f) // Sphere Mesh�� Local Space������ ������
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

    return output;
}

PS_OUT PS_SpotLight(VS_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;
       
    // Point Target���� ���� �ش� ������ �����ϴ� ��ü�� ��ġ�� Ȯ���Ѵ�.
    float2 vUV = _in.vPos.xy / g_vResolution.xy;
    float3 vViewPos = ViewPosTargetTex.Sample(Sample_Point, vUV).xyz;
    
    if (0.f == vViewPos.z)
        clip(-1);
    
    // Decal Test
    float3 vWorldPos = mul(float4(vViewPos, 1.f), g_matViewInv).xyz;
    float3 vLocalPos = mul(float4(vWorldPos, 1.f), g_matWorldInv).xyz;
    
    // ���� �������� ���ο� ���� �ʾ����� �����޽� �ܺο� �ִ� ��ǥ��.
    if (length(vLocalPos) > 0.5f) // Sphere Mesh�� Local Space������ ������
        clip(-1);
    
    float3 vViewNormal = ViewNormalTargetTex.Sample(Sample_Point, vUV).xyz;
    
    TLightColor color = CalLight3D(iLightIdx, vViewPos, vViewNormal);
    
    //TLightColor tLight = (TLightColor) 0.f;
    //TLightInfo tInfo = g_Light3DBuffer[_iLightIdx];
    
    output.vDiffuseLight.xyz = color.vDiffuse.xyz + color.vAmbient.xyz;
    output.vSpecularLight.xyz = color.vSpeculer.xyz;
    output.vDiffuseLight.w = 1.f;
    output.vSpecularLight.w = 1.f;
    
    return output;
}
#endif