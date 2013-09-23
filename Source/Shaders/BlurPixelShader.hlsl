SamplerState samplerState;
Texture2D tex;

float4 main(in float4 oPosH : SV_POSITION,
	        in float2 oTex : TEXCOORD) : SV_TARGET
{
	return tex.Sample(samplerState, oTex);
}