#pragma once
#include <string>
#include <vector>
#include "Sprite.h"
#include "Transform.h"

struct PictureSet
{
	Sprite* pSprite = nullptr;	//Spriteのポインタ
	Transform transform;		//transformクラス
	LPCWSTR FileName = L"";		//ファイルの名前
	bool FindFbx = false;		//Fbxファイルを事前にロードしているか
};

namespace Picture
{
	int Load(LPCWSTR filename);
	void SetTransform(int hModel, Transform transform);
	void Draw(int hModel);

	void Release();
}