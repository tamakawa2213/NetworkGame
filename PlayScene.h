#pragma once
#include "Engine/GameObject.h"

enum
{
	EASY,
	HARD
};

class PlayScene : public GameObject
{
public:
	PlayScene(GameObject* parent);

	~PlayScene();

	void Initialize() override;

	void Update() override;

	void Release() override;
};