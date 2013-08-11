cbuffer cbPerObject 
{
	//float4x4 World;
	float4x4 WorldViewProj;
	//float4x4 NormalTransformMatrix;
};

void VS(
	    float3 iPos : POSITION,
		float3 iNormal : NORMAL,
		float2 iTex : TEXCOORD,
		out float4 oPosH : SV_POSITION
		//out float4 oPosW : POSITION,
		//out float2 oTex : TEXCOORD,
		//out float4 oNormalW : NORMAL
		)
{
	//oPosW = mul(float4(iPos, 1.0f), World);
	//oNormalW = mul(float4(iNormal, 0.0f), NormalTransformMatrix);
	oPosH = mul(float4(iPos, 1.0f), WorldViewProj);
	//oTex = iTex;
}