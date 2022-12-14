#include "PlayScene.h"
#include "Ground.h"
#include "Player.h"
#include "Storage.h"

PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene")
{
}

PlayScene::~PlayScene()
{
}

void PlayScene::Initialize()
{
	Instantiate<Player>(this);
	Instantiate<Ground>(this);
}

void PlayScene::Update()
{
}

void PlayScene::Release()
{
}
