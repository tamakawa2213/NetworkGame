#pragma once

#include "Direct3D.h"
#include "Texture.h"
#include "Transform.h"

class Quad
{
protected:
	//コンスタントバッファ
	struct CONSTANT_BUFFER
	{
		XMMATRIX matWVP;
		XMMATRIX matNormal;
	};

	//頂点情報
	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
		XMVECTOR normal;
	};
	ID3D11Buffer* pVertexBuffer_;	//頂点バッファ
	ID3D11Buffer* pIndexBuffer_;	//インデックスバッファ
	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ

	Texture* pTexture_;
	int index_;		//頂点数
public:
	Quad();				//コンストラクタ
	~Quad();			//デストラクタ
	virtual HRESULT Initialize();	//初期化
	HRESULT ConBuf();
	virtual HRESULT LoadTex();	//テクスチャのロード
	void Draw(Transform& transform);	//描画
	void Release();		//解放
};