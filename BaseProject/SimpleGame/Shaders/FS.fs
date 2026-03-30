//--------------------------------------------------------
//FS.fs
//--------------------------------------------------------
#version 330

layout(location=0) out vec4 FragColor;

in vec2 v_Tex;

void main()
{
	if(v_Tex.x < 0.5){
		FragColor = vec4(0);
	}
	else{
		FragColor = vec4(1);
	}
	FragColor = vec4(sin(v_Tex.x * 10 * 3.141592), cos (v_Tex.x * 10 * 3.141592),0,1);
}