//--------------------------------------------------------
//FS.glsl
//--------------------------------------------------------
#version 330

layout(location=0) out vec4 FragColor;
layout(location=1) out vec4 FragColor1;
layout(location=2) out vec4 FragColor2;

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

vec4 RainDrop()
{
	float accum = 0;
	for (int i = 0; i < 500; ++i)
	{
		float sTime = u_Points[i].z;
		float lTime = u_Points[i].w;
		float newTime = u_Time - sTime;
		if (newTime > 0)
		{
			float t = fract(newTime / lTime);
			float OneMinus = 1 - t;
			t = t * lTime;
			vec2 center = u_Points[i].xy;
			vec2 currPos = v_Tex;
			float count = 5;
			float range = t/5;

			float d = distance(center, currPos);
			float fade = (1/range) * clamp(range - d, 0, 1);

			float grey = pow(
				abs(sin(d * 4 * c_PI * count - newTime * 10)), 
				4);

			accum += grey * fade * OneMinus;
		}
	}
	return vec4(accum);
}

vec4 Flag()
{
	float amp = 0.5;
	float speed = 8;
	float sininput = v_Tex.x * c_PI * 2 - u_Time * speed;
	float sinValue = v_Tex.x *amp*(((sin(sininput) + 1) / 2)-0.5)+ 0.5;
	
	float FWidth = 0.0;
	float width = 0.5 * mix(1, FWidth, v_Tex.x);
	float grey = 0;

	if(v_Tex.y < sinValue + width/2 && v_Tex.y > sinValue - width/2)
	{
		grey = 1;
	}
	else
	{
		grey = 0;
	}

	return vec4(grey);
}

vec4 Flame()
{
	float amp = 0.5;
	float speed = 8;
	float newY = 1 - v_Tex.y;

	float sininput = newY * c_PI * 2 - u_Time * speed;
	float sinValue = newY *amp*(((sin(sininput) + 1) / 2)-0.5)+ 0.5;
	
	float FWidth = 0.0;
	float width = 0.5 * mix(1, FWidth, newY);
	float grey = 0;

	if(v_Tex.x < sinValue + width/2 && v_Tex.x > sinValue - width/2)
	{
		grey = 1;
	}
	else
	{
		grey = 0;
	}

	return vec4(grey);
}

void main()
{
	FragColor = RainDrop();
	FragColor1 = Flame();
	FragColor2 = Flag();
}