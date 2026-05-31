//--------------------------------------------------------
//Dummy.vs
//--------------------------------------------------------
#version 330

uniform float u_Time;
uniform vec4 u_RainInfo[500];

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
	float newY = a_Position.y * (1.0 - value * 0.5) + 
								value * 0.25 * sin((newX + 0.5) * 2.0 * C_PI - u_Time);

	vec4 final = vec4(newX, newY, 0, 1);
	vec4 newPosition = final;

	gl_Position = newPosition;
	v_Grey = (sin((newX + 0.5) * 2.0 * C_PI - u_Time) + 1.0) / 2.0;
}

void Circles()
{
	float accum = 0;

	for (int i = 0; i < 500; i++)
	{
		vec2 center = u_RainInfo[i].xy - vec2(0.5, 0.5);
		vec2 pos = a_Position.xy;
		float lTime = u_RainInfo[i].z;
		float sTime = u_RainInfo[i].x;
		float nTime = u_Time - sTime;

		if (nTime > 0)
		{
			float lVal = fract(nTime / lTime);
			float OneMinus = 1.0 - lVal;
			float t = lVal * lTime;
			float d = distance(center, pos);
			float range = (t / 20.0);
			float fade = 30.0 * clamp(range - d, 0, 1.0);

			float sinValue = pow(abs(sin(d * 4 * C_PI * 8 - t * 2)), 3.0);

			accum += sinValue * fade * OneMinus;
		}
	}

	v_Grey = accum;
	//gl_Position = vec4(a_Position, 1.0);
	gl_Position = vec4(a_Position.x, a_Position.y + 0.1 * accum, a_Position.z, 1.0);
}

void main()
{
	Circles();
}
