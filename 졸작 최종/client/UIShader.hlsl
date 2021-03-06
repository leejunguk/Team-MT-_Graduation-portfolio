#ifndef UI_SHADER_HLSL
#define UI_SHADER_HLSL

#include "Shaders.hlsl"

#ifndef NUM_MAX_TEXTURE
#define NUM_MAX_TEXTURE 2
#endif

#ifndef NUM_MAX_UITEXTURE
#define NUM_MAX_UITEXTURE 2
#endif

Texture2D gUITextures[NUM_MAX_UITEXTURE] : register(t1);

cbuffer cbUIInfo : register(b6)
{
	float2  gxmf2ScreenPos;
	float2  gxmf2ScreenSize;

	uint2   gnNumSprite;
	uint2   gnNowSprite;

	uint2   gnSize;
	uint    gnTexType;
	float   gfData;

	float2 gfScale;
	float  dump1;
	float  dump2;
};

VS_TEXTURED_OUTPUT VSUITextured(uint nVertexID : SV_VertexID)
{
	VS_TEXTURED_OUTPUT output = (VS_TEXTURED_OUTPUT)0.0f;

	float2 screenpos = (float2) 0.0f;
	screenpos.x = (gxmf2ScreenPos.x - gxmf2ScreenSize.x / 2.0f) / (gxmf2ScreenSize.x / 2.0f);
	screenpos.y = (gxmf2ScreenPos.y - gxmf2ScreenSize.y / 2.0f) / (gxmf2ScreenSize.y / 2.0f);

	float2 size = (float2) 0.0f;
	size.x = ((float)gnSize.x / gxmf2ScreenSize.x) * gfScale.x;
	size.y = ((float)gnSize.y / gxmf2ScreenSize.y) * gfScale.y;

	if (nVertexID == 0)
	{
		output.uv = (float2(0.0f, 0.0f)); // 스크린 왼쪽 위 
		output.position = float4(screenpos.x - size.x, screenpos.y + size.y, 0.0f, 1.0f);
	}
	if (nVertexID == 1)
	{
		output.uv = (float2(1.0f, 0.0f)); // 스크린 오른쪽 위
		output.position = float4(screenpos.x + size.x, screenpos.y + size.y, 0.0f, 1.0f);

	}
	if (nVertexID == 2)
	{
		output.uv = (float2(1.0f, 1.0f)); // 스크린 오른쪽 아래
		output.position = float4(screenpos.x + size.x, screenpos.y - size.y, 0.0f, 1.0f);

	}
	if (nVertexID == 3)
	{
		output.uv = (float2(0.0f, 0.0f)); // 스크린 왼쪽 위
		output.position = float4(screenpos.x - size.x, screenpos.y + size.y, 0.0f, 1.0f);

	}
	if (nVertexID == 4)
	{
		output.uv = (float2(1.0f, 1.0f)); // 스크린 왼쪽 아래
		output.position = float4(screenpos.x + size.x, screenpos.y - size.y, 0.0f, 1.0f);

	}
	if (nVertexID == 5)
	{
		output.uv = (float2(0.0f, 1.0f)); // 스크린 오른쪽 아래
		output.position = float4(screenpos.x - size.x, screenpos.y - size.y, 0.0f, 1.0f);
	}

	return output;
};


float4 PSMiniMap(VS_TEXTURED_OUTPUT input) : SV_Target
{
	float4 finalColor = (float4) 0.0f;
	float2 uv = input.uv;

	uv = float2(input.uv.x / gnNumSprite.x + gfData / gnNumSprite.x, input.uv.y);

	if (gnTexType == 0)
		finalColor = gUITextures[0].Sample(gWrapSamplerState, uv);
	else if (gnTexType == 1)
		finalColor = gUITextures[1].Sample(gWrapSamplerState, uv);

	return finalColor;
}

float4 PSUIHPBar(VS_TEXTURED_OUTPUT input) : SV_Target
{

	float4 finalColor = (float4) 0.0f;
	float2 uv = input.uv;

	if (gnTexType == 0)
		finalColor = gUITextures[0].Sample(gWrapSamplerState, uv);
	else if (gnTexType == 1)
	{
		if (uv.x < gfData)
			finalColor = gUITextures[1].Sample(gWrapSamplerState, uv);
	}
	else if (gnTexType == 2)
		finalColor = gUITextures[2].Sample(gWrapSamplerState, uv);
	else if (gnTexType == 3)
		finalColor = gUITextures[3].Sample(gWrapSamplerState, uv);

	return finalColor;

}
#endif