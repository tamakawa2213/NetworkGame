#include "ResultScene.h"
#include "Engine/Picture.h"
#include "Observer.h"

ResultScene::ResultScene(GameObject* parent)
	: GameObject(parent, "ResultScene"), hPict_(-1)
{
}

ResultScene::~ResultScene()
{
}

void ResultScene::Initialize()
{
	if (Observer::GetWinner())
		hPict_ = Picture::Load(L"Assets\\Win.png");
	else
		hPict_ = Picture::Load(L"Assets\\Lose.png");

	assert(hPict_ >= 0);
}

void ResultScene::Update()
{
}

void ResultScene::Draw()
{
	Picture::Draw(hPict_);
}
