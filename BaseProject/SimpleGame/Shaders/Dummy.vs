//--------------------------------------------------------
//Dummy.vs
//--------------------------------------------------------
#version 330

uniform float u_Time;

in vec3 a_Position;

out float v_Grey;
out vec2 v_Tex;

const float C_PI = 3.141592;

void Frag()
{
	float tX, tY;
	tX = a_Position.x + 0.5;
	tY = 1.0 - (a_Position.y + 0.5);
	v_Tex = vec2(tX, tY);

	float value = a_Position.x + 0.5;
	float newX = a_Position.x;
	float newY = a_Position.y * (1.0 - value * 0.8) + 
								value * 0.25 * sin((newX + 0.5) * 2.0 * C_PI - u_Time);

	vec4 final = vec4(newX, newY, 0, 1);
	vec4 newPosition = final;

	gl_Position = newPosition;
	v_Grey = (sin((newX + 0.5) * 2.0 * C_PI - u_Time) + 1.0) / 2.0;
}
void main()
{
	Frag();
}
