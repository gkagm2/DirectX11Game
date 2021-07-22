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
	static void ImGuiTest();

	static void SceneSaveLoadPrefabTest();// TODO (Jang) : ������ RelativePath�� ����.

	static void PlayerSaveLoad();

	static void ChildObjectTest();				
	static void AnimationSaveLoad();
	
	static void TextureTest();
	
	
	
	// TODO (Jang) : string �����͸� �аԵǸ� ����� ���о���. Scene Save, Load�� �ȵ�. 
	
	


private:
	CTestScene();
	~CTestScene();
};