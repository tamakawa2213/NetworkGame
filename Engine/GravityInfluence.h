#pragma once
#include "GameObject.h"

class GravityInfluence : public GameObject
{
	const XMVECTOR GrAccel_ = { 0,0.01f,0,0 };	//�d�͉����x
protected:
	bool OnGround_;			//�n�ʂƐڂ��Ă��邩�ǂ���
	XMVECTOR Vertical_;		//�c�����̃x�N�g��
	void  Falldown();		//��������
public:
	//�R���X�g���N�^
	GravityInfluence(GameObject* parent);
	//�p���p�R���X�g���N�^
	GravityInfluence(GameObject* parent, std::string name);
	~GravityInfluence();
};