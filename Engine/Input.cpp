#include "Input.h"
#include "CallDef.h"
#include <assert.h>

namespace Input
{
	//キーボード
	LPDIRECTINPUT8   pDInput = nullptr;
	LPDIRECTINPUTDEVICE8 pKeyDevice = nullptr;	//デバイスオブジェクト
	BYTE keyState[256] = { 0 };					//現在の各キーの状態
	BYTE prevKeyState[256];						//前フレームでの各キーの状態

	//マウス
	LPDIRECTINPUTDEVICE8 pMouseDevice = nullptr;	//マウス
	DIMOUSESTATE mouseState;						//マウスの状態
	DIMOUSESTATE prevMouseState;					//前フレームのマウスの状態
	DirectX::XMFLOAT3 mousePosition;				//マウスの位置
	DirectX::XMFLOAT3 prevMousePosition;

	//コントローラー
	XINPUT_STATE controllerState_;
	XINPUT_STATE prevControllerState_;


	void Initialize(HWND hWnd)
	{
		HRESULT hr;
		hr = DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDInput, nullptr);
		assert(hr == S_OK);

		//キーボード
		hr = pDInput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, nullptr);
		assert(hr == S_OK);
		pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
		pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

		//マウス
		hr = pDInput->CreateDevice(GUID_SysMouse, &pMouseDevice, nullptr);
		assert(hr == S_OK);
		pMouseDevice->SetDataFormat(&c_dfDIMouse);
		pMouseDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

		
	}

	void Update()
	{
		memcpy(prevKeyState, keyState, sizeof(keyState));
		pKeyDevice->Acquire();
		pKeyDevice->GetDeviceState(sizeof(keyState), &keyState);

		pMouseDevice->Acquire();
		memcpy(&prevMouseState, &mouseState, sizeof(mouseState));
		pMouseDevice->GetDeviceState(sizeof(mouseState), &mouseState);
		prevMousePosition = mousePosition;

		//コントローラー
		prevControllerState_ = controllerState_;
		XInputGetState(0, &controllerState_);
	}

	///////////////////////////////////////////////////キーボード関連///////////////////////////////////////////

	bool IsKey(int keyCode)
	{
		if (keyState[keyCode] & 0x80)	//16進数80との論理積
		{
			return true;
		}

		return false;
	}

	bool IsKeyDown(int keyCode)
	{
		//今は押してて、前回は押してない
		if (IsKey(keyCode) && !(prevKeyState[keyCode] & 0x80))
		{
			return true;
		}
		return false;
	}

	bool IsKeyUp(int keyCode)
	{
		//前回は押してて、今は押してない
		if ((prevKeyState[keyCode] & 0x80) && !IsKey(keyCode))
		{
			return true;
		}
		return false;
	}

	///////////////////////////////////////////////////////マウス関連//////////////////////////////////////////////////

	DirectX::XMFLOAT3 GetMousePosition()
	{
		return mousePosition;
	}

	void SetMousePosition(int x, int y)
	{
		mousePosition = DirectX::XMFLOAT3((float)x, (float)y, 0);
	}

	DirectX::XMFLOAT3 GetMouseMovement()
	{
		DirectX::XMFLOAT3 MouseMovement;
		MouseMovement.x = mousePosition.x - prevMousePosition.x;
		MouseMovement.y = mousePosition.y - prevMousePosition.y;
		MouseMovement.z = mousePosition.z - prevMousePosition.z;
		return MouseMovement;
	}

	bool IsMouseMove()
	{
		DirectX::XMFLOAT3 MouseMovement;
		MouseMovement.x = mousePosition.x - prevMousePosition.x;
		MouseMovement.y = mousePosition.y - prevMousePosition.y;
		MouseMovement.z = mousePosition.z - prevMousePosition.z;
		if (MouseMovement.x == 0 && MouseMovement.y == 0 && MouseMovement.z == 0) {
			return false;
		}
		return true;
	}

	bool IsMouse(int mouseBotton)
	{
		if (mouseState.rgbButtons[mouseBotton] & 0x80)	//16進数80との論理積
		{
			return true;
		}
		return false;
	}

	bool IsMouseDown(int mouseBotton)
	{
		if (IsMouse(mouseBotton) && !(prevMouseState.rgbButtons[mouseBotton] & 0x80))
		{
			return true;
		}
		return false;
	}

	bool IsMouseUp(int mouseBotton)
	{
		if (!IsMouse(mouseBotton) && prevMouseState.rgbButtons[mouseBotton] & 0x80)
		{
			return true;
		}
		return false;
	}

	//////////////////////////////////////////コントローラー////////////////////////////////////////////

	bool IsCtrl(int ctrlBotton)
	{
		if (controllerState_.Gamepad.wButtons & ctrlBotton)
		{
			return true;
		}
		return false;
	}

	bool IsCtrlUp(int ctrlBotton)
	{
		if (IsCtrl(ctrlBotton) && !(prevControllerState_.Gamepad.wButtons & ctrlBotton))
		{
			return true;
		}
		return false;
	}

	bool IsCtrlDown(int ctrlBotton)
	{
		if (!IsCtrl(ctrlBotton) && prevControllerState_.Gamepad.wButtons & ctrlBotton)
		{
			return true;
		}
		return false;
	}

	DirectX::XMFLOAT3 CtrlL_StickInclination()
	{
		if ((controllerState_.Gamepad.sThumbLX <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			controllerState_.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
			(controllerState_.Gamepad.sThumbLY <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE&&
			controllerState_.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
		{
			controllerState_.Gamepad.sThumbLX = 0;
			controllerState_.Gamepad.sThumbLY = 0;
		}
		float x = (float)controllerState_.Gamepad.sThumbLX / 32767.0f;
		float y = (float)controllerState_.Gamepad.sThumbLY / 32767.0f;
		return DirectX::XMFLOAT3(x, y, 0);
	}

	DirectX::XMFLOAT3 CtrlR_StickInclination()
	{
		if ((controllerState_.Gamepad.sThumbRX <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			controllerState_.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
			(controllerState_.Gamepad.sThumbRY <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
				controllerState_.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
		{
			controllerState_.Gamepad.sThumbRX = 0;
			controllerState_.Gamepad.sThumbRY = 0;
		}
		float x = (float)controllerState_.Gamepad.sThumbRX / 32767.0f;
		float y = (float)controllerState_.Gamepad.sThumbRY / 32767.0f;
		return DirectX::XMFLOAT3(x, y, 0);
	}

	float CtrlTriggerInclination(BYTE Trigger)
	{
		float value = Trigger;
		if (value < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		{
			value = 0.0f;
		}
		value = value / 255;
		return value;
	}

	void Release()
	{
		SAFE_RELEASE(pMouseDevice);
		SAFE_RELEASE(pKeyDevice);
		SAFE_RELEASE(pDInput);
	}
}