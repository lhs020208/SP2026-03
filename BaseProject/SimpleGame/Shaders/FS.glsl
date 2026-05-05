//--------------------------------------------------------
//FS.glsl
//--------------------------------------------------------
#version 330

layout(location=0) out vec4 FragColor;

in vec2 v_Tex;
uniform float u_Time;
uniform sampler2D u_RGBTex;
uniform sampler2D u_CurrNumTex;
uniform sampler2D u_NumsTex;
uniform int u_inputNum;

uniform vec4 u_Points[500];

const float c_PI = 3.1415926535897932384626433832795;

void simple()
{
	if(v_Tex.x < 0.5){
	FragColor = vec4(0);
	}
	else{
	FragColor = vec4(1);
	}
}

void textureSampling()
{
	FragColor = texture(u_RGBTex, vec2(v_Tex.x, v_Tex.y));
}

void textureSamplingMix4()
{
	float resolX = 5;
	float resolY = 3;
	float shear = 0.5 * u_Time;

	float offsetX = fract(ceil(v_Tex.y * resolY) * shear);
	float offsetY = 0;

	float tx = fract(v_Tex.x * resolX + offsetX);
	float ty = fract(v_Tex.y * resolY + offsetY);

	vec2 tex = vec2(tx, ty);
	FragColor = texture(u_RGBTex, tex);
}

void Num()
{
	float tx = v_Tex.x;
	float ty = v_Tex.y;

	float offsetX = 0;
	float offsetY = 0;

	vec2 tex = vec2(tx + offsetX, ty + offsetY);
	FragColor = texture(u_CurrNumTex, tex);
}

void Nums()
{
	float index = float(u_inputNum);
	float tx = v_Tex.x / 5;
	float ty = v_Tex.y / 2;

	float offsetX = fract(index / 5.0);
	float offsetY = floor((index / 5.0)) / 2.0;

	vec2 tex = vec2(tx + offsetX, ty + offsetY);
	FragColor = texture(u_NumsTex, tex);
}

void main()
{
	Nums();
}