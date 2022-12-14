#pragma once
#include "GameObject.h"

class GravityInfluence : public GameObject
{
	const XMVECTOR GrAccel_ = { 0,0.01f,0,0 };	//重力加速度
protected:
	bool OnGround_;			//地面と接しているかどうか
	XMVECTOR Vertical_;		//縦向きのベクトル
	void  Falldown();		//落下処理
public:
	//コンストラクタ
	GravityInfluence(GameObject* parent);
	//継承用コンストラクタ
	GravityInfluence(GameObject* parent, std::string name);
	~GravityInfluence();
};