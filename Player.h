#pragma once
#include "Engine/GravityInfluence.h"

class Player : public GravityInfluence
{
    //�萔
    const float    RUN_SPEED;           //�ړ����x
    const float    ROTATE_SPEED;        //��]���x
    const XMFLOAT3 FIXED_CAM_POS;       //�Œ�J�����̈ʒu
    const float Jump_ = 0.4f;           //�W�����v��

    int hModel_;    //���f���ԍ�

    XMVECTOR vPos_;
    XMVECTOR vMove_;
    XMMATRIX mRotate_;

    bool Lose_;
protected:

    enum
    {
        COMMAND_ADV  = 0x01 << 0,       //�O�i
        COMMAND_BACK = 0x01 << 1,       //���
        COMMAND_ROTATE_R = 0x01 << 2,   //�E��]
        COMMAND_ROTATE_L = 0x01 << 3,   //����]
        COMMAND_JUMP = 0x01 << 4       //�W�����v
    };

    char Command_;  //�R�}���h
    virtual void SetCommand() = 0;

public:
    //�R���X�g���N�^
    Player(GameObject* parent);

    //�p���p�R���X�g���N�^
    Player(GameObject* parent, std::string name);

    //�f�X�g���N�^
    ~Player();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;

    //�Œ�X�V���\�b�h
    void UpdateBase();

    virtual void Hit(){}

    XMVECTOR VecMove_;
};