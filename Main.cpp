//インクルード
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include "Engine/Direct3D.h"
#include "Engine/Texture.h"
#include "Engine/Camera.h"
#include "Engine/CallDef.h"
#include "Engine/Transform.h"
#include "Engine/Input.h"
#include "Engine/Model.h"
#include "Engine/RootJob.h"

#pragma comment(lib, "winmm.lib")

//定数宣言
LPCWSTR WIN_CLASS_NAME = L"PersonalProduction";	//ウィンドウクラス名
const unsigned int WINDOW_WIDTH = 1200;	//ウィンドウの幅
const unsigned int WINDOW_HEIGHT = 600;	//ウィンドウの高さ

RootJob* pRootJob;

//プロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//エントリーポイント(プログラムのスタート地点)
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	srand((unsigned int)time(NULL));

	//ウィンドウクラス(設計図)を作成
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);				//この構造体のサイズ
	wc.hInstance = hInstance;					//インスタンスハンドル
	wc.lpszClassName = WIN_CLASS_NAME;			//ウィンドウクラス名
	wc.lpfnWndProc = WndProc;					//ウィンドウプロシージャ
	wc.style = CS_VREDRAW | CS_HREDRAW;			//スタイル（デフォルト）
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);	//アイコン
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);	//小さいアイコン
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	//マウスカーソル
	wc.lpszMenuName = NULL;						//メニュー
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //背景（白）

	RegisterClassEx(&wc);  //クラスを登録

	//ウィンドウサイズの計算
	RECT winRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, TRUE);
	int winW = winRect.right - winRect.left;	//ウィンドウ幅
	int winH = winRect.bottom - winRect.top;	//ウィンドウ高さ

	//ウィンドウを作成
	HWND hWnd = CreateWindow(
		WIN_CLASS_NAME,			//ウィンドウクラス名
		L"個人製作",		//タイトルバーに表示する内容
		WS_OVERLAPPEDWINDOW,	//スタイル（普通のウィンドウ）
		CW_USEDEFAULT,			//表示位置左（おまかせ）
		CW_USEDEFAULT,			//表示位置上（おまかせ）
		winW,					//ウィンドウ幅
		winH,					//ウィンドウ高さ
		NULL,					//親ウインドウ（なし）
		NULL,					//メニュー（なし）
		hInstance,				//インスタンス
		NULL					//パラメータ（なし）
	);

	//ウィンドウを表示
	ShowWindow(hWnd, nCmdShow);

	HRESULT hr;

	//COM(Component Object Model)の初期化
	hr = CoInitialize(nullptr);
	if (FAILED(hr)) {
		CoUninitialize();
		PostQuitMessage(0);
	}

	//Direct3D初期化
	hr = Direct3D::Initialize(WINDOW_WIDTH, WINDOW_HEIGHT, hWnd);
	if (FAILED(hr)) {
		Direct3D::Release();
		PostQuitMessage(0);
	}

	//RootJob初期化
	pRootJob = nullptr;
	pRootJob = new RootJob(nullptr);
	pRootJob->Initialize();

	//DirectInputの初期化
	Input::Initialize(hWnd);

	//カメラの初期化
	Camera::Initialize(winW, winH);

	//メッセージループ(何か起きるのを待つ)
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		//メッセージあり
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//メッセージなし
		else
		{
			timeBeginPeriod(1);	//timeEndPeriodとセットで使用する関数
			DWORD nowTime = timeGetTime();
			static DWORD lastUpdateTime = nowTime;

			if ((nowTime - lastUpdateTime) * 60 <= 1000.0f)
			{
				continue;
			}
			lastUpdateTime = nowTime;

			static char DrawRange = VP_LEFT;
			{
				//spaceキーを押したら描画変更
				if (Input::IsKeyDown(DIK_SPACE))
				{
					switch (Direct3D::SplitScrMode)
					{
					case SCREEN_FULL:
						Direct3D::SplitScrMode = SCREEN_SPLIT_2;
						break;
					case SCREEN_SPLIT_2:
						if (Input::GetMousePosition().x < winW / 2)	//マウスカーソルの位置でカメラの位置を決定
						{
							DrawRange = VP_LEFT;
						}
						else
						{
							DrawRange = VP_RIGHT;
						}
						Direct3D::SplitScrMode = SCREEN_FULL;
						break;
					default: break;
					}
				}
			}

			//ゲームの処理
			pRootJob->Update();
			Camera::Update();
			Direct3D::BeginDraw();

			//入力情報の更新
			Input::Update();
			pRootJob->UpdateSub();

			//描画処理
			switch (Direct3D::SplitScrMode)
			{
			case SCREEN_FULL:
				Direct3D::SetViewPort(VP_FULL);
				switch (DrawRange)
				{
				case VP_LEFT:
					Camera::SetPosition(XMVectorSet(-10.0f, 5.5f, 1.2f, 0));
					Camera::SetTarget(XMVectorSet(-10, 0, 1.3f, 0));
					break;
				case VP_RIGHT:
					Camera::SetPosition(XMVectorSet(1.5f, 6, 1.4f, 0));
					Camera::SetTarget(XMVectorSet(1.5f, 0, 1.5f, 0));
					break;
				default: break;
				}
				Camera::Update();
				//全オブジェクトを描画
				//ルートオブジェクトのDrawを呼んだあと、自動的に子、孫のUpdateが呼ばれる
				pRootJob->DrawSub();
				break;
			case SCREEN_SPLIT_2:
				if (Input::GetMousePosition().x < winW / 2)
				{
					//右画面描画
					{
						Direct3D::SetViewPort(VP_RIGHT);
						Camera::SetPosition(XMVectorSet(1.5f, 6, 1.4f, 0));
						Camera::SetTarget(XMVectorSet(1.5f, 0, 1.5f, 0));

						Camera::Update();
						//全オブジェクトを描画
						//ルートオブジェクトのDrawを呼んだあと、自動的に子、孫のUpdateが呼ばれる
						pRootJob->DrawSub();
					}

					//左画面描画
					{
						Direct3D::SetViewPort(VP_LEFT);
						Camera::SetPosition(XMVectorSet(-10.0f, 5.5f, 1.2f, 0));
						Camera::SetTarget(XMVectorSet(-10, 0, 1.3f, 0));

						Camera::Update();
						//全オブジェクトを描画
						//ルートオブジェクトのDrawを呼んだあと、自動的に子、孫のUpdateが呼ばれる
						pRootJob->DrawSub();
					}
				}
				else
				{
					//左画面描画
					{
						Direct3D::SetViewPort(VP_LEFT);
						Camera::SetPosition(XMVectorSet(-10.0f, 5.5f, 1.2f, 0));
						Camera::SetTarget(XMVectorSet(-10, 0, 1.3f, 0));

						Camera::Update();
						//全オブジェクトを描画
						//ルートオブジェクトのDrawを呼んだあと、自動的に子、孫のUpdateが呼ばれる
						pRootJob->DrawSub();
					}
					//右画面描画
					{
						Direct3D::SetViewPort(VP_RIGHT);
						Camera::SetPosition(XMVectorSet(1.5f, 6, 1.4f, 0));
						Camera::SetTarget(XMVectorSet(1.5f, 0, 1.5f, 0));

						Camera::Update();
						//全オブジェクトを描画
						//ルートオブジェクトのDrawを呼んだあと、自動的に子、孫のUpdateが呼ばれる
						pRootJob->DrawSub();
					}
				}
				break;
			default:
				break;
			}

			Direct3D::EndDraw();

			timeEndPeriod(1);
		}
	}
	pRootJob->ReleaseSub();
	SAFE_DELETE(pRootJob);
	Input::Release();
	Model::Release();
	Direct3D::Release();
	CoUninitialize();	//COMのRelease
	return 0;
}

//ウィンドウプロシージャ(何かあった時に呼び出される関数)
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);  //プログラム終了
		return 0;
	case WM_MOUSEMOVE:
		Input::SetMousePosition(LOWORD(lParam), HIWORD(lParam));
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}