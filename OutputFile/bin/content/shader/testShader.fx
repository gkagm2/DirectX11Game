#ifndef _TESTSHADER
#define _TESTSHADER

#include "value.fx"
#include "struct.fx"

// Texture
// �ػ�
// color

// �׷� �� ������ ���� ����
// 1024 ����

// CS ���� Semantic

// SV_GroupID           : �׷� �ε���(��ü �׷� �߿� ����� �׷�����)
// SV_GroupThreadID     : �׷� �������� ������ �ε���(3����)
// SV_GroupIndex        : �׷� �������� ������ �ε����� 1�������� ǥ��
// SV_DispatchThreadID  : ��ü ������ ����, �ε��� ��(3����)
RWTexture2D<float4> g_rwTex : register(u0); // unordered access

[numthreads(32, 32, 1)]
void CS_Test(int3 _iThreadID : SV_DispatchThreadID)
{
    if(g_int_0 <= _iThreadID.x || g_int_1 <= _iThreadID.y)
        return;
    
    g_rwTex[_iThreadID.xy] = g_vec4_0;
}

#endif