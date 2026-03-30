//--------------------------------------------------------
//FS.vs
//--------------------------------------------------------
#version 330

uniform float u_Time;
in vec3 a_Position;
in vec2 a_Tex;

out vec2 v_Tex;

void main()
{
	vec4 newPosition;
	newPosition.x = a_Position.x;
	newPosition.y = a_Position.y;
	newPosition.z = 0.0;
	newPosition.w = 1.0;

	v_Tex = a_Tex;
	gl_Position = newPosition;
}