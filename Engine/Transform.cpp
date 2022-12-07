#include "Transform.h"
#include "CallDef.h"
#include "GameObject.h"

Transform::Transform():matTranslate_(XMMatrixIdentity()),matRotate_(XMMatrixIdentity()),matScale_(XMMatrixIdentity()),
position_(0,0,0),rotate_(),scale_(1,1,1), pParent_(nullptr)
{
}

Transform::~Transform()
{
}

void Transform::Calclation()
{
    //ˆÚ“®s—ñ
    matTranslate_ = XMMatrixTranslation( position_.x, position_.y, position_.z);

    //‰ñ“]s—ñ
    matRotate_ = XMMatrixRotationX(XMConvertToRadians(rotate_.x))
        * XMMatrixRotationY(XMConvertToRadians(rotate_.y))
        * XMMatrixRotationZ(XMConvertToRadians(rotate_.z));

    //Šg‘ås—ñ
    matScale_ = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
}

XMMATRIX Transform::GetWorldMatrix()
{
    Calclation();
    if (pParent_)
    {
        return matScale_ * matRotate_ * matTranslate_ * pParent_->GetWorldMatrix();
    }
    //Šg‘å * ‰ñ“] * ˆÚ“®
    return (matScale_ * matRotate_ * matTranslate_);
}

XMMATRIX Transform::GetNormalMatrix()
{
    return (XMMatrixInverse(nullptr, matScale_) * matRotate_);
}
