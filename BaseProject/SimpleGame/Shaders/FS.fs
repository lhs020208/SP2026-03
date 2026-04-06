//--------------------------------------------------------
//FS.fs
//--------------------------------------------------------
#version 330

layout(location=0) out vec4 FragColor;

in vec2 v_Tex;
uniform float u_Time;
uniform vec4 u_Points[500];

const float c_PI = 3.1415926535897932384626433832795;
const vec4 c_Points[10] = vec4[10](
		vec4(0.3, 0.5, 0.1, 1.4),
		vec4(0.2, 0.2, 0.2, 2.4),
		vec4(0.1, 0.7, 0.3, 1.5),
		vec4(0.6, 0.1, 0.4, 0.9),
		vec4(0.5, 0.3, 0.5, 1.0),
		vec4(0.7, 0.9, 0.6, 2.4),
		vec4(0.6, 0.4, 0.7, 0.8),
		vec4(0.8, 0.3, 0.8, 3.0),
		vec4(0.1, 0.5, 0.9, 0.9),
		vec4(0.5, 0.7, 0.5, 1.1));

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
	float trans = c_PI/2;
	float periodX = (v_Tex.x * 2 * c_PI - trans) * 5;
	float periodY = (v_Tex.y * 2 * c_PI - trans) * 5;
	float valueX = pow(abs(sin(periodX)),16);
	float valueY = pow(abs(sin(periodY)),16);

	float value = max(valueX, valueY);
	//if (value <= 0.5) value = 0;
	//else if (value > 0.5) value = 1;

	FragColor = vec4(value);
}
void Circle()
{
	vec2 center = vec2(0.5, 0.5);
	vec2 currPos = v_Tex;
	float distance = distance(center, currPos);

	float radius = 0.5;
	float small_radius = radius - 0.01;

	if (distance > small_radius) {
		if (distance < radius) {
			FragColor = vec4(0);
		}
	}
	
	else {
		FragColor = vec4(1);
	}
}
void Circles()
{
    vec2 center = vec2(0.5, 0.5);
    vec2 currPos = v_Tex;
    float distance = distance(center, currPos);
	distance = mod(distance - u_Time * 0.1, 0.5);

    int count = 10;
    float radius = 0.5;
    float thickness = 0.02;

    FragColor = vec4(0);

    for (int i = 0; i < count; ++i)
    {
        float outer = radius - float(i) * (radius / float(count));
        float inner = outer - thickness;

        if (distance < outer && distance > inner)
        {
            FragColor = vec4(1);
        }
    }
}
void Circles2()
{
	vec2 center = vec2(0.5, 0.5);
	vec2 currPos = v_Tex;
	float count = 5;

	float d = distance(center, currPos);

	float grey = pow(
		abs(sin(d * 4 * c_PI * count - u_Time*10)), 
		4);
	FragColor = vec4(grey);
}
void RainDrop()
{
	float accum = 0;
	for (int i = 0; i < 500; ++i)
	{
		float sTime = u_Points[i].z;
		float lTime = u_Points[i].w;
		float newTime = u_Time - sTime;
		if (newTime > 0)
		{
			float t = fract(newTime / lTime);
			float OneMinus = 1 - t;
			t = t * lTime;
			vec2 center = u_Points[i].xy;
			vec2 currPos = v_Tex;
			float count = 5;
			float range = t/5;

			float d = distance(center, currPos);
			float fade = (1/range) * clamp(range - d, 0, 1);

			float grey = pow(
				abs(sin(d * 4 * c_PI * count - newTime * 10)), 
				4);

			accum += grey * fade * OneMinus;
		}
	}
	FragColor = vec4(accum,0,0,1);
}

void main()
{
	RainDrop();
}