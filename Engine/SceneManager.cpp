#include "SceneManager.h"
#include "Model.h"
#include "../PlayScene.h"
#include "../ResultScene.h"

SceneManager::SceneManager(GameObject* parent) : GameObject(parent, "SceneManager")
{
	CurrentSceneID_ = SCENE_ID_PLAY;
	NextSceneID_ = CurrentSceneID_;
}

SceneManager::~SceneManager()
{
}

void SceneManager::Initialize()
{
	Instantiate<PlayScene>(this);
}

void SceneManager::Update()
{
	if (CurrentSceneID_ != NextSceneID_)
	{
		KillAllChildren();
		Model::Release();

		switch (NextSceneID_)
		{
		case SCENE_ID_PLAY: Instantiate<PlayScene>(this); break;
		case SCENE_ID_RESULT: Instantiate<ResultScene>(this); break;
		}

		CurrentSceneID_ = NextSceneID_;
	}
	
}