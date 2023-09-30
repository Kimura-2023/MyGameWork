struct PS_IN
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 wPos : TEXCOORD1;
	float4 sunPos : TEXCOORD2;
};

cbuffer Light : register(b0)
{
	float4 lightDir;
};

SamplerState samp : register(s0);
Texture2D modelTex : register(t0);
Texture2D rampTex : register(t1);		// ランプテクスチャ
Texture2D sunViewTex : register(t2);	// 落ち影
Texture2D maskTex : register(t3);		// 透過するためのテクスチャ

// ---ディザパターン---
static const int pattern[4][4] = {
	{ 0,32, 8,40},
	{48,16,56,24},
	{12,44, 4,36},
	{60,28,52,20},
};

float4 main(PS_IN pin) : SV_TARGET
{
	// ---通常描画(基本的にToonPSと同じ)---
	float4 color = modelTex.Sample(samp,pin.uv);

	float3 N = normalize(pin.normal);
	float3 L = normalize(-lightDir.xyz);

	float diffuse = dot(N, L);

	// 内積の値を基にランプテクスチャの色を引っ張り出す
	// ※内積の結果は-1～1なので0～1に変換する
	diffuse = diffuse * 0.5f + 0.5f;
	float rampshadow = rampTex.Sample(samp, float2(diffuse,0.0f));

	float2 sunUV = pin.sunPos.xy / pin.sunPos.w;

	// 画面の座標-1～1をテクスチャの座標0～1に変換
	sunUV = sunUV * 0.5f + 0.5f;

	// 画面のY座標は下から上に向かって増えるが、テクスチャのY座標は上から下に向かって増えるので反転
	sunUV.y = 1.0f - sunUV.y;

	// 奥行情報を書き込んだテクスチャから、距離を取得
	float sunDepth = sunViewTex.Sample(samp, sunUV).r;
	sunDepth += 0.00025f;		// 数字は適宜調整
	// メインカメラで表示しようとしているピクセルの太陽から見た距離を取得
	float pixelDepth = pin.sunPos.z / pin.sunPos.w;

	float shadow = 1.0f;

	// 現在のピクセルの奥行きがテクスチャに書き込まれている奥行きよりも遠い場合、影と判定する。(デプスシャドウ)
	if (sunDepth < pixelDepth)
	{
		shadow = 0.5f;	// 影の強さ
	}

	color.rgb = color.rgb * rampshadow * shadow + 0.05f;

	// スクリーン上のUVを求める
	float2 texuv = pin.pos.xy;
	texuv.x /= 1280.0f;
	texuv.y /= 720.0f;

	int x = (int)fmod(pin.pos.x, 4.0f);
	int y = (int)fmod(pin.pos.y, 4.0f);
	int dither = pattern[y][x];

	clip(dither - maskTex.Sample(samp, texuv).r * 10);			// 移動オブジェクトが重なっていたらディザパターンで透過する

	color.rgb += maskTex.Sample(samp, texuv).r;

	return color;
}