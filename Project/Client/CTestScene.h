#pragma once
class CTestScene
{
public:
	static void CreateTestScene();


protected:

#ifdef _CAPTAIN_FOREVER_GAME
	static void CaptainForever();
#endif

#ifdef _BUTCHER_GAME
	static void Butcher();
#endif

#ifdef _SHOOTING2D_GAME
	static void Shooting2D();
#endif

protected:
	static void UpdateOldVersion();

protected:
	static void SceneStart();					// Done
	static void Camera();						// Done
	static void RenderingBoxObject();			// Done
	static void RenderingAnimationTexture();	// Done
	static void Collision2DTest();				// Done
	static void Collision2DTest2();				// Done
	static void Collision2DMouseAndRect();
	static void Light2DTest();					// Done
	static void TileMapTest();					// Done
	static void ParticleSystemTest();			// Done
	static void ParticleSystemTest2();			
	static void PrefabRegisterTest();			// Done
	static void FileSaveLoadTest();				// Done
	static void SceneSaveLoadTest();			// Done
	static void MouseMovementTest();
	static void ImGuiTest();					// �ϴ���
	static void DistortionObject();				// PostEffect�� �̿�
	static void FishEyePostEffect();			// PostEffect
	static void BlurEffect();					// Animation ���̴�	
	static void PaperBurnPostEffect();			// �̿ϼ�
	static void SoundTest();					// Done
	static void FontRendering();				// Done

	static void Light3DTest();
	static void FBXLoadingTest();

	static void Test();

	// TODO (Jang) : �ڽĿ�����Ʈ�� �ִ� ���¿��� �� Scene Stop �� �����ɸ��°� �����ϱ�
	static void MaterialCreateTest();			// Done

	static void MultiThreadScene_LoadingScene();
	


	static void SceneSaveLoadPrefabTest();// TODO (Jang) : ������ RelativePath�� ����.

	static void PlayerSaveLoad();
	

	static void ChildObjectTest();				
	static void AnimationSaveLoad();
	
	static void TextureTest();
	
	// TODO (Jang) : string �����͸� �аԵǸ� ����� ���о���. Scene Save, Load�� �ȵ�. 

public:
	static void CreateNewScene();

private:
	CTestScene();
	~CTestScene();
};