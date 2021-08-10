#pragma once
class CTestScene
{
public:
	static void CreateTestScene();

protected:
	static void SceneStart();					// Done
	static void Camera();						// Done
	static void RenderingBoxObject();			// Done
	static void RenderingAnimationTexture();	// Done
	static void Collision2DTest();				// Done
	static void Light2DTest();					// Done
	static void TileMapTest();					// Done
	static void ParticleSystemTest();			// Done
	static void PrefabRegisterTest();			// Done
	static void FileSaveLoadTest();				// Done
	static void SceneSaveLoadTest();			// Done
	static void MouseMovementTest();
	static void ImGuiTest();					// 하는중
	static void DistortionObject();				// PostEffect를 이용
	static void FishEyePostEffect();			// PostEffect



	// TODO (Jang) : 자식오브젝트가 있는 상태에서 로 Scene Stop 시 오류걸리는거 수정하기
	static void MaterialCreateTest();			// Done

	static void MultiThreadScene_LoadingScene();
	


	static void SceneSaveLoadPrefabTest();// TODO (Jang) : 프리펩 RelativePath가 없음.

	static void PlayerSaveLoad();
	

	static void ChildObjectTest();				
	static void AnimationSaveLoad();
	
	static void TextureTest();
	
	// TODO (Jang) : string 데이터를 읽게되면 제대로 안읽어짐. Scene Save, Load가 안됨. 

private:
	CTestScene();
	~CTestScene();
};