//--------------------------------------------------------
//Triangle.vs
//--------------------------------------------------------
#version 330

uniform float u_Time;
in vec3 a_Position;
in float a_Mass;
in vec2 a_Vel;
in float a_RV1;
in float a_RV2;
in float a_RV3;

out float v_Grey;

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

float psudoRandom(float n)
{
	return fract(sin(n) * 43758.5453123);
}

void Falling()
{
	float newtime = u_Time - a_RV2 * 3;
	vec4 newPosition;

	if (newtime > 0.0)
	{
		float lifeTime = (a_RV3 + 0.5) * 0.2;
		float t = mod(newtime, lifeTime);
		float tt = t * t;
		float vx = a_Vel.x / 5;
		float vy = a_Vel.y / 5;
		float rv = a_RV1;

		float size = psudoRandom(a_RV1) * ((lifeTime - t)/lifeTime);
		float initPosx = a_Position.x * size + 0.8 * cos(rv * 2.0 * C_PI);
		float initPosy = a_Position.y * size + 0.8 * sin(rv * 2.0 * C_PI);

		newPosition.x = initPosx + vx * t + 0.5 * C_G.x * tt;
		newPosition.y = initPosy + vy * t + 0.5 * C_G.y * tt;
		newPosition.z = 0.0;
		newPosition.w = 1.0;
	}
	else
	{
		newPosition.x = 10000.0;
		newPosition.y = 10000.0;
		newPosition.z = 0.0;
		newPosition.w = 1.0;
	}

	gl_Position = newPosition;
}

void Thrust()
{
	float newTime = u_Time - a_RV1;
	float t = mod(newTime, 1.0);
	vec4 newPosition;

	if (newTime > 0.0)
	{
		vec4 newPosition;

		float moveX = -1.0 + 2.0 * t;

		float Amplitude = (a_RV2 - 0.5) * t;
		float period = a_RV3;
		float Frequency = period * C_PI * 2;

		float size = t * 2;

		newPosition.x = a_Position.x * (size) + moveX;
		newPosition.y = a_Position.y * (size) + Amplitude * sin(t * Frequency);
		newPosition.z = a_Position.z;
		newPosition.w = 1.0;

		gl_Position = newPosition;
		v_Grey = 1 - t;
	}
	else
	{
		newPosition.x = 10000.0;
		newPosition.y = 10000.0;
		newPosition.z = 0.0;
		newPosition.w = 1.0;
		gl_Position = newPosition;
		v_Grey = 0;
	}
}

void Thrust2()
{
	float newTime = u_Time - a_RV1 * 0.15;
	vec4 newPosition;

	if (newTime > 0.0)
	{
		float lifeTime = 0.45 + a_RV3 * 0.2;
		float t = mod(newTime, lifeTime);
		float nt = t / lifeTime;

		// 왼쪽 -> 오른쪽 이동
		float moveX = -1.05 + nt * 2.1;

		// 앞쪽(좌측)은 두껍고 둥글게, 뒤로 갈수록 급격히 가늘어짐
		float widthY = 0.22 * pow(1.0 - nt, 1.8);
		float widthX = 0.10 * pow(1.0 - nt, 0.7);

		// 화염 머리 쪽을 조금 더 부풀리기
		float headBulge = 1.0 + 0.8 * exp(-nt * 8.0);

		// 입자별 세로 퍼짐
		float spreadY = (a_RV2 - 0.5) * 2.0 * widthY * headBulge;

		// 약간의 흔들림
		float wobble = sin(nt * (12.0 + a_RV3 * 18.0)) * widthY * 0.35;

		// 꼬리 쪽 약간의 위아래 랜덤 흔들림
		float noise = (psudoRandom(a_RV2 * 97.1 + floor(newTime * 40.0)) - 0.5) * widthY * 0.5;

		newPosition.x = moveX + a_Position.x * widthX;
		newPosition.y = spreadY + wobble + noise + a_Position.y * widthY;
		newPosition.z = a_Position.z;
		newPosition.w = 1.0;

		gl_Position = newPosition;

		// 앞은 밝고, 뒤는 어둡게
		v_Grey = 1.0 - pow(nt, 0.6);
	}
	else
	{
		newPosition = vec4(10000.0, 10000.0, 0.0, 1.0);
		gl_Position = newPosition;
		v_Grey = 0.0;
	}
}

void main()
{
	Thrust2();
}