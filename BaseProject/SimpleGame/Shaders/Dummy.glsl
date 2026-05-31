//--------------------------------------------------------
//Dummy.glsl
//--------------------------------------------------------
#version 330

layout(location=0) out vec4 FragColor;

uniform sampler2D u_PHTex;

in float v_Grey;
in vec2 v_Tex;

const float C_PI = 3.141592;

mat2 rotate2D(float radian)
{
	return mat2(cos(radian), -sin(radian), sin(radian), cos(radian));
}

void main()
{
	//FragColor = vec4(v_Grey);
	//FragColor = vec4(v_Tex,0,1);

	vec2 newTex = rotate2D(-C_PI / 2.0) * v_Tex;

	FragColor = texture(u_PHTex, newTex);
}
