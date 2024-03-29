#include "Direct3D.h"
#include "Camera.h"
#include "CallDef.h"
#include <d3dcompiler.h>

//変数
namespace Direct3D
{
	//Direct3D関連
	ID3D11Device* pDevice = nullptr;						//デバイス
	ID3D11DeviceContext* pContext = nullptr;				//デバイスコンテキスト
	IDXGISwapChain* pSwapChain = nullptr;					//スワップチェイン
	ID3D11RenderTargetView* pRenderTargetView = nullptr;	//レンダーターゲットビュー
	ID3D11BlendState* pBlendState = nullptr;				//ブレンドステート
	ID3D11Texture2D* pDepthStencil = nullptr;				//深度ステンシル
	ID3D11DepthStencilView* pDepthStencilView = nullptr;	//深度ステンシルビュー

	//シェーダ関連
	struct SHADER {
		ID3D11VertexShader* pVertexShader = nullptr;		//頂点シェーダー
		ID3D11PixelShader* pPixelShader = nullptr;			//ピクセルシェーダー
		ID3D11InputLayout* pVertexLayout = nullptr;			//頂点インプットレイアウト
		ID3D11RasterizerState* pRasterizerState = nullptr;	//ラスタライザー
	};
	SHADER shader[SHADER_MAX];

	int scrWidth, scrHeight;

	char SplitScrMode = SCREEN_FULL;

	D3D11_VIEWPORT vp[VP_MAX];
}

//初期化
HRESULT Direct3D::Initialize(int winW, int winH, HWND hWnd)
{
	scrWidth = winW;
	scrHeight = winH;

	HRESULT hr;

	///////////////////////////いろいろ準備するための設定///////////////////////////////
	//いろいろな設定項目をまとめた構造体
	DXGI_SWAP_CHAIN_DESC scDesc;

	//とりあえず全部0
	ZeroMemory(&scDesc, sizeof(scDesc));

	//描画先のフォーマット
	scDesc.BufferDesc.Width = winW;		//画面幅
	scDesc.BufferDesc.Height = winH;	//画面高さ
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// 何色使えるか

	//FPS（1/60秒に1回）
	scDesc.BufferDesc.RefreshRate.Numerator = 60;
	scDesc.BufferDesc.RefreshRate.Denominator = 1;

	//その他
	scDesc.Windowed = TRUE;			//ウィンドウモードかフルスクリーンか
	scDesc.OutputWindow = hWnd;		//ウィンドウハンドル
	scDesc.BufferCount = 1;			//バックバッファの枚数
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//バックバッファの使い道＝画面に描画するために
	scDesc.SampleDesc.Count = 1;		//MSAA（アンチエイリアス）の設定
	scDesc.SampleDesc.Quality = 0;		//　〃

	////////////////上記設定をもとにデバイス、コンテキスト、スワップチェインを作成////////////////////////
	D3D_FEATURE_LEVEL level;
	hr = D3D11CreateDeviceAndSwapChain(
		nullptr,						// どのビデオアダプタを使用するか？既定ならばnullptrで
		D3D_DRIVER_TYPE_HARDWARE,		// ドライバのタイプを渡す。ふつうはHARDWARE
		nullptr,						// 上記をD3D_DRIVER_TYPE_SOFTWAREに設定しないかぎりnullptr
		0,								// 何らかのフラグを指定する。（デバッグ時はD3D11_CREATE_DEVICE_DEBUG？）
		nullptr,						// デバイス、コンテキストのレベルを設定。nullptrにしとけばOK
		0,								// 上の引数でレベルを何個指定したか
		D3D11_SDK_VERSION,				// SDKのバージョン。必ずこの値
		&scDesc,						// 上でいろいろ設定した構造体
		&pSwapChain,					// 無事完成したSwapChainのアドレスが返ってくる
		&pDevice,						// 無事完成したDeviceアドレスが返ってくる
		&level,							// 無事完成したDevice、Contextのレベルが返ってくる
		&pContext);						// 無事完成したContextのアドレスが返ってくる
	HR_FAILED(hr, L"デバイス、コンテキスト、スワップチェインの作成に失敗しました");

	///////////////////////////レンダーターゲットビュー作成///////////////////////////////
	//スワップチェーンからバックバッファを取得（バックバッファ ＝ レンダーターゲット）
	ID3D11Texture2D* pBackBuffer = nullptr;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	assert(pBackBuffer != nullptr);

	//レンダーターゲットビューを作成
	hr = pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
	HR_FAILED_RELEASE(hr, L"レンダーターゲットビューの作成に失敗しました", pBackBuffer);
	
	//レンダリング結果を表示する範囲
	vp[VP_FULL].Width = (float)winW;	//幅
	vp[VP_FULL].Height = (float)winH;	//高さ
	vp[VP_FULL].MinDepth = 0.0f;		//手前
	vp[VP_FULL].MaxDepth = 1.0f;		//奥
	vp[VP_FULL].TopLeftX = 0;			//左
	vp[VP_FULL].TopLeftY = 0;			//上

	vp[VP_LEFT].Width = (float)winW / 2;//幅
	vp[VP_LEFT].Height = (float)winH;	//高さ
	vp[VP_LEFT].MinDepth = 0.0f;		//手前
	vp[VP_LEFT].MaxDepth = 1.0f;		//奥
	vp[VP_LEFT].TopLeftX = 0;			//左
	vp[VP_LEFT].TopLeftY = 0;			//上

	vp[VP_RIGHT].Width = (float)winW / 2;	//幅
	vp[VP_RIGHT].Height = (float)winH;		//高さ
	vp[VP_RIGHT].MinDepth = 0.0f;			//手前
	vp[VP_RIGHT].MaxDepth = 1.0f;			//奥
	vp[VP_RIGHT].TopLeftX = (float)winW / 2;//左
	vp[VP_RIGHT].TopLeftY = 0;				//上

	//深度ステンシルビューの作成
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = winW;
	descDepth.Height = winH;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	pDevice->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
	assert(pDepthStencil != 0);
	pDevice->CreateDepthStencilView(pDepthStencil, NULL, &pDepthStencilView);

	//ブレンドステートの作成
	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;
	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	pDevice->CreateBlendState(&BlendDesc, &pBlendState);
	float blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
	pContext->OMSetBlendState(pBlendState, blendFactor, UINT_MAX);

	//データを画面に描画するための一通りの設定（パイプライン）
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// データの入力種類を指定
	pContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);		// 描画先を設定
	
	

	//シェーダー準備(処理が長くなったので分割)
	hr = InitShader();
	if (FAILED(hr)) {//失敗したら、hr(原因)をreturnする
		return hr;
	}
	return S_OK;	//HRESULT型の戻り値を返す必要がある
}

//シェーダー準備
HRESULT Direct3D::InitShader()
{
	HRESULT hr;

	hr = InitShader2D();
	assert(hr == S_OK);

	hr = InitShader3D();
	assert(hr == S_OK);

	return S_OK;
}

HRESULT Direct3D::InitShader2D()
{
	HRESULT hr;
	///////////////////////////////////////////////////// 2D用 /////////////////////////////////////////////////////////////////////

	// 頂点シェーダ(VertexShader)の作成（コンパイル）
	ID3DBlob* pCompileVS = nullptr;
	D3DCompileFromFile(L"Simple2D.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	assert(pCompileVS != nullptr);	//ファイルを開けなかった場合、処理が終了する
	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &shader[SHADER_2D].pVertexShader);

	HR_FAILED_RELEASE(hr, L"頂点シェーダの作成に失敗しました",pCompileVS);

	//頂点インプットレイアウト
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },					//位置
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT	, 0, sizeof(XMVECTOR), D3D11_INPUT_PER_VERTEX_DATA, 0 },	//UV座標
	};

	hr = pDevice->CreateInputLayout(layout, (UINT)std::size(layout), pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &shader[SHADER_2D].pVertexLayout);
	HR_FAILED(hr, L"頂点インプットレイアウトの作成に失敗しました");

	SAFE_RELEASE(pCompileVS);		//コンパイルした頂点シェーダを解放

	// ピクセルシェーダ(PixelShader)の作成（コンパイル）
	ID3DBlob* pCompilePS = nullptr;
	D3DCompileFromFile(L"Simple2D.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	assert(pCompilePS != nullptr);
	hr = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, &shader[SHADER_2D].pPixelShader);

	HR_FAILED_RELEASE(hr, L"ピクセルシェーダの作成に失敗しました",pCompilePS);

	SAFE_RELEASE(pCompilePS);		//コンパイルしたピクセルシェーダを解放

	//ラスタライザ作成
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_BACK;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = FALSE;
	hr = pDevice->CreateRasterizerState(&rdc, &shader[SHADER_2D].pRasterizerState);

	HR_FAILED(hr, L"ラスタライザの作成に失敗しました");

	return S_OK;
}

HRESULT Direct3D::InitShader3D()
{
	HRESULT hr;
	///////////////////////////////////////////////// 3D用 /////////////////////////////////////////////////////////////////

	// 頂点シェーダ(VertexShader)の作成（コンパイル）
	ID3DBlob* pCompileVS = nullptr;
	D3DCompileFromFile(L"Simple3D.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	assert(pCompileVS != nullptr);	//ファイルを開けなかった場合、処理が終了する
	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &shader[SHADER_3D].pVertexShader);
	HR_FAILED_RELEASE(hr, L"頂点シェーダの作成に失敗しました", pCompileVS);

	//頂点インプットレイアウト
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },						//位置
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT	, 0, sizeof(XMVECTOR), D3D11_INPUT_PER_VERTEX_DATA, 0 },		//UV座標
		{ "NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(XMVECTOR) * 2 ,	D3D11_INPUT_PER_VERTEX_DATA, 0 },	//法線
	};
	hr = pDevice->CreateInputLayout(layout, (UINT)std::size(layout), pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &shader[SHADER_3D].pVertexLayout);

	HR_FAILED(hr, L"頂点インプットレイアウトの作成に失敗しました");

	SAFE_RELEASE(pCompileVS);		//コンパイルした頂点シェーダを解放

	// ピクセルシェーダ(PixelShader)の作成（コンパイル）
	ID3DBlob* pCompilePS = nullptr;
	D3DCompileFromFile(L"Simple3D.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	assert(pCompilePS != nullptr);
	hr = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, &shader[SHADER_3D].pPixelShader);

	HR_FAILED_RELEASE(hr, L"ピクセルシェーダの作成に失敗しました", pCompilePS);

	SAFE_RELEASE(pCompilePS);		//コンパイルしたピクセルシェーダを解放

	//ラスタライザ作成
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_BACK;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = FALSE;
	hr = pDevice->CreateRasterizerState(&rdc, &shader[SHADER_3D].pRasterizerState);

	HR_FAILED(hr, L"ラスタライザの作成に失敗しました");
	return S_OK;
}

void Direct3D::SetShader(int type)
{
	//それぞれをデバイスコンテキストにセット
	pContext->VSSetShader(shader[type].pVertexShader, NULL, 0);	//頂点シェーダー
	pContext->PSSetShader(shader[type].pPixelShader, NULL, 0);	//ピクセルシェーダー
	pContext->IASetInputLayout(shader[type].pVertexLayout);		//頂点インプットレイアウト
	pContext->RSSetState(shader[type].pRasterizerState);		//ラスタライザ
}

//描画開始
void Direct3D::BeginDraw()
{
	//背景の色
	float clearColor[4] = { 0.0f, 0.5f, 0.5f, 1.0f };//R,G,B,A

	//画面をクリア
	pContext->ClearRenderTargetView(pRenderTargetView, clearColor);
	
	Camera::Update();

	//深度バッファクリア
	pContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

//描画終了
void Direct3D::EndDraw()
{
	//スワップ（バックバッファを表に表示する）
	pSwapChain->Present(0, 0);
}

//解放処理
void Direct3D::Release()
{
	for (int i = 0; i < SHADER_MAX; i++) {
		SAFE_RELEASE(shader[i].pRasterizerState);
		SAFE_RELEASE(shader[i].pVertexLayout);
		SAFE_RELEASE(shader[i].pPixelShader);
		SAFE_RELEASE(shader[i].pVertexShader);
	}
	SAFE_RELEASE(pBlendState);
	SAFE_RELEASE(pRenderTargetView);
	SAFE_RELEASE(pSwapChain);
	SAFE_RELEASE(pContext);
	SAFE_RELEASE(pDevice);
}

void Direct3D::SetViewPort(char lr)
{
	pContext->RSSetViewports(1, &vp[lr]);
}