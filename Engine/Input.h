#pragma once

#include <DirectXMath.h>
#include <dInput.h>
#include "XInput.h"

#pragma comment(lib, "Xinput.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dInput8.lib")

namespace Input
{
	extern XINPUT_STATE controllerState_;

	void Initialize(HWND hWnd);
	void Update();

	//キーボード
	bool IsKey(int keyCode);		//キーを押している間
	bool IsKeyDown(int keyCode);	//キーを押した瞬間
	bool IsKeyUp(int keyCode);		//キーを離した瞬間

	//マウス
	DirectX::XMFLOAT3 GetMousePosition();	//マウスの位置を取得
	void SetMousePosition(int x, int y);	//マウスの位置を設定
	DirectX::XMFLOAT3 GetMouseMovement();	//マウスの移動量を取得
	bool IsMouseMove();						//マウスが動いたか
	bool IsMouse(int mouseBotton);			//マウスを押している間
	bool IsMouseDown(int mouseBotton);		//マウスを押した瞬間
	bool IsMouseUp(int mouseBotton);		//マウスを離した瞬間

	//コントローラー
	bool IsCtrl(int ctrlBotton);				//ボタンを押している間
	bool IsCtrlUp(int ctrlBotton);				//ボタンを離した瞬間
	bool IsCtrlDown(int ctrlBotton);			//ボタンを押した瞬間
	DirectX::XMFLOAT3 CtrlL_StickInclination();	//Lスティックの傾き
	DirectX::XMFLOAT3 CtrlR_StickInclination();	//Rスティックの傾き
	float CtrlTriggerInclination(BYTE Trigger);	//トリガーの押し込み具合

	void Release();
};