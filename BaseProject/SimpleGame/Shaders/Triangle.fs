//--------------------------------------------------------
//Triangle.fs
//--------------------------------------------------------
#version 330

layout(location=0) out vec4 FragColor;

in float v_Grey;

void main()
{
	FragColor = vec4(1, (1 - v_Grey), 0, 1) * v_Grey;
}
