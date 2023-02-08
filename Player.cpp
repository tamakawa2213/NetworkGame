#include "Player.h"
#include "Ground.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"
#include "Engine/SceneManager.h"
#include "Observer.h"

//�R���X�g���N�^
Player::Player(GameObject* parent)
    :Player(parent, "Player")
{
}

Player::Player(GameObject* parent, std::string name)
    :GravityInfluence(parent, name), hModel_(-1), ROTATE_SPEED(2.0f), RUN_SPEED(0.015f), FIXED_CAM_POS(0, 5, -35),
    vPos_(), vMove_(), mRotate_(), Command_(0), VecMove_(), Lose_(false)
{
}

//�f�X�g���N�^
Player::~Player()
{
}

//������
void Player::Initialize()
{
    //���f���f�[�^�̃��[�h
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

//�X�V
void Player::Update()
{
    UpdateBase();
    ///////////////////////////////////////////// ���� /////////////////////////////////////////////////////////
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

    XMStoreFloat3(&transform_.position_, vPos_); //vector->float�ϊ�

    Hit();

    ///////////////////////////////////////////////////////// �J�����̐ݒ� /////////////////////////////////////////////////////////////////
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

//�`��
void Player::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

//�J��
void Player::Release()
{
}

void Player::UpdateBase()
{
    //////////////////////////////////////////// ���C�L���X�g //////////////////////////////////////////////////

    Ground* pGround = (Ground*)FindObject("Ground");    //�X�e�[�W�I�u�W�F�N�g��T��
    int hGroundModel = pGround->GetModelHandle();    //���f���ԍ����擾

    RayCastData data;
    data.start = transform_.position_;   //���C�̔��ˈʒu
    data.dir = XMFLOAT3(0, -1, 0);       //���C�̕���
    Model::RayCast(hGroundModel, data); //���C�𔭎�

    //���C������������
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
        //���C������������
        if (data.hit) {
            //���̕��ʒu���グ��
            transform_.position_.y += data.dist;
            OnGround_ = true;
        }
        else
        {
            Lose_ = true;
        }
    }


    XMFLOAT3 move = { 0 , 0 , RUN_SPEED }; //�ړ��������(���̂�float)
    vMove_ = XMLoadFloat3(&move); //float->vector�ϊ�(���ڃx�N�g���͍��Ȃ�)
    mRotate_ = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y)); //transform_.rotate_.y�����W�A���ɕϊ����ĂԂ�����
    vPos_ = XMLoadFloat3(&transform_.position_); //���݈ʒu���x�N�g���ɕϊ�
    vMove_ = XMVector3TransformCoord(vMove_, mRotate_); //�x�N�g��vMove���s��mRotate�ŕό`������
}
