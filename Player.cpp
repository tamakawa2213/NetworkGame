#include "Player.h"
#include "Ground.h"
#include "Engine/SphereCollider.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"


//�R���X�g���N�^
Player::Player(GameObject* parent)
    :GravityInfluence(parent, "Player"), hModel_(-1), ROTATE_SPEED(2.0f), RUN_SPEED(0.3f), FIXED_CAM_POS(0, 5, -35), FIX_TANK_POS(0, 0, 0)
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
    hModel_ = Model::Load("Assets\\TankBase.fbx");
    assert(hModel_ >= 0);

    SphereCollider* collision = new SphereCollider(XMFLOAT3(0, 0, 0), 2.2f);
    AddCollider(collision);
}

//�X�V
void Player::Update()
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
        if (data.dist <= 0.1f)
        {
            OnGround_ = true;
        }
        else
        {
            OnGround_ = false;
        }
    }
    //������Ȃ���Ώ�Ƀ��C�𔭎�
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
        //��Ɍ��������C��������Ȃ��Ȃ���(�X�e�[�W����O�ꂽ)�ꍇ
        else {
            //Player�������ʒu�ɖ߂�
            transform_.position_ = FIX_TANK_POS;
        }
    }


    XMFLOAT3 move = { 0 , 0 , RUN_SPEED }; //�ړ��������(���̂�float)
    XMVECTOR vMove = XMLoadFloat3(&move); //float->vector�ϊ�(���ڃx�N�g���͍��Ȃ�)
    XMMATRIX mRotate = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y)); //transform_.rotate_.y�����W�A���ɕϊ����ĂԂ�����
    XMVECTOR vPos = XMLoadFloat3(&transform_.position_); //���݈ʒu���x�N�g���ɕϊ�
    vMove = XMVector3TransformCoord(vMove, mRotate); //�x�N�g��vMove���s��mRotate�ŕό`������


    ///////////////////////////////////////////// ���� /////////////////////////////////////////////////////////

    if (Input::IsKey(DIK_A))
    {
        //���炩�̏���
        transform_.rotate_.y -= ROTATE_SPEED;
    }
    if (Input::IsKey(DIK_D))
    {
        //���炩�̏���
        transform_.rotate_.y += ROTATE_SPEED;
    }
    if (Input::IsKey(DIK_W))
    {
        vPos += vMove; //�w�肵���x�N�g�������Z

    }
    if (Input::IsKey(DIK_S))
    {
        //���炩�̏���
        vPos -= vMove;//�w�肵���x�N�g�������Z

    }

    Falldown();
    //�W�����v
    if (Input::IsKeyDown(DIK_SPACE) && OnGround_)
    {
        Vertical_ += XMVECTOR{ 0,Jump_,0,0 };
    }
    vPos += Vertical_;

    XMStoreFloat3(&transform_.position_, vPos); //vector->float�ϊ�

    ///////////////////////////////////////////////////////// �J�����̐ݒ� /////////////////////////////////////////////////////////////////
    XMFLOAT3 cam = this->transform_.position_;
    XMVECTOR vCam = XMVectorSet(0, 8.0f, -10.0f, 0);
    vCam = XMVector3TransformCoord(vCam, mRotate);
    XMFLOAT3 camTar = transform_.position_;

    XMStoreFloat3(&cam, vPos + vCam);
    Camera::SetPosition(cam);
    camTar.y += 3;
    Camera::SetTarget(XMLoadFloat3(&camTar));
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