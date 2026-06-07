//--------------------------------------------------------
//Triangle.vs
//--------------------------------------------------------
#version 330

uniform float u_Time;
in vec3 a_Position;
in float a_Mass;
in vec2 a_Vel;
in float a_RV1;
in float a_RV2;
in float a_RV3;
in vec2 a_Tex;
in vec3 a_RGB;

out float v_Grey;
out vec3 v_Color;
out vec2 v_Tex;

const float C_PI = 3.141592;
const vec2 C_G = vec2(0.0, -9.8);

void Basic()
{
	float t = mod(u_Time, 1.0);

	vec4 newPosition;
	

	newPosition.x = a_Position.x + t;
	newPosition.y = a_Position.y;
	newPosition.z = a_Position.z;
	newPosition.w = 1.0;

	gl_Position = newPosition;
}

float psudoRandom(float n)
{
	return fract(sin(n) * 43758.5453123);
}

void shape()
{
	float lifetime = 0.5 + 5.0 * a_RV1;
	float startTime = 5.0 * a_RV2;

	float newTime = u_Time - startTime;
	if (newTime > 0.0)
	{
		float t = fract(newTime / lifetime) * lifetime;
		float tt = t*t;

		float newX = a_Position.x + a_Vel.x * t;
		float newY = a_Position.y + a_Vel.y * t;

		gl_Position = vec4(newX, newY, 0, 1);
		v_Grey = 1 - fract(newTime / lifetime);
	}
	else
	{
		gl_Position = vec4(-100,0,0,1);
		v_Grey = 1.0;
	}

	v_Color = a_RGB;
	v_Tex = a_Tex;
}

void Falling()
{
    float startTime = a_RV2 * 3;
    float newTime = u_Time - startTime;

    if(newTime > 0)
    {
        float lifeScale = 2.0;
        float lifeTime = 0.5 + a_RV3 * lifeScale;
        float t = lifeTime*fract(newTime/lifeTime); //0~lifeTime구간 반복
        float tt = t*t;
        float vx, vy;
        float sx, sy;
        vx = a_Vel.x/30;
        vy = a_Vel.y/30;

        sx = a_Position.x * (1-psudoRandom(a_RV1)) + sin(a_RV1*2*C_PI);
        sy = a_Position.y * (1-psudoRandom(a_RV1)) + cos(a_RV1*2*C_PI);

        vec4 newPos;
        newPos.x = sx + vx*t;
        newPos.y = sy + vy*t + 0.5*C_G[1]*tt;
        newPos.z = 0;
        newPos.w = 1;

        gl_Position = newPos;
        v_Grey = 1;
    }
    else
    {
        gl_Position = vec4(-1000, 0, 0, 1);
    }

	v_Color = a_RGB;
	v_Tex = a_Tex;
}

void main()
{
	Falling();
}