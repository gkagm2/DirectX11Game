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
	
	
	
	// TODO (Jang) : string 데이터를 읽게되면 제대로 안읽어짐. Scene Save, Load가 안됨. 
	static void SceneSaveLoadTest();
	static void FileSaveLoadTest();


private:
	CTestScene();
	~CTestScene();
};