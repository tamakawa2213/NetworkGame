#pragma once
#include "Engine/GravityInfluence.h"

//◆◆◆を管理するクラス
class Player : public GravityInfluence
{
    //定数
    const float    RUN_SPEED;           //移動速度
    const float    ROTATE_SPEED;        //回転速度
    const XMFLOAT3 FIXED_CAM_POS;       //固定カメラの位置
    const XMFLOAT3 FIX_TANK_POS;        //Tankの位置を修正

    int hModel_;    //モデル番号
    const float Jump_ = 0.4f;     //ジャンプ力
public:
    //コンストラクタ
    Player(GameObject* parent);

    //デストラクタ
    ~Player();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;
};