#pragma once
#include "GameObject.h"

enum SCENE_ID
{
	SCENE_ID_PLAY = 0,
	SCENE_ID_RESULT
};

class SceneManager : public GameObject
{
	unsigned int CurrentSceneID_;
	unsigned int NextSceneID_;
public:
	SceneManager(GameObject* parent);
	~SceneManager();

	void Initialize() override;
	void Update() override;

	void ChangeScene(unsigned int SCENE_ID) { NextSceneID_ = SCENE_ID; }
};