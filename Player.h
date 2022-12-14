#pragma once
#include "Engine/GravityInfluence.h"

//���������Ǘ�����N���X
class Player : public GravityInfluence
{
    //�萔
    const float    RUN_SPEED;           //�ړ����x
    const float    ROTATE_SPEED;        //��]���x
    const XMFLOAT3 FIXED_CAM_POS;       //�Œ�J�����̈ʒu
    const XMFLOAT3 FIX_TANK_POS;        //Tank�̈ʒu���C��

    int hModel_;    //���f���ԍ�
    const float Jump_ = 0.4f;     //�W�����v��
public:
    //�R���X�g���N�^
    Player(GameObject* parent);

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
};