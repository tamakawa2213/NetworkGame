#pragma once
#include "Engine/GravityInfluence.h"

class Player : public GravityInfluence
{
    //定数
    const float    RUN_SPEED;           //移動速度
    const float    ROTATE_SPEED;        //回転速度
    const XMFLOAT3 FIXED_CAM_POS;       //固定カメラの位置
    const XMFLOAT3 FIX_TANK_POS;        //Tankの位置を修正
    const float Jump_ = 0.4f;           //ジャンプ力

    int hModel_;    //モデル番号


    XMVECTOR vPos_;
    XMVECTOR vMove_;
    XMMATRIX mRotate_;
protected:
    enum
    {
        COMMAND_ADV  = 0x01,       //前進
        COMMAND_BACK = 0x02,       //後退
        COMMAND_ROTATE_R = 0x04,   //右回転
        COMMAND_ROTATE_L = 0x08,   //左回転
        COMMAND_JUMP = 0x0f        //ジャンプ
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
};