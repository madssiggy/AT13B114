#pragma once


class CManager
{
private:
	static class CScene* m_Scene;
	
	static class CFade* m_Fade;

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static class CScene* GetScene() { return m_Scene; }

	static void StartFadeIn(int Frame);
	static void StartFadeOut(int Frame);
	static bool IsFade();

	template<typename T>
	static void SetScene()
	{
		if (m_Scene != NULL)
		{
			m_Scene->Uninit();
			delete m_Scene;
		}

		T* scene = new T();
		m_Scene = scene;
		scene->Init();
	}

};