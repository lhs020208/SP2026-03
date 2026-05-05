//--------------------------------------------------------
//FS.glsl
//--------------------------------------------------------
#version 330

layout(location=0) out vec4 FragColor;

in vec2 v_Tex;
uniform float u_Time;
uniform sampler2D u_RGBTex;
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
void Line()
{
	FragColor = vec4(sin(v_Tex.x * 10 * c_PI));
}

void Flag()
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
		discard;
	}

	FragColor = vec4(grey );
}

void Flame()
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
		discard;
	}

	FragColor = vec4(grey );
}

void textureSampling()
{
	FragColor = texture(u_RGBTex, vec2(v_Tex.x, v_Tex.y));
}

void textureSamplingBluh()
{
	vec4 c0;
	vec4 c1;
	vec4 c2;
	vec4 c3;
	vec4 c4;

	float offsetX = 0.01;
	c0 = texture(u_RGBTex, vec2(v_Tex.x - offsetX * 2, v_Tex.y));
	c1 = texture(u_RGBTex, vec2(v_Tex.x - offsetX * 1, v_Tex.y));
	c2 = texture(u_RGBTex, vec2(v_Tex.x - offsetX * 0, v_Tex.y));
	c3 = texture(u_RGBTex, vec2(v_Tex.x + offsetX * 1, v_Tex.y));
	c4 = texture(u_RGBTex, vec2(v_Tex.x + offsetX * 2, v_Tex.y));

	vec4 sum = c0 + c1 + c2 + c3 + c4;
	sum = sum / 5;

	FragColor = sum;
}

void textureSamplingMix1()
{
	float tx = v_Tex.x;
	float ty = v_Tex.y;
	ty = -abs(1 - v_Tex.y * 2);

	vec2 tex = vec2(tx, ty);
	FragColor = texture(u_RGBTex, tex);

}

void textureSamplingMix2()
{
	float tx = v_Tex.x;
	float ty = v_Tex.y;

	tx = fract(tx * 3);
	ty = ty / 3;

	float offsetX = 0;
	float offsetY = (2 - floor(v_Tex.x * 3))/3;
	

	vec2 tex = vec2(tx + offsetX, ty + offsetY);
	FragColor = texture(u_RGBTex, tex);
}

void textureSamplingMix3()
{
	float tx = v_Tex.x;
	float ty = v_Tex.y;

	tx = fract(tx * 3);
	ty = ty / 3;

	float offsetX = 0;
	float offsetY = (floor(v_Tex.x * 3))/3;
	

	vec2 tex = vec2(tx + offsetX, ty + offsetY);
	FragColor = texture(u_RGBTex, tex);
}

void main()
{
	textureSamplingMix3();
}