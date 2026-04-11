//--------------------------------------------------------
//FS.glsl
//--------------------------------------------------------
#version 330

layout(location=0) out vec4 FragColor;

in vec2 v_Tex;
uniform float u_Time;
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

void main()
{
	Flame();
}