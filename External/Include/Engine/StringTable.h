#pragma once

// Message
#define STR_MSG_FailDeviceInitializing _T("��ġ �ʱ�ȭ ����")
#define STR_MSG_InitError _T("�ʱ�ȭ ����")
#define STR_MSG_FailedToCreateDeviceObject _T("��ġ��ü ���� ����")
#define STR_MSG_OccurError _T("���� �߻�")
#define STR_MSG_FailSwapChain _T("����ü�� ����")
#define STR_MSG_MultiSampleSupportNotAvailable _T("��Ƽ ���� ���� �Ұ�")
#define STR_MSG_FailCreateSwapChain _T("����ü�� ���� ����")
#define STR_MSG_FailCreateView _T("�� ���� ����")
#define STR_MSG_FailDuplicateResourceKey _T("���ҽ� Ű �ߺ� �߻�")
#define STR_MSG_FailedToLoadResource _T("���ҽ� �ε� ����")
#define STR_MSG_FailedToLoadTexture _T("�ؽ��� �ε� ����")
#define STR_MSG_FailedToCreateShaderResourceView _T("���̴� ���ҽ� �� ���� ����")

  // Ascii code character set
#define STR_MSG_FailedToCreateVertexShader "���ؽ� ���̴� ���� ����"
#define STR_MSG_FailedToCreatePixelShader "�ȼ� ���̴� ���� ����"
#define STR_MSG_FailedToCreateComputeShader "��ǻƮ ���̴� ���� ����"


/////////////// Path ///////////////
#define STR_FILE_PATH_Content _T("\\bin\\content\\")
#define STR_FILE_PATH_Shader _T("shader\\std2d.fx")

// Path Test
#define STR_FILE_PATH_TestComputeShader _T("shader\\testShader.fx")

// std2.fx functions name
#define STR_FUNC_NAME_VTXShader "VS_Std2D"
#define STR_FUNC_NAME_PIXShader "PS_Std2D"
#define STR_FUNC_NAME_VTXShaderCollider2D "VS_Collider2D"
#define STR_FUNC_NAME_PIXShaderCollider2D "PS_Collider2D"
#define STR_FUNC_NAME_VTXShaderLight2D "VS_Std2D_Light2D"
#define STR_FUNC_NAME_PIXShaderLight2D "PS_Std2D_Light2D"
#define STR_FUNC_NAME_VTXShaderTileMap "VS_TileMap"
#define STR_FUNC_NAME_PIXShaderTileMap "PS_TileMap"

////////// Resource Key ///////////////
// Mesh
#define STR_KEY_RectMesh _T("RectMash")
#define STR_KEY_RectLineMesh _T("RectLineMesh")
#define STR_KEY_CubeMesh _T("CubeMesh")
#define STR_KEY_CircleMesh _T("CircleMesh")
#define STR_KEY_CircleLineMesh _T("CircleLineMesh")

// Shader
#define STR_KEY_StdAlphaBlend_CoverageShader _T("StandardShaderAlphaBlendCoverage")
#define STR_KEY_StdAlphaBlendShader _T("StandardShaderAlphaBlend")
#define STR_KEY_StdLight2DShader _T("StandardShaderLight2D")
#define STR_KEY_Collider2DShader _T("Collider2DShader")
#define STR_KEY_TileMapShader _T("TileMapShader")

 /////// / // Test ///// / / /
#define STR_KEY_TestComputeShader _T("TestComputeShader")


// Material
#define STR_KEY_StdAlphaBlend_CoverageMtrl _T("StandardMaterialAlphaBlendCoverage")
#define STR_KEY_StdAlphaBlendMtrl _T("StandardMaterialAlphaBlend")
#define STR_KEY_StdLight2DMtrl _T("StandardMaterialLight2D")
#define STR_KEY_Collider2DMtrl _T("Collider2DMaterial")
#define STR_KEY_TileMapMtrl _T("TileMapMaterial")

// Resource View Key
#define STR_ResourceKey_DSVTexture _T("DepthStencilTexture")

////////// Shooting Game //////////
// GameObject name
#define STR_OBJ_NAME_GameManager _T("ShootingGameManager")
#define STR_OBj_NAME_Bullet _T("Bullet")
#define STR_OBJ_NAME_Enemy _T("Enemy")

#define STR_PATH_Box _T("texture\\Box.png")
#define STR_PATH_Player _T("texture\\Player.png")
#define STR_PATH_Monster _T("texture\\Monster.bmp")
#define STR_PATH_Enemy1 _T("texture\\Enemy1.png")
#define STR_PATH_Enemy2 _T("texture\\Enemy2.png")
#define STR_PATH_Enemy3 _T("texture\\Enemy3.png")
#define STR_PATH_Enemy4 _T("texture\\Enemy4.png")
#define STR_PATH_Anim _T("texture\\link_0.png")