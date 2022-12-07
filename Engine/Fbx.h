#pragma once

#include <d3d11.h>
#include <fbxsdk.h>
#include <string>
#include "Transform.h"

#pragma comment(lib, "LibFbxSDK-MT.lib")
#pragma comment(lib, "LibXml2-MT.lib")
#pragma comment(lib, "zlib-MT.lib")

//クラスの前方宣言
class Texture;

struct RayCastData
{
	XMFLOAT3 start;
	XMFLOAT3 dir;
	float dist;
	bool hit;

	RayCastData() : start(), dir(), dist(9999.0f), hit(false) {}
};

class Fbx
{
	//マテリアル
	struct MATERIAL
	{
		Texture* pTexture;
		XMFLOAT4 diffuse;
	};
	struct CONSTANT_BUFFER
	{
		XMMATRIX	matWVP;
		XMMATRIX	matNormal;
		XMFLOAT4	diffuseColor;
		XMFLOAT4	chroma;
		XMFLOAT4	light;
		int			isTexture;
		float		bright;
	};

	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
		XMVECTOR normal;
	};

	ID3D11Buffer* pVertexBuffer_;	//頂点バッファ
	ID3D11Buffer** pIndexBuffer_;	//インデックスバッファ
	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ
	MATERIAL* pMaterialList_;
	int* indexCount_;
	int vertexCount_;	//頂点数
	int polygonCount_;	//ポリゴン数
	int materialCount_;	//マテリアルの個数
	VERTEX* pVertices_;	//頂点情報を持つ構造体のポインタ
	int** ppIndex_;
public:
	Fbx();
	~Fbx();
	HRESULT Load(std::string fileName);
	void InitVertex(fbxsdk::FbxMesh* pMesh);	//頂点バッファ準備
	void InitIndex(fbxsdk::FbxMesh* pMesh);	//インデックスバッファ準備
	HRESULT IntConstantBuffer();
	void InitMaterial(fbxsdk::FbxNode* pNode);
	void RayCast(RayCastData& rayData);
	void Draw(Transform& transform);
	void Draw(Transform& transform, XMFLOAT3 Chroma, float Bright, float Alpha);	//transform, RGBの値, 明度(0 ～255)
	void Release();

	XMVECTOR NormalDotLight(Transform tr);
};