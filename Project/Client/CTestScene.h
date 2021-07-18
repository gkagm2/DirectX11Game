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

	static void ChildObjectTest();				
	static void AnimationSaveLoad();
	static void PlayerSaveLoad();
	static void TextureTest();
	
	
	
	// TODO (Jang) : string �����͸� �аԵǸ� ����� ���о���. Scene Save, Load�� �ȵ�. 
	static void SceneSaveLoadTest();
	static void FileSaveLoadTest();


private:
	CTestScene();
	~CTestScene();
};