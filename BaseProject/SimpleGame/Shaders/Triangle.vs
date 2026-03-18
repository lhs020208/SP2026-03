//--------------------------------------------------------
//Triangle.vs
//--------------------------------------------------------
#version 330

uniform float u_Time;
in vec3 a_Position;
in float a_Mass;
in vec2 a_Vel;
in float a_RV;

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

void Falling()
{
	float t = mod(u_Time, 1.0);
	float tt = t * t;
	float vx = a_Vel.x;
	float vy = a_Vel.y;
	float rv = a_RV;

	float initPosx = a_Position.x + 0.5 * cos(rv * 2 * C_PI);
	float initPosy = a_Position.y + 0.5 * sin(rv * 2 * C_PI);
	vec4 newPosition;

	newPosition.x = initPosx + vx * t + 0.5 * C_G.x * tt;
	newPosition.y = initPosy + vy * t + 0.5 * C_G.y * tt;
	newPosition.z = 0;
	newPosition.w = 1.0;

	gl_Position = newPosition;
}

void main()
{
	Falling();
}