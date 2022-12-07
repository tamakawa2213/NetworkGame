#include "Camera.h"
#include "CallDef.h"

//変数
namespace Camera {
	XMVECTOR position_;	//カメラの位置（視点）
	XMVECTOR target_;	//見る位置（焦点）
	XMMATRIX viewMatrix_;	//ビュー行列
	XMMATRIX projMatrix_;	//プロジェクション行列


//初期化
	void Camera::Initialize(int winW, int winH)
	{
		position_ = XMVectorSet(NULL, 5, 1.4f, NULL);	//カメラの位置
		target_ = XMVectorSet(NULL, NULL, 1.5f, NULL);		//カメラの焦点
		projMatrix_ = XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)winW / (FLOAT)winH / 2.0f, 0.1f, 1000.0f);
	}

	//更新
	void Camera::Update()
	{
		//ビュー行列の作成(カメラ固定のゲームならInitializeに書く)
		viewMatrix_ = XMMatrixLookAtLH(position_, target_, XMVectorSet(0, 1, 0, 0));
	}

	//位置を設定
	void Camera::SetPosition(XMVECTOR position)
	{
		position_ = position;
	}

	void Camera::SetPosition(XMFLOAT3 position)
	{
		position_ = XMLoadFloat3(&position);
	}

	//焦点を設定
	void Camera::SetTarget(XMVECTOR target)
	{
		target_ = target;
	}

	//ビュー行列を取得
	XMMATRIX Camera::GetViewMatrix()
	{
		return viewMatrix_;
	}

	//プロジェクション行列を取得
	XMMATRIX Camera::GetProjectionMatrix()
	{
		return projMatrix_;
	}
	XMFLOAT3 Camera::GetCameraPosition()
	{
		XMFLOAT3 pos;
		XMStoreFloat3(&pos, position_);
		return pos;
	}
	XMFLOAT3 Camera::GetCameraTarget()
	{
		XMFLOAT3 tar;
		XMStoreFloat3(&tar, target_);
		return tar;
	}
	XMVECTOR GetCameraVecPosition()
	{
		return position_;
	}
	XMVECTOR GetCameraVecTarget()
	{
		return target_;
	}
}