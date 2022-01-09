#pragma once

// Message
#define STR_MSG_FailDeviceInitializing _T("장치 초기화 실패")
#define STR_MSG_InitError _T("초기화 오류")
#define STR_MSG_FailedToCreateDeviceObject _T("장치객체 생성 실패")
#define STR_MSG_OccurError _T("오류 발생")
#define STR_MSG_FailSwapChain _T("스왑체인 실패")
#define STR_MSG_MultiSampleSupportNotAvailable _T("멀티 샘플 지원 불가")
#define STR_MSG_FailCreateSwapChain _T("스왑체인 생성 실패")
#define STR_MSG_FailCreateView _T("뷰 생성 실패")
#define STR_MSG_FailDuplicateResourceKey _T("리소스 키 중복 발생")
#define STR_MSG_FailedToLoadResource _T("리소스 로딩 실패")
#define STR_MSG_FailedToLoadTexture _T("텍스쳐 로딩 실패")
#define STR_MSG_FailedToCreateShaderResourceView _T("쉐이더 리소스 뷰 생성 실패")

  // Ascii code character set
#define STR_MSG_FailedToCreateVertexShader "버텍스 쉐이더 생성 실패"
#define STR_MSG_FailedToCreatePixelShader "픽셀 쉐이더 생성 실패"
#define STR_MSG_FailedToCreateComputeShader "컴퓨트 쉐이더 생성 실패"
#define STR_MSG_FailedToCreateGeometryShader "지오메트리 쉐이더 생성 실패"


/////////////// Path ///////////////
#define STR_FILE_PATH_Content _T("\\bin\\content\\")
#define STR_FILE_PATH_Shader _T("shader\\std2d.fx")
#define STR_FILE_PATH_TileMapShader _T("shader\\tilemap.fx")
#define STR_FILE_PATH_ParticleUpdateShader _T("shader\\particle_update.fx")
#define STR_FILE_PATH_ParticleShader _T("shader\\particle.fx")
#define STR_FILE_PATH_Material _T("material\\")
#define STR_FILE_PATH_PostEffectShader _T("shader\\posteffect.fx")

// Resource의 key이자 path임
#define STR_FILE_PATH_NoiseTexture1 _T("texture\\noise\\noise_01.png")
#define STR_FILE_PATH_NoiseTexture2 _T("texture\\noise\\noise_02.png")


// Scene path
#define STR_FILE_PATH_TempScene _T("scene\\tempScene.scene")
#define STR_FILE_PATH_LoadingScene _T("scene\\loadingScene.scene")

// Path Test
#define STR_FILE_PATH_TestComputeShader _T("shader\\testShader.fx")

// Directory path
#define STR_DIR_PATH_Scene _T("scene\\")
#define STR_DIR_PATH_Texture _T("texture\\")
#define STR_DIR_PATH_Prefab _T("prefab\\")
#define STR_DIR_PATH_Shader _T("shader\\")
#define STR_DIR_PATH_Material _T("material\\")
#define STR_DIR_PATH_Anim _T("anim\\")

#define STR_DIR_PATH_Config _T("config\\") // 엔진 환경

// Extension
#define STR_EXTENSION_Anim _T(".anim")
#define STR_EXTENSION_Mtrl _T(".mtrl")
#define STR_EXTENSION_Prefab _T(".pref")
#define STR_EXTENSION_SceneData _T(".sdata")

// std2.fx functions name
#define STR_FUNC_NAME_VTXShader "VS_Std2D"
#define STR_FUNC_NAME_PIXShader "PS_Std2D"
#define STR_FUNC_NAME_VTXShaderCollider2D "VS_Collider2D"
#define STR_FUNC_NAME_PIXShaderCollider2D "PS_Collider2D"
#define STR_FUNC_NAME_VTXShaderLineStrip "VS_LineStrip"
#define STR_FUNC_NAME_PIXShaderLineStrip "PS_LineStrip"
#define STR_FUNC_NAME_VTXShaderLight2D "VS_Std2D_Light2D"
#define STR_FUNC_NAME_PIXShaderLight2D "PS_Std2D_Light2D"
#define STR_FUNC_NAME_VTXShaderTileMap "VS_TileMap"
#define STR_FUNC_NAME_PIXShaderTileMap "PS_TileMap"
#define STR_FUNC_NAME_VTXCanvasShader "VS_Canvas"
#define STR_FUNC_NAME_PIXCanvasShader "PS_Canvas"
#define STR_FUNC_NAME_VTXFog2DShader "VS_Fog2D"
#define STR_FUNC_NAME_PIXFog2DShader "PS_Fog2D"
#define STR_FUNC_NAME_VTXButtonUIShader "VS_ButtonUI"
#define STR_FUNC_NAME_PIXButtonUIShader "PS_ButtonUI"
#define STR_FUNC_NAME_VTXGridShader "VS_Grid"
#define STR_FUNC_NAME_PIXGridShader "PS_Grid"
#define STR_FUNC_NAME_VTXLineShader "VS_Line"
#define STR_FUNC_NAME_PIXLineShader "PS_Line"


// particle.fx functions name
#define STR_FUNC_NAME_VTX_Particle "VS_Particle"
#define STR_FUNC_NAME_GEO_Particle "GS_Particle"
#define STR_FUNC_NAME_PIX_Particle "PS_Particle"

// particle_update.fx functions name
#define STR_FUNC_NAME_ParticleUpdate "CS_ParticleUpdate"

// posteffect.fx functions name
#define STR_FUNC_NAME_VTX_Distortion "VS_Distortion"
#define STR_FUNC_NAME_PIX_Distortion "PS_Distortion"
#define STR_FUNC_NAME_VTX_FishEye "VS_FishEye"
#define STR_FUNC_NAME_PIX_FishEye "PS_FishEye"
#define STR_FUNC_NAME_VTX_Blur "VS_Blur"
#define STR_FUNC_NAME_PIX_Blur "PS_Blur"


////////// Resource Key ///////////////
// Mesh
#define STR_KEY_PointMesh _T("PointMesh")
#define STR_KEY_RectMesh _T("RectMesh")
#define STR_KEY_RectLineMesh _T("RectLineMesh")
#define STR_KEY_CubeMesh _T("CubeMesh")
#define STR_KEY_CircleMesh _T("CircleMesh")
#define STR_KEY_CircleLineMesh _T("CircleLineMesh")
#define STR_KEY_LineStripMesh _T("LineStriptMesh")
#define STR_KEY_LineListMesh _T("LineListMesh")

// Shader
#define STR_KEY_StdAlphaBlend_CoverageShader _T("StandardShaderAlphaBlendCoverage")
#define STR_KEY_StdAlphaBlendShader _T("StandardShaderAlphaBlend")
#define STR_KEY_StdLineStripShader _T("LineStripShader")
#define STR_KEY_StdLight2DShader _T("StandardShaderLight2D")
#define STR_KEY_Collider2DShader _T("Collider2DShader")
#define STR_KEY_TileMapShader _T("TileMapShader")
#define STR_KEY_ParticleShader _T("ParticleShader")
#define STR_KEY_DistortionShader _T("DistortionShader")
#define STR_KEY_FishEyeShader _T("FishEyeShader")
#define STR_KEY_BlurShader _T("BlurShader")
#define STR_KEY_CanvasShader _T("CanvasShader")
#define STR_KEY_Fog2DShader _T("Fog2DShader")
#define STR_KEY_ButtonUIShader _T("ButtonUIShader")
#define STR_KEY_GridShader _T("GridShader")
#define STR_KEY_LineListShader _T("LineListShader")

// compute shader
#define STR_KEY_ParticleUpdateShader _T("ParticleUpdateShader")

 /////// / // Test ///// / / /
#define STR_KEY_TestComputeShader _T("TestComputeShader")
#define STR_FILE_PATH_Prefab _T("prefab\\")

// Material
#define STR_KEY_StdAlphaBlend_CoverageMtrl _T("StandardMaterialAlphaBlendCoverage")
#define STR_KEY_StdAlphaBlendMtrl _T("StandardMaterialAlphaBlend")
#define STR_KEY_LineStripMtrl _T("LineStripLMaterial")
#define STR_KEY_LineListMtrl _T("LineListMaterial")
#define STR_KEY_StdLight2DMtrl _T("StandardMaterialLight2D")
#define STR_KEY_Collider2DNoneColliedMtrl _T("Collider2DNonColliedMaterial") // 충돌되지 않았을 시 나타내는 메터리얼
#define STR_KEY_Collider2DCollisionMtrl _T("Collider2DCollisionMaterial") // 충돌 시 나타내는 메터리얼 
#define STR_KEY_TileMapMtrl _T("TileMapMaterial")
#define STR_KEY_ParticleMtrl _T("ParticleMaterial")
#define STR_KEY_DistortionMtrl _T("DistortionMaterial")
#define STR_KEY_FishEyeMtrl _T("FishEyeMaterial")
#define STR_KEY_BlurMtrl _T("BlurMaterial")
#define STR_KEY_PaperBurnMtrl _T("PaperBurnMaterial")
#define STR_KEY_CanvasMtrl _T("CanvasMaterial")
#define STR_KEY_Fog2DMtrl _T("Fog2DMaterial")
#define STR_KEY_ButtonUIMtrl _T("ButtonUIMaterial")
#define STR_KEY_GridMtrl _T("GridMaterial")

// Resource View Key
#define STR_ResourceKey_DSVTexture _T("DepthStencilTexture")
#define STR_ResourceKey_RTTexture _T("RenderTargetTexture")
#define STR_ResourceKey_PostEffectTargetTexture _T("PostEffectTargetTexture")


// Tool Object name
#define STR_TOOL_OBJ_NAME_ToolCamera _T("Tool Camera_Engine")
#define STR_TOOL_OBJ_NAME_ToolUICamera _T("Tool UI Camera_Engine")
#define STR_TOOL_OBJ_NAME_ToolOrthogonalAxisCoordinate _T("Tool OrthogonalAxisCoordinate")

///////////////////////
// GameObject name
#define STR_OBJ_NAME_UICamera _T("UI Camera")
#define STR_OBJ_NAME_UICanvas _T("UI Canvas")

#define STR_OBJ_NAME_Cursor _T("__Cursor")

////////// Shooting Game //////////
// GameObject name
#define STR_OBJ_NAME_GameManager _T("ShootingGameManager")
#define STR_OBj_NAME_Bullet _T("Bullet")
#define STR_OBJ_NAME_Enemy _T("Enemy")

#define STR_PATH_Box _T("texture\\Box.png")
#define STR_PATH_Player _T("texture\\Player.png")
#define STR_PATH_PlayerAnim _T("texture\\Player_Anim.png")
#define STR_PATH_Monster _T("texture\\Monster.bmp")
#define STR_PATH_Enemy1 _T("texture\\Enemy1.png")
#define STR_PATH_Enemy2 _T("texture\\Enemy2.png")
#define STR_PATH_Enemy3 _T("texture\\Enemy3.png")
#define STR_PATH_Enemy4 _T("texture\\Enemy4.png")
#define STR_PATH_Anim _T("texture\\link_0.png")
#define STR_PATH_Fog2D _T("texture\\Fog2D.png")


//////////////// Butcher string table
// Animation Name
#define BUTCHER_Anim_Player_Dead _T("Player_Dead_Bu")

// Object Name
#define BUTCHER_ObjName_Muzzle _T("Muzzle")
#define BUTCHER_ObjName_RotationPos _T("GunRotationPos")
#define BUTCHER_ObjName_WayPoint_bu _T("WayPoint_bu")