#include "PlayScene.h"
#include "Ground.h"
#include "Playable.h"
#include "Reception.h"
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
	Instantiate<Playable>(this);
	Instantiate<Reception>(this);
	Instantiate<Ground>(this);
}

void PlayScene::Update()
{
}

void PlayScene::Release()
{
}
