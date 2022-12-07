#pragma once
#include "Direct3D.h"
#include "Texture.h"
#include "Transform.h"

class Sprite
{
protected:
	//コンスタントバッファ
	struct CONSTANT_BUFFER
	{
		XMMATRIX matW;
	};

	//頂点情報
	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
	};
	ID3D11Buffer* pVertexBuffer_;	//頂点バッファ
	ID3D11Buffer* pIndexBuffer_;	//インデックスバッファ
	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ

	Texture* pTexture_;
	int index_;		//頂点数

public:
	Sprite();				//コンストラクタ
	~Sprite();				//デストラクタ
	virtual HRESULT Initialize(LPCWSTR filename);	//初期化
	HRESULT ConBuf(LPCWSTR filename);
	virtual HRESULT Load(LPCWSTR filename);
	//Transformを引数にとる
	void Draw(Transform& transform);	//描画
	void Release();						//解放
};