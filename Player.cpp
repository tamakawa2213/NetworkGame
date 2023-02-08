#include "Player.h"
#include "Ground.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"
#include "Engine/SceneManager.h"
#include "Observer.h"

//コンストラクタ
Player::Player(GameObject* parent)
    :Player(parent, "Player")
{
}

Player::Player(GameObject* parent, std::string name)
    :GravityInfluence(parent, name), hModel_(-1), ROTATE_SPEED(2.0f), RUN_SPEED(0.015f), FIXED_CAM_POS(0, 5, -35),
    vPos_(), vMove_(), mRotate_(), Command_(0), VecMove_(), Lose_(false)
{
}

//デストラクタ
Player::~Player()
{
}

//初期化
void Player::Initialize()
{
    //モデルデータのロード
    hModel_ = Model::Load("Assets\\Parrot.fbx");
    assert(hModel_ >= 0);

    if (this->GetObjectName() == "Playable")
    {
        transform_.rotate_.y = 180;
        transform_.position_.z = 5;
    }
    else
    {
        transform_.position_.z = -5;
    }
}

//更新
void Player::Update()
{
    UpdateBase();
    ///////////////////////////////////////////// 入力 /////////////////////////////////////////////////////////
    SetCommand();
    Falldown();
    

    switch (Command_)
    {
    case COMMAND_ADV:
        VecMove_ += vMove_;
        break;

    case COMMAND_BACK:
        VecMove_ -= vMove_;
        break;

    case COMMAND_ROTATE_R:
        transform_.rotate_.y += ROTATE_SPEED;
        break;

    case COMMAND_ROTATE_L:
        transform_.rotate_.y -= ROTATE_SPEED;
        break;

    case COMMAND_JUMP:
        Vertical_ += XMVECTOR{ 0,Jump_,0,0 };
        break;
    default:
        break;
    }

    Command_ = 0;

    VecMove_ *= 0.98f;

    vPos_ += Vertical_ + VecMove_;

    XMStoreFloat3(&transform_.position_, vPos_); //vector->float変換

    Hit();

    ///////////////////////////////////////////////////////// カメラの設定 /////////////////////////////////////////////////////////////////
    if (this->GetObjectName() == "Playable")
    {
        XMFLOAT3 cam = this->transform_.position_;
        XMVECTOR vCam = XMVectorSet(0, 8.0f, -10.0f, 0);
        vCam = XMVector3TransformCoord(vCam, mRotate_);
        XMFLOAT3 camTar = transform_.position_;

        XMStoreFloat3(&cam, vPos_ + vCam);
        Camera::SetPosition(cam);
        camTar.y += 3;
        Camera::SetTarget(XMLoadFloat3(&camTar));
    }


    if (Lose_)
    {
        if (GetObjectName() == "Playable")
            Observer::SetWinner(false);
        else
            Observer::SetWinner(true);

        SCENE_CHANGE(SCENE_ID_RESULT);
    }
}

//描画
void Player::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

//開放
void Player::Release()
{
}

void Player::UpdateBase()
{
    //////////////////////////////////////////// レイキャスト //////////////////////////////////////////////////

    Ground* pGround = (Ground*)FindObject("Ground");    //ステージオブジェクトを探す
    int hGroundModel = pGround->GetModelHandle();    //モデル番号を取得

    RayCastData data;
    data.start = transform_.position_;   //レイの発射位置
    data.dir = XMFLOAT3(0, -1, 0);       //レイの方向
    Model::RayCast(hGroundModel, data); //レイを発射

    //レイが当たったら
    if (data.hit)
    {
        if (data.dist <= 0.3f)
        {
            OnGround_ = true;
        }
        else
        {
            OnGround_ = false;
        }
    }
    else {
        data.start = transform_.position_;
        data.dir = XMFLOAT3(0, 1, 0);
        Model::RayCast(hGroundModel, data);
        //レイが当たったら
        if (data.hit) {
            //その分位置を上げる
            transform_.position_.y += data.dist;
            OnGround_ = true;
        }
        else
        {
            Lose_ = true;
        }
    }


    XMFLOAT3 move = { 0 , 0 , RUN_SPEED }; //移動距離作り(作るのはfloat)
    vMove_ = XMLoadFloat3(&move); //float->vector変換(直接ベクトルは作れない)
    mRotate_ = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y)); //transform_.rotate_.yをラジアンに変換してぶち込む
    vPos_ = XMLoadFloat3(&transform_.position_); //現在位置をベクトルに変換
    vMove_ = XMVector3TransformCoord(vMove_, mRotate_); //ベクトルvMoveを行列mRotateで変形させる
}
