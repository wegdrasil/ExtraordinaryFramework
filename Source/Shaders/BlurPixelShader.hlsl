SamplerState samplerState;
Texture2D tex;

float4 main(in float2 oTex : TEXCOORD) : SV_TARGET
{
	float4 color = tex.Sample(samplerState, oTex);
	return color;
}