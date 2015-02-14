#include <gl/glut.h>
#include <gl/gl.h>
#include <gl/GLU.h>
#include <gl/GLAux.h>
#include <math.h>
#include<stdio.h>
#include<Windows.h>

/* Animation list.
 0. stand
 1. walk
 2. run (질주)
 3. shoot
 4. pass
*/

// define animation status
#define ANI_STAND	0				// 서있는 모션
#define ANI_WALK	1				// 걷는 모션
#define ANI_RUN		2				// 뛰는 모션
#define ANI_SHOOT	3				// 슛 모션
#define ANI_PASS	4				// 패스 모션

// animation status
static int ani = ANI_STAND;			// 애니메이션 판별 변수 (서있는 자세가 기본)

// 키의 눌린 상태를 저장
bool SpecialKeyStates[4] = {false, false, false, false};		// 0:LEFT	1:UP	2:RIGHT		3:DOWN
bool KeyStates[2] = { false, false };							// 0:'E'	1:

// angle
// 팔 각도
GLfloat L_arm1_ang = 0.0f;			// arm1 : 어깨 각도
GLfloat L_arm2_ang = 0.0f;			// arm2 : 팔꿈치 각도
GLfloat R_arm1_ang = 0.0f;
GLfloat R_arm2_ang = 0.0f;
// 다리 각도
GLfloat L_leg1_ang = 0.0f;			// leg1 : 골반 각도
GLfloat L_leg2_ang = 0.0f;			// leg2 : 무릎 각도
GLfloat R_leg1_ang = 0.0f;
GLfloat R_leg2_ang = 0.0f;
// 허리 각도
GLfloat Waist_ang_x = 0.0f;
GLfloat Waist_ang_y = 0.0f;
// 목 각도
GLfloat Neck_ang = 0.0f;

// Charater direction
GLfloat character_angle= 0.0f;

// Character Position
GLfloat character_pos_x = 0.0f;
GLfloat character_pos_y = 0.0f;			// 중력 영향받음 
GLfloat character_pos_z = 0.0f;

// Character Speed
GLfloat speed = 0.0f;

// Animation time (speed)
static double walkTime = 0.0;
static double runTime = 0.0;

/*========== Physics Simulation ==========*/
GLfloat dt;							// 시간 변화량
const GLfloat acc = -9.8f;			// 중력가속도
GLfloat vel = 0.0f;					// vel += acc*dt
GLfloat pos = 0.0f;					// pos += vel*dt  , draw object at pos


/*========== Texture 변수 ==========*/
unsigned int MyTextureObject[1];
AUX_RGBImageRec *pTextureImage[1];			//텍스쳐 저장 공간을 가리키는 포인터

/*========== Modeling Objects ==========*/
void DrawHead(float xPos, float yPos, float zPos) {
	glPushMatrix();			// 행렬 스택에 현재 행렬을 저장
		glColor3f(0.5f, 0.5f, 0.5f);
		glTranslatef(xPos, yPos, zPos);
		glScalef(2.0f, 2.0f, 1.0f);					// 머리크기 2x2x2
		glutSolidCube(1);
	glPopMatrix();
}
void DrawTorso(float xPos, float yPos, float zPos) {
	glPushMatrix();			// 행렬 스택에 현재 행렬을 저장
		glColor3f(0.5f, 0.5f, 0.5f);
		glTranslatef(xPos, yPos, zPos);
		glScalef(4.0f, 4.0f, 1.0f);					// 몸통크기 3x4x1
		glutSolidCube(1);
	glPopMatrix();
}
void DrawPelvis(float xPos, float yPos, float zPos) {
	glPushMatrix();			// 행렬 스택에 현재 행렬을 저장
		glColor3f(0.0f, 0.0f, 1.0f);
		glTranslatef(xPos, yPos, zPos);
		glScalef(1.0f, 1.0f, 1.0f);					// 몸통크기 3x4x1
		glutSolidCube(1);
	glPopMatrix();
}
void DrawArm(float xPos, float yPos, float zPos) {
	glPushMatrix();			// 행렬 스택에 현재 행렬을 저장
		glColor3f(0.5f, 0.5f, 0.5f);
		glTranslatef(xPos, yPos, zPos);
		glScalef(1.0f, 3.0f, 1.0f);
		glutSolidCube(1);
	glPopMatrix();
}
void DrawLeg1(float xPos, float yPos, float zPos) {
	glPushMatrix();			// 행렬 스택에 현재 행렬을 저장
		glColor3f(0.5f, 0.5f, 0.5f);
		glTranslatef(xPos, yPos, zPos);
		glScalef(1.2f, 3.0f, 1.2f);
		glutSolidCube(1);
	glPopMatrix();
}
void DrawLeg2(float xPos, float yPos, float zPos) {
	glPushMatrix();			// 행렬 스택에 현재 행렬을 저장
		glColor3f(0.5f, 0.5f, 0.5f);
		glTranslatef(xPos, yPos, zPos);
		glScalef(1.0f, 4.0f, 1.0f);
		glutSolidCube(1);
	glPopMatrix();
}
void DrawFoot (float xPos, float yPos, float zPos) {
	glPushMatrix();			// 행렬 스택에 현재 행렬을 저장
		glColor3f(0.5f, 0.5f, 0.5f);
		glTranslatef(xPos, yPos, zPos);
		glScalef(1.0f, 1.0f, 2.0f);
		glutSolidCube(1);
	glPopMatrix();
}
// 관절
void DrawJoint() {
	glPushMatrix();
		glColor3f(1.0f, 0.0f, 0.0f);
		glutSolidSphere(0.5, 10, 10);
	glPopMatrix();
}

/*========== Draw Character ==========*/
void DrawCharacter(float xPos, float yPos, float zPos) {
	glTranslatef(xPos, yPos, zPos);			// 좌표계이동 : 캐릭터의 위치 (원점)
	DrawPelvis(0.0f, 0.0f, 0.0f);			// 원점에 골반을 그림

	glPushMatrix();							// 골반 좌표 등록
		glTranslatef(0.0f, 1.0f, 0.0f);		// 허리 관절 위치 잡기
		glRotatef(Waist_ang_x, 1.0f, 0.0f, 0.0f);
		glRotatef(Waist_ang_y, 0.0f, 1.0f, 0.0f);
		DrawJoint();						// 허리 관절 생성
		glTranslatef(0.0f, 2.0f, 0.0f);
		glPushMatrix();						// 몸통 중심좌표 등록
			DrawTorso(0.0f, 0.0f, 0.0f);			// 몸통 그리기
			
			glTranslatef(0.0f, 2.5f, 0.0f);			// 목 좌표계
			DrawJoint();							// 목 그리기
			DrawHead(0.0f, 1.0f, 0.0f);				// 머리를 그림
		glPopMatrix();
	glPopMatrix();
	
	// 왼팔 그리기
	glPushMatrix();							// 모델 중심 좌표 저장
		glTranslatef(2.5f, 4.5f, 0.0f);			// 좌표계이동 : 왼쪽어깨
		glRotatef(L_arm1_ang, 1.0f, 0.0f, 0.0f);
		DrawJoint();
		glRotatef(5, 0.0f, 0.0f, 1.0f);		// 어깨를 10도 기울임
		DrawArm(0.0f, -1.5f, 0.0f);				// 어깨보다 1.5 낮은위치에 윗팔을 그림
		glPushMatrix();							// 어깨좌표계 저장
			glTranslatef(0.0, -3.5f, 0.0f);			// 좌표계이동 : 왼쪽팔꿈치
			glRotatef(L_arm2_ang, 1.0f, 0.0f, 0.0f);
			DrawJoint();
			glRotatef(-15, 1.0f, 0.0f, 0.0f);
			DrawArm(0.0f, -1.5f, 0.0f);
			// 손을 그린다면 이곳에
		glPopMatrix();
	glPopMatrix();
	// 오른팔 그리기
	glPushMatrix();							// 모델 중심 좌표 저장
		glTranslatef(-2.5f, 4.5f, 0.0f);			// 좌표계이동 : 오른쪽어깨
		glRotatef(R_arm1_ang, 1.0f, 0.0f, 0.0f);
		DrawJoint();
		glRotatef(-5, 0.0f, 0.0f, 1.0f);		// 어깨를 10도 기울임
		DrawArm(0.0f, -1.5f, 0.0f);				// 어깨보다 1.5 낮은위치에 윗팔을 그림
		glPushMatrix();							// 어깨좌표계 저장
			glTranslatef(0.0, -3.5f, 0.0f); 		// 좌표계이동 : 오른쪽팔꿈치
			glRotatef(R_arm2_ang, 1.0f, 0.0f, 0.0f);
			DrawJoint();
			glRotatef(-15, 1.0f, 0.0f, 0.0f);
			DrawArm(0.0f, -1.5f, 0.0f);
			// 손을 그린다면 이곳에
		glPopMatrix();
	glPopMatrix();

	// 왼다리 그리기
	glPushMatrix();							// 모델 중심 좌표 저장
		glTranslatef(1.0f, 0.0f, 0.0f);		// 좌표계이동 : 왼쪽 골반
		glRotatef(L_leg1_ang, 1.0f, 0.0f, 0.0f);
		DrawJoint();
		DrawLeg1(0.0f, -1.5f, 0.0f);			// 골반보다 2 낮은위치에 허벅지를 그림
		glPushMatrix();							// 골반 좌표계 저장
			glTranslatef(0.0, -3.5f, 0.0f);		// 좌표계이동 : 왼쪽무릎
			glRotatef(L_leg2_ang, 1.0f, 0.0f, 0.0f);
			DrawJoint();								// 왼쪽무릎을 그리고
			DrawLeg2(0.0f, -2.0f, 0.0f);				// 왼쪽무릎아래를 그린다
			glPushMatrix();								// 무릎 좌표계 저장
				glTranslatef(0.0, -4.5f, 0.0f);				// 좌표계이동 : 왼쪽발목
				DrawJoint();								// 발목관절을 그리고
				DrawFoot(0.0f, -0.5f, 0.5f);				// 발을 그린다.
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
	// 오른다리 그리기
	glPushMatrix();							// 모델 중심 좌표 저장
		glTranslatef(-1.0f, 0.0f, 0.0f);		// 좌표계이동 : 오른쪽 골반
		glRotatef(R_leg1_ang, 1.0f, 0.0f, 0.0f);
		DrawJoint();
		DrawLeg1(0.0f, -1.5f, 0.0f);			// 골반보다 2 낮은위치에 허벅지를 그림
		glPushMatrix();							// 골반 좌표계 저장
			glTranslatef(0.0, -3.5f, 0.0f);		// 좌표계이동 : 오른쪽무릎
			glRotatef(R_leg2_ang, 1.0f, 0.0f, 0.0f);
			DrawJoint();								// 오른쪽무릎을 그리고
			DrawLeg2(0.0f, -2.0f, 0.0f);				// 오른쪽무릎아래를 그린다
			glPushMatrix();								// 무릎 좌표계 저장
				glTranslatef(0.0, -4.5f, 0.0f);				// 좌표계이동 : 오른쪽발목
				DrawJoint();								// 발목관절을 그리고
				DrawFoot(0.0f, -0.5f, 0.5f);				// 발을 그린다.
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}

/*========== Draw Animation ==========*/
void DrawStand (float xPos, float yPos, float zPos) {
	L_arm1_ang = 0;
	R_arm1_ang = 0;
	R_arm2_ang = 0;
	L_arm2_ang = 0;
	
	R_leg2_ang = 0;
	L_leg2_ang = 0;
	R_leg1_ang = 0;
	L_leg1_ang = 0;
	
	Waist_ang_x = 0.0f;
	Waist_ang_y = 0.0f;
	DrawCharacter(xPos, yPos, zPos);
}
void DrawWalk (float xPos, float yPos, float zPos) {
	
	L_arm1_ang = sin(walkTime)*60;				// 왼팔은 60도까지 움직이되 sin() 으로 주기적인 움직임 설정
	R_arm1_ang = -L_arm1_ang;					// 오른팔은 왼팔 반대 방향 80도 각도까지
	R_arm2_ang = -abs(sin(walkTime)*30+60);		// 오른팔 각도 조절 (절댓값을 줌으로써 팔이 뒤로 꺾이지 않음)
	L_arm2_ang = -abs(-sin(walkTime)*30+60);		// 왼팔 각도 조절
	
	R_leg2_ang = abs(-sin(walkTime)*30-30);			// 오른쪽 종아리 각도 조절  
	L_leg2_ang = abs(sin(walkTime)*30-30);			// 왼쪽 종아리 각도 조절
	R_leg1_ang = sin(walkTime)*30;					// 오른쪽 다리는 30도까지 움직이되 sin()으로 주기적인 움직임 설정
	L_leg1_ang = -R_leg1_ang;						// 왼쪽 다리는 오른쪽 다리 반대로
	
	// 캐릭터가 달리면서 앞, 뒤로 뒤뚱거리고 몸이 틀어지는 것을 표현
	Waist_ang_x = abs(sin(walkTime)*5);
	Waist_ang_y = sin(walkTime)*5;


	// 캐릭터가 달리면서 상하로 움직이는 것을 표현
	float i=0;
	i=abs(sin(walkTime)*1.0);					// i 변수 값 설정
	glTranslatef(0.0, i-0.5, 0);				// 로봇의 몸체가 y축 방향으로 변수 i만큼 이동
	
	DrawCharacter(xPos, yPos, zPos);
}
void DrawRun (float xPos, float yPos, float zPos) {

	L_arm1_ang = sin(runTime)*80;				// 왼팔은 80도까지 움직이되 sin() 으로 주기적인 움직임 설정
	R_arm1_ang = -L_arm1_ang;					// 오른팔은 왼팔 반대 방향 80도 각도까지
	R_arm2_ang = -abs(sin(runTime)*60+50);		// 오른팔 각도 조절 (절댓값을 줌으로써 팔이 뒤로 꺾이지 않음)
	L_arm2_ang = -abs(-sin(runTime)*60+50);		// 왼팔 각도 조절
	
	R_leg2_ang = abs(-sin(runTime)*30-30);			// 오른쪽 종아리 각도 조절  
	L_leg2_ang = abs(sin(runTime)*30-30);			// 왼쪽 종아리 각도 조절
	R_leg1_ang = sin(runTime)*45-10;					// 오른쪽 다리는 60도까지 움직이되 sin()으로 주기적인 움직임 설정
	L_leg1_ang = -R_leg1_ang-20;						// 왼쪽 다리는 오른쪽 다리 반대로
	
	// 로봇이 달리면서 앞, 뒤로 뒤뚱거리고 몸이 틀어지는 것을 표현
	Waist_ang_x = abs(sin(runTime)*15);
	Waist_ang_y = sin(runTime)*15;


	// 캐릭터가 달리면서 상하로 움직이는 것을 표현
	float i=0;
	i=abs(sin(runTime)*2.0);					// i 변수 값 설정
	glTranslatef(0.0, i-0.5, 0);				// 로봇의 몸체가 y축 방향으로 변수 i만큼 이동
	
	DrawCharacter(xPos, yPos, zPos);
}

/*========== Draw Environment ==========*/
void DrawGround() {
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, MyTextureObject[0]);
	glBegin(GL_QUADS);

	/*
	glTexCoord2f(0.0f,0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); //앞면
	glTexCoord2f(1.0f,0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f,1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f,1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	
	glTexCoord2f(1.0f,0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);//뒷면
	glTexCoord2f(1.0f,1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f,1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f,0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	*/
	glTexCoord2f(0.0f,1.0f); glVertex3f(-300.0f, 0.0f, -150.0f);//윗면
	glTexCoord2f(0.0f,0.0f); glVertex3f(-300.0f, 0.0f, 150.0f);
	glTexCoord2f(1.0f,0.0f); glVertex3f(300.0f,  0.0f, 150.0f);
	glTexCoord2f(1.0f,1.0f); glVertex3f(300.0f,  0.0f, -150.0f);
	
	/*
	glTexCoord2f(1.0f,1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);//아랫면
	glTexCoord2f(0.0f,1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f,0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f,0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);

	
	glTexCoord2f(1.0f,0.0f); glVertex3f(1.0f, -1.0f, -1.0f);//오른쪽 옆면
	glTexCoord2f(1.0f,1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f,1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f,0.0f); glVertex3f(1.0f, -1.0f, 1.0f);

	glTexCoord2f(0.0f,0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);//왼쪽 옆면
	glTexCoord2f(1.0f,0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f,1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f,1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	*/
	glEnd();

		/* 원본
		glColor3f(0.0f, 1.0f, 0.0f);			//R,G,B 색상
		glTranslatef(0.0f, -0.5f, 0.0f);
		glScalef(100.0f, 1.0f, 100.0f);
		glutSolidCube(1);
		*/
	glPopMatrix();
}

 
void textOut(float x, float y, float z, char *string, void *font)
{
	int len, i;
	glColor3f (1.0, 1.0, 1.0);
	glRasterPos3f(x, y , z);
	len = strlen(string);
	for (i = 0; i < len; i++) {
		glutBitmapCharacter(font, string[i]);
	}
}

/*========== Draw Text ==========*/

void DrawText(){
	textOut(character_pos_x-40,45,character_pos_z,"KOR 1 | JAP 0",GLUT_BITMAP_TIMES_ROMAN_24);
	textOut(character_pos_x+35,45,character_pos_z,"SBS",GLUT_BITMAP_TIMES_ROMAN_24);
}


// Display Callback Function
void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);			// 깊이 테스트 활성화

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	// gluLookAt() 함수로 카메라의 시점을 지정 가능 (인자 : 카메라위치 / 보는대상 / 카메라 위방향 벡터)
	// 카메라는 캐릭터를 내려다무본다. (공을 본다로 변경)
	gluLookAt(character_pos_x+0.0, 30.0, character_pos_z+60.0,   character_pos_x, 0.0, character_pos_z,   0.0, 6.0, -3.0);

	DrawGround();						// 지면을 그린다. 텍스처이용한 것으로 수정
	DrawText();							//텍스트 출력
	
	// ** 캐릭터 모델 좌표계 ( 캐릭터의 발 사이 중심이 원점 )
	glTranslatef(character_pos_x, 0.0f, character_pos_z);
	// >> 중력을 줄때 모델좌표계 y값이 0일때를 바닥으로 인식할 수 있음

	glPushMatrix();
		glTranslatef(0.0f, 9.0f, 0.0f);
		switch (ani) {
		case ANI_STAND:
			glRotatef(character_angle, 0.0f, 1.0f, 0.0f);
			DrawStand(0.0f, 0.0f, 0.0f);
			break;
		case ANI_WALK:
			glRotatef(character_angle, 0.0f, 1.0f, 0.0f);
			DrawWalk(0.0f, 0.0f, 0.0f);
			break;
		case ANI_RUN:
			glRotatef(character_angle, 0.0f, 1.0f, 0.0f);
			DrawRun(0.0f, 0.0f, 0.0f);
			break;
		}
	glPopMatrix();
	glutSwapBuffers();
}

// Keyboard Callback Function
void MyKeyboard(unsigned char key, int x, int y) {
	switch(key) {
	case 'e':
		KeyStates[0] = true;
		break;
	default:
		break;
	}
}
void MyKeyRelease (unsigned char key, int x, int y) {
	switch(key) {
	case 'e':
		KeyStates[0] = false;
		break;
	default:
		break;
	}
}
void MySpecialKey(int key, int x, int y) {
	// 방향키가 눌린 경우 1. 캐릭터의 방향을 바꾼다. 2. 캐릭터의 방향변수를 변경한다. 3. 애니메이션을 걷기 모드로 전환한다.
	switch (key) {
	case GLUT_KEY_LEFT:
		SpecialKeyStates[0] = true;
		break;
	case GLUT_KEY_UP:
		SpecialKeyStates[1] = true;
		break;
	case GLUT_KEY_RIGHT:
		SpecialKeyStates[2] = true;
		break;
	case GLUT_KEY_DOWN:
		SpecialKeyStates[3] = true;
		break;
	}
}
void MySpecialRelease(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		SpecialKeyStates[0] = false;
		ani = ANI_STAND;
		break;
	case GLUT_KEY_UP:
		SpecialKeyStates[1] = false;
		ani = ANI_STAND;
		break;
	case GLUT_KEY_RIGHT:
		SpecialKeyStates[2] = false;
		ani = ANI_STAND;
		break;
	case GLUT_KEY_DOWN:
		SpecialKeyStates[3] = false;
		ani = ANI_STAND;
		break;
	default:
		break;
	}
}

// Timer Callback Function
void MyTimer(int value) {
	// 걷는 애니메이션과 뛰는 애니메이션 속도 설정
	walkTime = walkTime + 0.12;
	runTime = runTime + 0.15;
	
	// 방향에 따라 속도를 주어 캐릭터가 움직이도록 함
	if (SpecialKeyStates[0] && SpecialKeyStates[1]) {					// 좌상단 이동
		character_angle = 225.0;
		if (KeyStates[0])
			ani = ANI_RUN;
		else
			ani = ANI_WALK;
		character_pos_x -= speed/sqrt(2.0);
		character_pos_z -= speed/sqrt(2.0);
	}else if (SpecialKeyStates[1] && SpecialKeyStates[2]) {				// 우상단 이동
		character_angle = 135.0;
		if (KeyStates[0])
			ani = ANI_RUN;
		else
			ani = ANI_WALK;
		character_pos_x += speed/sqrt(2.0);
		character_pos_z -= speed/sqrt(2.0);
	}else if (SpecialKeyStates[2] && SpecialKeyStates[3]) {				// 우하단 이동
		character_angle = 45.0;
		if (KeyStates[0])
			ani = ANI_RUN;
		else
			ani = ANI_WALK;
		character_pos_x += speed/sqrt(2.0);
		character_pos_z += speed/sqrt(2.0);
	}else if (SpecialKeyStates[3] && SpecialKeyStates[0]) {				// 좌하단 이동
		character_angle = 315.0;
		if (KeyStates[0])
			ani = ANI_RUN;
		else
			ani = ANI_WALK;
		character_pos_x -= speed/sqrt(2.0);
		character_pos_z += speed/sqrt(2.0);
	}else if (SpecialKeyStates[0]) {					// 좌로이동
		character_angle = 270.0;
		if (KeyStates[0])
			ani = ANI_RUN;
		else
			ani = ANI_WALK;
		character_pos_x -= speed;
	}else if (SpecialKeyStates[1]) {					// 상단이동
		character_angle = 180.0;
		if (KeyStates[0])
			ani = ANI_RUN;
		else
			ani = ANI_WALK;
		character_pos_z -= speed;
	}else if (SpecialKeyStates[2]) {					// 우로이동
		character_angle = 90.0;
		if (KeyStates[0])
			ani = ANI_RUN;
		else
			ani = ANI_WALK;
		character_pos_x += speed;
	}else if (SpecialKeyStates[3])  {					// 하단이동
		character_angle = 0.0;
		if (KeyStates[0])
			ani = ANI_RUN;
		else
			ani = ANI_WALK;
		character_pos_z += speed;
	}

	// 수정되었으니 Invalidate 시키자
	glutPostRedisplay();
	glutTimerFunc(10, MyTimer, 1);
}

// Idle Callback Function
void MyIdle() {
	// Idle 상태일때 ani를 확인하여 speed를 정해줌
	if (ani == ANI_WALK)
		speed = 0.5f;
	else if (ani == ANI_RUN)
		speed = 1.0f;
}

// Resize Callback Function
void MyResize() {


}


AUX_RGBImageRec * LoadBMP(char *szFilename){
	FILE *pFile = NULL;
	if(!szFilename){
		return NULL;
	}
	pFile = fopen(szFilename,"r");
	if(pFile){
		fclose(pFile);
		return auxDIBImageLoad(szFilename);	//파일에서 메모리로
	}
	return NULL;
}

int LoadGLTextures(char *szFilePath){		//파일을 로드하고 텍스쳐로 변환하는 과정
	int Status = FALSE;
	glClearColor(0,0,0,0.5);
	memset(pTextureImage,0,sizeof(void *)*1);//포인터를 널로 
	
	if(pTextureImage[0]=LoadBMP(szFilePath)){
		Status=TRUE;						 //상태 플렉을 TRUE로 변경.
		glGenTextures(1,&MyTextureObject[0]);//텍스쳐 생성
		glBindTexture(GL_TEXTURE_2D, MyTextureObject[0]);
		glTexImage2D(GL_TEXTURE_2D,0,3,pTextureImage[0]->sizeX,pTextureImage[0]->sizeY,0,GL_RGB,GL_UNSIGNED_BYTE,pTextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glEnable(GL_TEXTURE_2D);
	}

	if(pTextureImage[0]){					//텍스처가 존재하면
		if(pTextureImage[0]->data){			//텍스처 영상이 존재하면
			free(pTextureImage[0]->data);	//텍스처 영상 공간 반납
		}
		free(pTextureImage[0]);
	}
	return Status;
}





int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 800);
	glutCreateWindow("OpenGL Soccer Game Sample by HW");
	
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);				// 투영 행렬 선택
	glLoadIdentity();							// 초기화
	// orthograpic 투영 (전면카메라. 모델 확인용 시점)
	// glOrtho(-30.0, 30.0, -30.0, 30.0, -30.0, 60.0);
	// perspective 투영 ( 시야각(FOV), 종횡비, Near, Far ) 
	gluPerspective(90.0, 1.0, 1.0, 200.0);
	// 카메라의 시점은 디스플레이 콜백 함수에서 지정해주겠다. (그릴때마다 위치가 변경되므로)
	

	// Register callback function
	glutDisplayFunc(MyDisplay);
	glutSpecialFunc(MySpecialKey);
	glutSpecialUpFunc(MySpecialRelease);
	glutKeyboardFunc(MyKeyboard);
	glutKeyboardUpFunc(MyKeyRelease);
	glutIdleFunc(MyIdle);
	glutTimerFunc(10, MyTimer, 1);
	if(LoadGLTextures(argv[1])){
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_SMOOTH);
			glClearDepth(1.0);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
			glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
			glutMainLoop();
	}
	glutMainLoop();
	return 0;
}