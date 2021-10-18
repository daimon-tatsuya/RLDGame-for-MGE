struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
};

cbuffer CONSTANT_BUFFER : register(b1)
{
	float4 options;
}

Texture2D diffuse_map : register(t0);
SamplerState diffuse_map_sampler_state : register(s0);

float4 main(VS_OUT pin) : SV_TARGET
{
	return 1;
	float4 color = diffuse_map.Sample(diffuse_map_sampler_state, pin.texcoord);
	float alpha = color.w;

	float offset_texcel = options.x;
	float2 offset;
	offset.x = offset_texcel / 1280;
	offset.y = offset_texcel / 720;

	color += diffuse_map.Sample(diffuse_map_sampler_state, pin.texcoord + float2(-offset.x, 0));
	color += diffuse_map.Sample(diffuse_map_sampler_state, pin.texcoord + float2(+offset.x, 0));
	color += diffuse_map.Sample(diffuse_map_sampler_state, pin.texcoord + float2(0, -offset.y));
	color += diffuse_map.Sample(diffuse_map_sampler_state, pin.texcoord + float2(0, +offset.y));
	//•½‹Ï
	color /= 5.0;

	return float4(color.rgb, alpha);
}