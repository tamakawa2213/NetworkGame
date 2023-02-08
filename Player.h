#pragma once
#include "Engine/GravityInfluence.h"

class Player : public GravityInfluence
{
    //定数
    const float    RUN_SPEED;           //移動速度
    const float    ROTATE_SPEED;        //回転速度
    const XMFLOAT3 FIXED_CAM_POS;       //固定カメラの位置
    const float Jump_ = 0.4f;           //ジャンプ力

    int hModel_;    //モデル番号

    XMVECTOR vPos_;
    XMVECTOR vMove_;
    XMMATRIX mRotate_;

    bool Lose_;
protected:

    enum
    {
        COMMAND_ADV  = 0x01 << 0,       //前進
        COMMAND_BACK = 0x01 << 1,       //後退
        COMMAND_ROTATE_R = 0x01 << 2,   //右回転
        COMMAND_ROTATE_L = 0x01 << 3,   //左回転
        COMMAND_JUMP = 0x01 << 4       //ジャンプ
    };

    char Command_;  //コマンド
    virtual void SetCommand() = 0;

public:
    //コンストラクタ
    Player(GameObject* parent);

    //継承用コンストラクタ
    Player(GameObject* parent, std::string name);

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

    //固定更新メソッド
    void UpdateBase();

    virtual void Hit(){}

    XMVECTOR VecMove_;
};