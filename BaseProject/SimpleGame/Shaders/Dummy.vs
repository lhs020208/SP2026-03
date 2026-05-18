//--------------------------------------------------------
//Dummy.vs
//--------------------------------------------------------
#version 330

uniform float u_Time;

in vec3 a_Position;

out float v_Grey;

const float C_PI = 3.141592;

void main()
{
	float sinvalue = -sin(2 * C_PI * a_Position.x - u_Time);
	float newX = a_Position.x + 0;
	float newY = a_Position.y + sinvalue * 0.25 * (a_Position.x + 0.5);

	vec4 newPosition = vec4(newX, newY, 0.0, 1.0);
	gl_Position = newPosition;
	v_Grey = (0.8 + sinvalue) / 2;
}
