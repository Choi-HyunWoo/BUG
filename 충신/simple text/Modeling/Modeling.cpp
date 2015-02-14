#include <gl/glut.h>
#include <gl/gl.h>
#include <gl/GLU.h>
#include <gl/GLAux.h>
#include <math.h>


/* Animation list.
 0. stand
 1. walk
 2. run (질주)
 3. shoot
 4. pass
*/
// define animation status
#define ANI_STAND	0
#define ANI_WALK	1
#define ANI_RUN		2
#define ANI_SHOOT	3
#define ANI_PASS	4

// animation status
static int ani = ANI_STAND;

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

//허리 각도
GLfloat waist_ang = 0.0f;
// 목 각도
GLfloat neck_ang = 0.0f;


// 캐릭터 방향
GLfloat character_dir= 0.0f;

// time
static double walkTime = 0.0;
static double runTime = 0.0;



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
	
	DrawCharacter(0.0f, 0.0f, 0.0f);
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
	glRotatef(abs(sin(walkTime)*5), 1, 0, 0);	// x축을 기준으로 16도까지 각도가 틀어짐
	glRotatef(sin(walkTime)*5, 0, 1, 0);		// y축을 기준으로 16도까지 각도가 틀어짐 sin() 함수로 주기적인 움직임 설정

	// 캐릭터가 달리면서 상하로 움직이는 것을 표현
	float i=0;
	i=abs(sin(walkTime)*0.3);					// i 변수 값 설정
	glTranslatef(0.0, i-0.2, 0);				// 로봇의 몸체가 y축 방향으로 변수 i만큼 이동
	
	DrawCharacter(0.0f, 0.0f, 0.0f);
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
	glRotatef(abs(sin(runTime)*15), 1, 0, 0);	// x축을 기준으로 16도까지 각도가 틀어짐
	glRotatef(sin(runTime)*15, 0, 1, 0);		// y축을 기준으로 16도까지 각도가 틀어짐 sin() 함수로 주기적인 움직임 설정

	// 캐릭터가 달리면서 상하로 움직이는 것을 표현
	float i=0;
	i=abs(sin(runTime)*1.0);					// i 변수 값 설정
	glTranslatef(0.0, i-0.5, 0);				// 로봇의 몸체가 y축 방향으로 변수 i만큼 이동
	
	DrawCharacter(0.0f, 0.0f, 0.0f);
}

/*========== Draw Environment ==========*/
void DrawGround() {
	glPushMatrix();
		glColor3f(0.0f, 0.8f, 0.0f);
		glTranslatef(0.0f, -9.5f, 0.0f);
		glScalef(20.0f, 1.0f, 20.0f);
		glutSolidCube(1);
	glPopMatrix();
}

GLubyte *LoadBmp(const char *Path, int *Width, int *Height)
{
     HANDLE hFile;
     DWORD FileSize, dwRead;
     BITMAPFILEHEADER *fh=NULL;
     BITMAPINFOHEADER *ih;
     BYTE *pRaster;
 
     hFile=CreateFileA(Path,GENERIC_READ,0,NULL,
          OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
     if (hFile==INVALID_HANDLE_VALUE) {
          return NULL;
     }
 
     FileSize=GetFileSize(hFile,NULL);
     fh=(BITMAPFILEHEADER *)malloc(FileSize);
     ReadFile(hFile,fh,FileSize,&dwRead,NULL);
     CloseHandle(hFile);
 
     int len = FileSize - fh->bfOffBits;
     pRaster=(GLubyte *)malloc(len);
     memcpy(pRaster, (BYTE *)fh+fh->bfOffBits, len);
 
     // RGB로 순서를 바꾼다.
     for (BYTE *p=pRaster;p < pRaster + len - 3;p+=3) {
          BYTE b = *p;
          *p = *(p+2);
          *(p+2) = b;
     }
 
     ih=(BITMAPINFOHEADER *)((PBYTE)fh+sizeof(BITMAPFILEHEADER));
     *Width=ih->biWidth;
     *Height=ih->biHeight;
 
     free(fh);
     return pRaster;
}
 
void DoDisplay()
{
     GLubyte *data;
     int Width, Height;
 
     glClear(GL_COLOR_BUFFER_BIT);
 
     data = LoadBmp("chestnut.bmp", &Width, &Height);
     if (data != NULL) {
          glRasterPos2f(-0.5,-0.5);
          glDrawPixels(Width, Height, GL_RGB, GL_UNSIGNED_BYTE, data);
          free(data);
     }
 
     glFlush();
}


void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	DrawGround();
	glRotatef(character_dir, 0.0f, 1.0f, 0.0f);
	
	switch (ani) {
	case ANI_STAND:
		DrawStand(0.0f, 0.0f, 0.0f);
		break;
	case ANI_WALK:
		DrawWalk(0.0f, 0.0f, 0.0f);
		break;
	case ANI_RUN:
		DrawRun(0.0f, 0.0f, 0.0f);
		break;
	}
	glutSwapBuffers();
}
void MyKeyboard(unsigned char key, int x, int y) {
	switch(key) {
	case 's':
		ani=ANI_STAND;
		break;
	case 'e':
		ani=ANI_RUN;
		break;
	}
}
void MySpecialKey(int key, int x, int y) { 
	switch(key) {
	case GLUT_KEY_UP:
		character_dir = 180.0;
		ani = ANI_WALK;
		break;
	case GLUT_KEY_DOWN:
		character_dir = 0.0;
		ani = ANI_WALK;
		break;
	case GLUT_KEY_RIGHT:
		character_dir = 90.0;
		ani = ANI_WALK;
		break;
	case GLUT_KEY_LEFT:
		character_dir = 270.0;
		ani = ANI_WALK;
		break;
	default:
		break;
	}
}

void MyTimer(int value) {
	/*
	camera_angle += 1.0f;
	if (camera_angle >= 360.0f)
		camera_angle = 0.0f;
	*/
	walkTime = walkTime + 0.1;
	runTime = runTime + 0.12;

	glutPostRedisplay();
	glutTimerFunc(10, MyTimer, 1);
}



int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Character Modeling by HW");
	
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-20.0, 20.0, -20.0, 20.0, -20.0, 50.0);

	glutDisplayFunc(MyDisplay);
	glutSpecialFunc(MySpecialKey);
	glutKeyboardFunc(MyKeyboard);
	//glutIdleFunc(MyIdle);
	glutTimerFunc(10, MyTimer, 1);

	glutMainLoop();
	return 0;
}