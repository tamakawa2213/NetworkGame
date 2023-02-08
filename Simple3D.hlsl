//───────────────────────────────────────
 // テクスチャ＆サンプラーデータのグローバル変数定義 :の後ろにあるものはセマンティクスという
//───────────────────────────────────────
Texture2D	g_texture : register(t0);	//テクスチャー
SamplerState	g_sampler : register(s0);	//サンプラー

//───────────────────────────────────────
// コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer global
{
	float4x4	matWVP;			//ワールド・ビュー・プロジェクションの合成行列
	float4x4	matNormal;		//移動行列を除いたワールド行列
	float4		diffuseColor;	//ディフューズカラー（マテリアルの色）
	float4		chroma;			//Red, Green, Blue, Alpha
	float4		light;			//光のベクトル
	bool		isTexture;		//テクスチャ貼ってあるかどうか
	float		bright;			//ポリゴンの明るさ
};

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
	float4 pos	: SV_POSITION;	//位置
	float2 uv	: TEXCOORD;		//UV座標
	float4 color: COLOR;		//色（明るさ）
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//ピクセルシェーダーへ渡す情報
	VS_OUT outData;

	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
	outData.pos = mul(pos, matWVP);
	outData.uv = uv;
	
	//法線にワールド行列を掛けて回転
	normal = mul(normal, matNormal);

	float4 Light = light;
	Light.a = 0;
	Light = normalize(Light);
	outData.color = clamp(dot(normal, Light), 0, 1);
	//まとめて出力
	return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
	float4 diffuse;

	float Bright_ = (bright / 255.0f);
	inData.color.a = 1.0f;

	float4 ambient = g_texture.Sample(g_sampler, inData.uv) * float4(Bright_, Bright_, Bright_, 1);

	if (isTexture)
	{
		diffuse = chroma * g_texture.Sample(g_sampler, inData.uv) * inData.color;
		return chroma * (diffuse + ambient);
	}
	else
	{
		return chroma * diffuseColor * inData.color;
	}
}
//Illegal character in shader file のエラーが出たら使用不可の全角が使用されている