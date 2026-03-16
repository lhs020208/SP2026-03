//--------------------------------------------------------
//Triangle.vs
//--------------------------------------------------------
#version 330

uniform float u_Time;
in vec3 a_Position;

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

void SinMove1()
{
	float t = mod(u_Time, 1.0);

	vec4 newPosition;
	
	float Amplitude = 0.4;

	newPosition.x = a_Position.x + t;
	newPosition.y = a_Position.y + Amplitude * sin(t * 2.0 * 3.141592);
	newPosition.z = a_Position.z;
	newPosition.w = 1.0;

	gl_Position = newPosition;
}

void SinMove2()
{
	float t = mod(u_Time, 1.0);

	vec4 newPosition;

	float Amplitude = 0.4;
	int Count = 2;
	float Frequency = Count * 2.0 * 3.141592;

	// 왼쪽(-1)에서 오른쪽(1)까지 1회 이동
	float moveX = -1.0 + 2.0 * t;

	newPosition.x = a_Position.x + moveX;
	newPosition.y = a_Position.y + Amplitude * sin(t * Frequency);
	newPosition.z = a_Position.z;
	newPosition.w = 1.0;

	gl_Position = newPosition;
}

void CircleMove()
{
	float t = mod(u_Time, 1.0);
	vec4 newPosition;
	float Amplitude = 0.8;

	newPosition.x = a_Position.x + Amplitude * cos(t * 2.0 * 3.141592);
	newPosition.y = a_Position.y - Amplitude * sin(t * 2.0 * 3.141592);
	newPosition.z = a_Position.z;
	newPosition.w = 1.0;
	gl_Position = newPosition;
}


vec2 GetStarPoint(int idx, float rOuter, float rInner)
{
	float pi = 3.141592;
	float angle;
	float r;

	if (idx % 2 == 0)
		r = rOuter;
	else
		r = rInner;

	// 기존 각도에서 반바퀴 회전
	angle = float(idx) * pi / 5.0 - pi / 2.0 + pi;

	return vec2(cos(angle) * r, sin(angle) * r);
}

void StarMove()
{
	vec4 newPosition;

	float t = mod(u_Time, 1.0);

	// 별을 한 바퀴 도는 진행도
	float scaled = t * 10.0;
	int seg = int(floor(scaled));
	float localT = fract(scaled);

	vec2 p0 = GetStarPoint(seg % 10, 0.8, 0.35);
	vec2 p1 = GetStarPoint((seg + 1) % 10, 0.8, 0.35);

	vec2 offset = mix(p0, p1, localT);

	newPosition.x = a_Position.x + offset.x;
	newPosition.y = a_Position.y + offset.y;
	newPosition.z = a_Position.z;
	newPosition.w = 1.0;

	gl_Position = newPosition;
}

void SqareMove()
{
	float t = mod(u_Time, 1.0);
	vec4 newPosition;
	// 사각형의 네 꼭짓점
	vec2 corners[4] = vec2[4](
		vec2(-0.8, -0.8),
		vec2(0.8, -0.8),
		vec2(0.8, 0.8),
		vec2(-0.8, 0.8)
	);
	// 사각형을 한 바퀴 도는 진행도
	float scaled = t * 4.0;
	int seg = int(floor(scaled));
	float localT = fract(scaled);
	vec2 p0 = corners[seg % 4];
	vec2 p1 = corners[(seg + 1) % 4];
	vec2 offset = mix(p0, p1, localT);
	newPosition.x = a_Position.x + offset.x;
	newPosition.y = a_Position.y + offset.y;
	newPosition.z = a_Position.z;
	newPosition.w = 1.0;
	gl_Position = newPosition;
}

void RandomMove()
{
	vec4 newPosition;

	// 일정 시간마다 새 위치를 뽑도록 시간 구간화
	float tick = floor(u_Time * 100.0);

	// pseudo-random 함수
	float randX = fract(sin(tick * 12.9898) * 43758.5453123);
	float randY = fract(sin((tick + 100.0) * 78.233) * 43758.5453123);

	// 0~1 범위를 -1~1 범위로 변환
	float posX = randX * 2.0 - 1.0;
	float posY = randY * 2.0 - 1.0;

	newPosition.x = a_Position.x + posX;
	newPosition.y = a_Position.y + posY;
	newPosition.z = a_Position.z;
	newPosition.w = 1.0;

	gl_Position = newPosition;
}

void main()
{
	RandomMove();
}