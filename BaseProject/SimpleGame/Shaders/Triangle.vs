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

void Falling()
{
	float newtime = u_Time - a_RV2;
	vec4 newPosition;

	if (newtime > 0.0)
	{
		float t = mod(newtime, 1.0);
		float tt = t * t;
		float vx = a_Vel.x / 5;
		float vy = a_Vel.y / 5;
		float rv = a_RV1;
		float rv2 = psudoRandom(a_RV2);

		float initPosx = a_Position.x * rv2 + 0.8 * cos(rv * 2.0 * C_PI);
		float initPosy = a_Position.y * rv2 + 0.8 * sin(rv * 2.0 * C_PI);

		newPosition.x = initPosx + vx * t + 0.5 * C_G.x * tt;
		newPosition.y = initPosy + vy * t + 0.5 * C_G.y * tt;
		newPosition.z = 0.0;
		newPosition.w = 1.0;
	}
	else
	{
		newPosition.x = 10000.0;
		newPosition.y = 10000.0;
		newPosition.z = 0.0;
		newPosition.w = 1.0;
	}

	gl_Position = newPosition;
}

void Rose()
{
	float newtime = u_Time - a_RV2;
	vec4 newPosition;

	if (newtime > 0.0)
	{
		float t = mod(newtime, 1.0);
		float rv1 = a_RV1;
		float rv2 = psudoRandom(a_RV2);

		float angle = rv1 * 2.0 * C_PI + t * 2.0 * C_PI;

		// ¿ÂπÃ∞Óº±: r = a * cos(k*theta)
		float a = 0.7;
		float k = 4.0 + floor(rv2 * 3.0); // 4~6 ¡§µµ∑Œ ≤…¿Ÿ ºˆ ∫Ø»≠
		float radius = a * cos(k * angle);

		newPosition.x = a_Position.x * 0.5 + radius * cos(angle);
		newPosition.y = a_Position.y * 0.5 + radius * sin(angle);

		newPosition.z = 0.0;
		newPosition.w = 1.0;
	}
	else
	{
		newPosition = vec4(10000.0, 10000.0, 0.0, 1.0);
	}

	gl_Position = newPosition;
}

void main()
{
	Rose();
}