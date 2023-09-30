struct PS_IN
{
	// スクリーンの座標-1～1はw成分で割ると正しく計算できる
	// x' = x/w, y' = y/w, z' = z/w
	// 「SV_POSITIONのパラメータ」は頂点シェーダ―から
	// ピクセルシェーダーに渡される途中で勝手に計算されている
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float2 uv : TEXCOORD0;
	float4 wPos : TEXCOORD1;
	float4 sunPos : TEXCOORD2;
};

// テクスチャ
// register(t0)はモデルに張り付けるデフォルトのテクスチャ
Texture2D sunViewTex : register(t2);
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_TARGET
{
	// 座標をwで割る
	float2 sunUV = pin.sunPos.xy / pin.sunPos.w;
	// 画面の座標-1～1をテクスチャの座標0～1に変換
	sunUV = sunUV * 0.5f + 0.5f;
	// 画面のY座標は下から上に向かって増えるが、
	// テクスチャのY座標は上から下に向かって増えるので反転
	sunUV.y = 1.0f - sunUV.y;


	// 奥行情報を書き込んだテクスチャから、距離を取得
	float sunDepth = sunViewTex.Sample(samp, sunUV).r;

	// シャドウアクネ(マッハバンド)対策
	// 描画するピクセルの奥行と、テクスチャの奥行が一致している場合、
	// テクスチャに書き込まれたデータとの差異が発生して影ができない部分に
	// 影ができてしまう。テクスチャに書き込まれた距離は実際は少し遠い、と置くことで回避できる。
	sunDepth += 0.001f;		// 数字は適宜調整

	// メインカメラで表示しようとしているピクセルの太陽から見た距離を取得
	float pixelDepth = pin.sunPos.z / pin.sunPos.w;


	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	// 現在のピクセルの奥行きがテクスチャに書き込まれている奥行きよりも遠い場合、影と判定する。
	// ※ピクセルシェーダーではピクセルごとに計算するので、あまりif分は使わない方がよい
	if (sunDepth < pixelDepth)
	{
		color.rgb = 0.2f;	// 影色
	}

	return color;
}