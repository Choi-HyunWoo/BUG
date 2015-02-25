#include <windows.h>
#include <stdio.h>
#include <gl/glut.h>
#include <gl/gl.h>
#include <gl/GLU.h>
#include <gl/GLAux.h>
#include <math.h>
#include<string.h>
#include<conio.h>//to use itoa(정수를 문자열로)
#include<stdlib.h>


#define MAX_NO_TEXTURES 7			//1개=지표,6개=하늘(직육면체)
GLuint texture[MAX_NO_TEXTURES];
float ratio;
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

//점수 표현을 위한 변수들(골을 넣으면 해당 함수에서 scorep1,혹은 scorep2를 ++시켜주면 됨)
int  scorep1=0;
int  scorep2=0;
char string1[100];
char string2[100];


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
		glColor3f(0.0f, 0.0f, 0.5f);
		glTranslatef(0.0f, -0.5f, 0.0f);
		glScalef(100.0f, 1.0f, 100.0f);
		glutSolidCube(1);
	glPopMatrix();
}

AUX_RGBImageRec *LoadBMP(char *Filename)    // Loads A Bitmap Image
{
 FILE *File=NULL;         // File Handle
 if (!Filename)          // Make Sure A Filename Was Given
 {
  return NULL;         // If Not Return NULL
 }
 File=fopen(Filename,"r");       // Check To See If The File Exists
 if (File)           // Does The File Exist?
 {
  fclose(File);         // Close The Handle
  return auxDIBImageLoad(Filename);    // Load The Bitmap And Return A Pointer
 }
 return NULL;          // If Load Failed Return NULL
}

void init(void)
{
 glShadeModel(GL_SMOOTH);       // Enable Smooth Shading
 glClearColor(0.0f, 0.0f, 0.0f, 0.5f);    // Black Background
 glEnable ( GL_COLOR_MATERIAL );
 glColorMaterial ( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
 glEnable ( GL_TEXTURE_2D );
 glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );
 glGenTextures (1, texture);
 AUX_RGBImageRec *TextureImage[7];					// Create Storage Space For The Texture
 memset(TextureImage,0,sizeof(void *)*1);            // Set The Pointer To NULL
	
 if (TextureImage[0]=LoadBMP("ground.bmp"))//이미지 로딩
 {
  glGenTextures(1, &texture[0]);     //텍스쳐 생성
  //텍스쳐에 이미지 넣기
  glBindTexture(GL_TEXTURE_2D, texture[0]);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
 
 }
 if (TextureImage[0])         // If Texture Exists
 {
  if (TextureImage[0]->data)       // If Texture Image Exists
  {
   free(TextureImage[0]->data);     // Free The Texture Image Memory
  }
  free(TextureImage[0]);        // Free The Image Structure
 }

 if (TextureImage[1]=LoadBMP("sky1.bmp"))//이미지 로딩
 {
  glGenTextures(1, &texture[1]);     //텍스쳐 생성
  //텍스쳐에 이미지 넣기
  glBindTexture(GL_TEXTURE_2D, texture[1]);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[1]->sizeX, TextureImage[1]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[1]->data);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
 }
 if (TextureImage[1])         // If Texture Exists
 {
  if (TextureImage[1]->data)       // If Texture Image Exists
  {
   free(TextureImage[1]->data);     // Free The Texture Image Memory
  }
  free(TextureImage[1]);        // Free The Image Structure
 }

 if (TextureImage[2]=LoadBMP("sky2.bmp"))//이미지 로딩
 {
  glGenTextures(1, &texture[2]);     //텍스쳐 생성
  //텍스쳐에 이미지 넣기
  glBindTexture(GL_TEXTURE_2D, texture[2]);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[2]->sizeX, TextureImage[2]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[2]->data);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
 }
 if (TextureImage[2])         // If Texture Exists
 {
  if (TextureImage[2]->data)       // If Texture Image Exists
  {
   free(TextureImage[2]->data);     // Free The Texture Image Memory
  }
  free(TextureImage[2]);        // Free The Image Structure
 }

  if (TextureImage[3]=LoadBMP("sky3.bmp"))//이미지 로딩
 {
  glGenTextures(1, &texture[3]);     //텍스쳐 생성
  //텍스쳐에 이미지 넣기
  glBindTexture(GL_TEXTURE_2D, texture[3]);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[3]->sizeX, TextureImage[3]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[3]->data);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
 }
 if (TextureImage[3])         // If Texture Exists
 {
  if (TextureImage[3]->data)       // If Texture Image Exists
  {
   free(TextureImage[3]->data);     // Free The Texture Image Memory
  }
  free(TextureImage[3]);        // Free The Image Structure
 }

  if (TextureImage[4]=LoadBMP("sky4.bmp"))//이미지 로딩
 {
  glGenTextures(1, &texture[4]);     //텍스쳐 생성
  //텍스쳐에 이미지 넣기
  glBindTexture(GL_TEXTURE_2D, texture[4]);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[4]->sizeX, TextureImage[4]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[4]->data);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
 }
 if (TextureImage[4])         // If Texture Exists
 {
  if (TextureImage[4]->data)       // If Texture Image Exists
  {
   free(TextureImage[4]->data);     // Free The Texture Image Memory
  }
  free(TextureImage[4]);        // Free The Image Structure
 }

 if (TextureImage[5]=LoadBMP("sky5.bmp"))//이미지 로딩
 {
  glGenTextures(1, &texture[5]);     //텍스쳐 생성
  //텍스쳐에 이미지 넣기
  glBindTexture(GL_TEXTURE_2D, texture[5]);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[5]->sizeX, TextureImage[5]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[5]->data);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
 }
 if (TextureImage[5])         // If Texture Exists
 {
  if (TextureImage[5]->data)       // If Texture Image Exists
  {
   free(TextureImage[5]->data);     // Free The Texture Image Memory
  }
  free(TextureImage[5]);        // Free The Image Structure
 }


 glEnable ( GL_CULL_FACE ); 
}

void DrawTexture()
{
// glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
// glLoadIdentity ( );
 
  //텍스쳐 출력
 glEnable(GL_TEXTURE_2D);//gl의 텍스처 기능 활성화 ,반대로는 glDisalbe사용
 glPushMatrix();
 glDisable(GL_DEPTH_TEST);
 glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);//GL_DECAL은 물체면 위에 텍스처 색을 덧칠하는 효과를 주기 위함이다. p.613참고
 glColor3f(0,0,0);




 glBindTexture ( GL_TEXTURE_2D, texture[1] );//앞
 glBegin ( GL_QUADS );
    glTexCoord2f(0.0f,0.0f); glVertex3f(-300.0f, 0.0f, -150.0f); 
	glTexCoord2f(1.0f,0.0f); glVertex3f(300.0f, 0.0f, -150.0f);
	glTexCoord2f(1.0f,1.0f); glVertex3f(300.0f, 100.0f, -150.0f);
	glTexCoord2f(0.0f,1.0f); glVertex3f(-300.0f, 100.0f, -150.0f);
  glEnd();
  
  /*
 glBindTexture ( GL_TEXTURE_2D, texture[2] );//뒤
 glBegin ( GL_QUADS );
	glTexCoord2f(0.0f,0.0f); glVertex3f(300.0f, 0.0f, 150.0f);
	glTexCoord2f(1.0f,0.0f); glVertex3f(-300.0f, 0.0f, 150.0f);
	glTexCoord2f(1.0f,1.0f); glVertex3f(-300.0f, 100.0f,150.0f);
	glTexCoord2f(0.0f,1.0f); glVertex3f(300.0f, 100.0f, 150.0f);
  glEnd();
  */
  glBindTexture ( GL_TEXTURE_2D, texture[3] );//좌
 glBegin ( GL_QUADS );
	glTexCoord2f(0.0f,0.0f); glVertex3f(300.0f, 0.0f, -150.0f);
	glTexCoord2f(1.0f,0.0f); glVertex3f(300.0f, 0.0f, 150.0f);
	glTexCoord2f(1.0f,1.0f); glVertex3f(300.0f, 100.0f, 150.0f);
	glTexCoord2f(0.0f,1.0f); glVertex3f(300.0f, 100.0f, -150.0f);
  glEnd();

  glBindTexture ( GL_TEXTURE_2D, texture[4] );//우
 glBegin ( GL_QUADS );
    glTexCoord2f(0.0f,0.0f); glVertex3f(-300.0f, 0.0f, 150.0f); 
	glTexCoord2f(1.0f,0.0f); glVertex3f(-300.0f, 0.0f, -150.0f);
	glTexCoord2f(1.0f,1.0f); glVertex3f(-300.0f, 100.0f, -150.0f);
	glTexCoord2f(0.0f,1.0f); glVertex3f(-300.0f, 100.0f, 150.0f);
  glEnd();

 glBindTexture ( GL_TEXTURE_2D, texture[5] );//상
 glBegin ( GL_QUADS );
	glTexCoord2f(0.0f,0.0f); glVertex3f(-300.0f, 100.0f, -150.0f);
	glTexCoord2f(1.0f,0.0f); glVertex3f(300.0f, 100.0f, -150.0f);
	glTexCoord2f(1.0f,1.0f); glVertex3f(300.0f, 100.0f,150.0f);
	glTexCoord2f(0.0f,1.0f); glVertex3f(-300.0f, 100.0f, 150.0f);
  glEnd();
   glBindTexture ( GL_TEXTURE_2D, texture[0] );
 glBegin ( GL_QUADS );

	glTexCoord2f(0.0f,1.0f); glVertex3f(-300.0f, 0.0f, -150.0f);//아래
	glTexCoord2f(0.0f,0.0f); glVertex3f(-300.0f, 0.0f, 150.0f);
	glTexCoord2f(1.0f,0.0f); glVertex3f(300.0f,  0.0f, 150.0f);
	glTexCoord2f(1.0f,1.0f); glVertex3f(300.0f,  0.0f, -150.0f);
	
 glEnd();
 
 

  glDepthMask(GL_TRUE);
  glEnable(GL_DEPTH_TEST);
  glPopMatrix();

  glDisable(GL_TEXTURE_2D);

  //glutSwapBuffers();
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
	
	//scorep(int형)을 string(string형)으로 10진수 형태로 변경해 주겠다.
	itoa(scorep1,string1,10);
	itoa(scorep2,string2,10);
	char stringp1[10]="KOR : ";
	char stringp2[10]="JAP : ";
	
	char *result1=strcat(stringp1,string1);//KOR : 1 이런식으로 증가된 점수를 붙여주기 위함
	char *result2=strcat(stringp2,string2);//JAP : 1 이런식으로 증가된 점수를 붙여주기 위함

	textOut(character_pos_x-40,45,character_pos_z,result1,GLUT_BITMAP_TIMES_ROMAN_24);
	textOut(character_pos_x-41,40,character_pos_z,result2,GLUT_BITMAP_TIMES_ROMAN_24);
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
	//gluLookAt(character_pos_x-30, 40.0, character_pos_z+0.0,   character_pos_x, 0.0, character_pos_z,   -30.0, 50.0, 0.0);
	
	//DrawGround();						// 지면을 그린다.
	DrawTexture();
	DrawText();							//텍스트 출력.

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
	
	init();
	// Register callback function
	glutDisplayFunc(MyDisplay);
	glutSpecialFunc(MySpecialKey);
	glutSpecialUpFunc(MySpecialRelease);
	glutKeyboardFunc(MyKeyboard);
	glutKeyboardUpFunc(MyKeyRelease);
	glutIdleFunc(MyIdle);
	glutTimerFunc(10, MyTimer, 1);

	glutMainLoop();
	return 0;
}