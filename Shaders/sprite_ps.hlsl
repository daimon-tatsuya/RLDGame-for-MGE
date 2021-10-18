//float4 main() : SV_TARGET
//{
//	return float4(1.0f, 1.0f, 1.0f, 1.0f);
//}

// UNIT.04
Texture2D diffuse_map : register(t0);
SamplerState diffuse_map_sampler_state : register(s0);

// UNIT.02
#include "sprite.hlsli"
float4 main(VS_OUT pin) : SV_TARGET
{
	//	//return pin.color;
	//	float alpha = 1.0;
	////return pin.color;
	//float contrast = 5;
	//float threshold = 0.1;
	//float offset = pin.color.rgb - threshold;
	//pin.color.rgb = threshold + offset * contrast;
	//pin.texcoord += options.xy;
	float4 color = diffuse_map.Sample(diffuse_map_sampler_state, pin.texcoord) * pin.color;
//	float alpha = 0.f;

	return color;
}

