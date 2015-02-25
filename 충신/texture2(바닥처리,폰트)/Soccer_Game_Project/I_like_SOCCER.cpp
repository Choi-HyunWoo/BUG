#include <windows.h>
#include <stdio.h>
#include <gl/glut.h>
#include <gl/gl.h>
#include <gl/GLU.h>
#include <gl/GLAux.h>
#include <math.h>
#include<string.h>
#include<conio.h>//to use itoa(������ ���ڿ���)
#include<stdlib.h>


#define MAX_NO_TEXTURES 7			//1��=��ǥ,6��=�ϴ�(������ü)
GLuint texture[MAX_NO_TEXTURES];
float ratio;
/* Animation list.
 0. stand
 1. walk
 2. run (����)
 3. shoot
 4. pass
*/

// define animation status
#define ANI_STAND	0				// ���ִ� ���
#define ANI_WALK	1				// �ȴ� ���
#define ANI_RUN		2				// �ٴ� ���
#define ANI_SHOOT	3				// �� ���
#define ANI_PASS	4				// �н� ���

// animation status
static int ani = ANI_STAND;			// �ִϸ��̼� �Ǻ� ���� (���ִ� �ڼ��� �⺻)

// Ű�� ���� ���¸� ����
bool SpecialKeyStates[4] = {false, false, false, false};		// 0:LEFT	1:UP	2:RIGHT		3:DOWN
bool KeyStates[2] = { false, false };							// 0:'E'	1:

//���� ǥ���� ���� ������(���� ������ �ش� �Լ����� scorep1,Ȥ�� scorep2�� ++�����ָ� ��)
int  scorep1=0;
int  scorep2=0;
char string1[100];
char string2[100];


// angle
// �� ����
GLfloat L_arm1_ang = 0.0f;			// arm1 : ��� ����
GLfloat L_arm2_ang = 0.0f;			// arm2 : �Ȳ�ġ ����
GLfloat R_arm1_ang = 0.0f;
GLfloat R_arm2_ang = 0.0f;
// �ٸ� ����
GLfloat L_leg1_ang = 0.0f;			// leg1 : ��� ����
GLfloat L_leg2_ang = 0.0f;			// leg2 : ���� ����
GLfloat R_leg1_ang = 0.0f;
GLfloat R_leg2_ang = 0.0f;
// �㸮 ����
GLfloat Waist_ang_x = 0.0f;
GLfloat Waist_ang_y = 0.0f;
// �� ����
GLfloat Neck_ang = 0.0f;

// Charater direction
GLfloat character_angle= 0.0f;

// Character Position
GLfloat character_pos_x = 0.0f;
GLfloat character_pos_y = 0.0f;			// �߷� ������� 
GLfloat character_pos_z = 0.0f;

// Character Speed
GLfloat speed = 0.0f;

// Animation time (speed)
static double walkTime = 0.0;
static double runTime = 0.0;

/*========== Physics Simulation ==========*/
GLfloat dt;							// �ð� ��ȭ��
const GLfloat acc = -9.8f;			// �߷°��ӵ�
GLfloat vel = 0.0f;					// vel += acc*dt
GLfloat pos = 0.0f;					// pos += vel*dt  , draw object at pos


/*========== Modeling Objects ==========*/
void DrawHead(float xPos, float yPos, float zPos) {
	glPushMatrix();			// ��� ���ÿ� ���� ����� ����
		glColor3f(0.5f, 0.5f, 0.5f);
		glTranslatef(xPos, yPos, zPos);
		glScalef(2.0f, 2.0f, 1.0f);					// �Ӹ�ũ�� 2x2x2
		glutSolidCube(1);
	glPopMatrix();
}
void DrawTorso(float xPos, float yPos, float zPos) {
	glPushMatrix();			// ��� ���ÿ� ���� ����� ����
		glColor3f(0.5f, 0.5f, 0.5f);
		glTranslatef(xPos, yPos, zPos);
		glScalef(4.0f, 4.0f, 1.0f);					// ����ũ�� 3x4x1
		glutSolidCube(1);
	glPopMatrix();
}
void DrawPelvis(float xPos, float yPos, float zPos) {
	glPushMatrix();			// ��� ���ÿ� ���� ����� ����
		glColor3f(0.0f, 0.0f, 1.0f);
		glTranslatef(xPos, yPos, zPos);
		glScalef(1.0f, 1.0f, 1.0f);					// ����ũ�� 3x4x1
		glutSolidCube(1);
	glPopMatrix();
}
void DrawArm(float xPos, float yPos, float zPos) {
	glPushMatrix();			// ��� ���ÿ� ���� ����� ����
		glColor3f(0.5f, 0.5f, 0.5f);
		glTranslatef(xPos, yPos, zPos);
		glScalef(1.0f, 3.0f, 1.0f);
		glutSolidCube(1);
	glPopMatrix();
}
void DrawLeg1(float xPos, float yPos, float zPos) {
	glPushMatrix();			// ��� ���ÿ� ���� ����� ����
		glColor3f(0.5f, 0.5f, 0.5f);
		glTranslatef(xPos, yPos, zPos);
		glScalef(1.2f, 3.0f, 1.2f);
		glutSolidCube(1);
	glPopMatrix();
}
void DrawLeg2(float xPos, float yPos, float zPos) {
	glPushMatrix();			// ��� ���ÿ� ���� ����� ����
		glColor3f(0.5f, 0.5f, 0.5f);
		glTranslatef(xPos, yPos, zPos);
		glScalef(1.0f, 4.0f, 1.0f);
		glutSolidCube(1);
	glPopMatrix();
}
void DrawFoot (float xPos, float yPos, float zPos) {
	glPushMatrix();			// ��� ���ÿ� ���� ����� ����
		glColor3f(0.5f, 0.5f, 0.5f);
		glTranslatef(xPos, yPos, zPos);
		glScalef(1.0f, 1.0f, 2.0f);
		glutSolidCube(1);
	glPopMatrix();
}
// ����
void DrawJoint() {
	glPushMatrix();
		glColor3f(1.0f, 0.0f, 0.0f);
		glutSolidSphere(0.5, 10, 10);
	glPopMatrix();
}

/*========== Draw Character ==========*/
void DrawCharacter(float xPos, float yPos, float zPos) {
	glTranslatef(xPos, yPos, zPos);			// ��ǥ���̵� : ĳ������ ��ġ (����)
	DrawPelvis(0.0f, 0.0f, 0.0f);			// ������ ����� �׸�

	glPushMatrix();							// ��� ��ǥ ���
		glTranslatef(0.0f, 1.0f, 0.0f);		// �㸮 ���� ��ġ ���
		glRotatef(Waist_ang_x, 1.0f, 0.0f, 0.0f);
		glRotatef(Waist_ang_y, 0.0f, 1.0f, 0.0f);
		DrawJoint();						// �㸮 ���� ����
		glTranslatef(0.0f, 2.0f, 0.0f);
		glPushMatrix();						// ���� �߽���ǥ ���
			DrawTorso(0.0f, 0.0f, 0.0f);			// ���� �׸���
			
			glTranslatef(0.0f, 2.5f, 0.0f);			// �� ��ǥ��
			DrawJoint();							// �� �׸���
			DrawHead(0.0f, 1.0f, 0.0f);				// �Ӹ��� �׸�
		glPopMatrix();
	glPopMatrix();
	
	// ���� �׸���
	glPushMatrix();							// �� �߽� ��ǥ ����
		glTranslatef(2.5f, 4.5f, 0.0f);			// ��ǥ���̵� : ���ʾ��
		glRotatef(L_arm1_ang, 1.0f, 0.0f, 0.0f);
		DrawJoint();
		glRotatef(5, 0.0f, 0.0f, 1.0f);		// ����� 10�� �����
		DrawArm(0.0f, -1.5f, 0.0f);				// ������� 1.5 ������ġ�� ������ �׸�
		glPushMatrix();							// �����ǥ�� ����
			glTranslatef(0.0, -3.5f, 0.0f);			// ��ǥ���̵� : �����Ȳ�ġ
			glRotatef(L_arm2_ang, 1.0f, 0.0f, 0.0f);
			DrawJoint();
			glRotatef(-15, 1.0f, 0.0f, 0.0f);
			DrawArm(0.0f, -1.5f, 0.0f);
			// ���� �׸��ٸ� �̰���
		glPopMatrix();
	glPopMatrix();
	// ������ �׸���
	glPushMatrix();							// �� �߽� ��ǥ ����
		glTranslatef(-2.5f, 4.5f, 0.0f);			// ��ǥ���̵� : �����ʾ��
		glRotatef(R_arm1_ang, 1.0f, 0.0f, 0.0f);
		DrawJoint();
		glRotatef(-5, 0.0f, 0.0f, 1.0f);		// ����� 10�� �����
		DrawArm(0.0f, -1.5f, 0.0f);				// ������� 1.5 ������ġ�� ������ �׸�
		glPushMatrix();							// �����ǥ�� ����
			glTranslatef(0.0, -3.5f, 0.0f); 		// ��ǥ���̵� : �������Ȳ�ġ
			glRotatef(R_arm2_ang, 1.0f, 0.0f, 0.0f);
			DrawJoint();
			glRotatef(-15, 1.0f, 0.0f, 0.0f);
			DrawArm(0.0f, -1.5f, 0.0f);
			// ���� �׸��ٸ� �̰���
		glPopMatrix();
	glPopMatrix();

	// �޴ٸ� �׸���
	glPushMatrix();							// �� �߽� ��ǥ ����
		glTranslatef(1.0f, 0.0f, 0.0f);		// ��ǥ���̵� : ���� ���
		glRotatef(L_leg1_ang, 1.0f, 0.0f, 0.0f);
		DrawJoint();
		DrawLeg1(0.0f, -1.5f, 0.0f);			// ��ݺ��� 2 ������ġ�� ������� �׸�
		glPushMatrix();							// ��� ��ǥ�� ����
			glTranslatef(0.0, -3.5f, 0.0f);		// ��ǥ���̵� : ���ʹ���
			glRotatef(L_leg2_ang, 1.0f, 0.0f, 0.0f);
			DrawJoint();								// ���ʹ����� �׸���
			DrawLeg2(0.0f, -2.0f, 0.0f);				// ���ʹ����Ʒ��� �׸���
			glPushMatrix();								// ���� ��ǥ�� ����
				glTranslatef(0.0, -4.5f, 0.0f);				// ��ǥ���̵� : ���ʹ߸�
				DrawJoint();								// �߸������ �׸���
				DrawFoot(0.0f, -0.5f, 0.5f);				// ���� �׸���.
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
	// �����ٸ� �׸���
	glPushMatrix();							// �� �߽� ��ǥ ����
		glTranslatef(-1.0f, 0.0f, 0.0f);		// ��ǥ���̵� : ������ ���
		glRotatef(R_leg1_ang, 1.0f, 0.0f, 0.0f);
		DrawJoint();
		DrawLeg1(0.0f, -1.5f, 0.0f);			// ��ݺ��� 2 ������ġ�� ������� �׸�
		glPushMatrix();							// ��� ��ǥ�� ����
			glTranslatef(0.0, -3.5f, 0.0f);		// ��ǥ���̵� : �����ʹ���
			glRotatef(R_leg2_ang, 1.0f, 0.0f, 0.0f);
			DrawJoint();								// �����ʹ����� �׸���
			DrawLeg2(0.0f, -2.0f, 0.0f);				// �����ʹ����Ʒ��� �׸���
			glPushMatrix();								// ���� ��ǥ�� ����
				glTranslatef(0.0, -4.5f, 0.0f);				// ��ǥ���̵� : �����ʹ߸�
				DrawJoint();								// �߸������ �׸���
				DrawFoot(0.0f, -0.5f, 0.5f);				// ���� �׸���.
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
	
	L_arm1_ang = sin(walkTime)*60;				// ������ 60������ �����̵� sin() ���� �ֱ����� ������ ����
	R_arm1_ang = -L_arm1_ang;					// �������� ���� �ݴ� ���� 80�� ��������
	R_arm2_ang = -abs(sin(walkTime)*30+60);		// ������ ���� ���� (������ �����ν� ���� �ڷ� ������ ����)
	L_arm2_ang = -abs(-sin(walkTime)*30+60);		// ���� ���� ����
	
	R_leg2_ang = abs(-sin(walkTime)*30-30);			// ������ ���Ƹ� ���� ����  
	L_leg2_ang = abs(sin(walkTime)*30-30);			// ���� ���Ƹ� ���� ����
	R_leg1_ang = sin(walkTime)*30;					// ������ �ٸ��� 30������ �����̵� sin()���� �ֱ����� ������ ����
	L_leg1_ang = -R_leg1_ang;						// ���� �ٸ��� ������ �ٸ� �ݴ��
	
	// ĳ���Ͱ� �޸��鼭 ��, �ڷ� �ڶװŸ��� ���� Ʋ������ ���� ǥ��
	Waist_ang_x = abs(sin(walkTime)*5);
	Waist_ang_y = sin(walkTime)*5;


	// ĳ���Ͱ� �޸��鼭 ���Ϸ� �����̴� ���� ǥ��
	float i=0;
	i=abs(sin(walkTime)*1.0);					// i ���� �� ����
	glTranslatef(0.0, i-0.5, 0);				// �κ��� ��ü�� y�� �������� ���� i��ŭ �̵�
	
	DrawCharacter(xPos, yPos, zPos);
}
void DrawRun (float xPos, float yPos, float zPos) {

	L_arm1_ang = sin(runTime)*80;				// ������ 80������ �����̵� sin() ���� �ֱ����� ������ ����
	R_arm1_ang = -L_arm1_ang;					// �������� ���� �ݴ� ���� 80�� ��������
	R_arm2_ang = -abs(sin(runTime)*60+50);		// ������ ���� ���� (������ �����ν� ���� �ڷ� ������ ����)
	L_arm2_ang = -abs(-sin(runTime)*60+50);		// ���� ���� ����
	
	R_leg2_ang = abs(-sin(runTime)*30-30);			// ������ ���Ƹ� ���� ����  
	L_leg2_ang = abs(sin(runTime)*30-30);			// ���� ���Ƹ� ���� ����
	R_leg1_ang = sin(runTime)*45-10;					// ������ �ٸ��� 60������ �����̵� sin()���� �ֱ����� ������ ����
	L_leg1_ang = -R_leg1_ang-20;						// ���� �ٸ��� ������ �ٸ� �ݴ��
	
	// �κ��� �޸��鼭 ��, �ڷ� �ڶװŸ��� ���� Ʋ������ ���� ǥ��
	Waist_ang_x = abs(sin(runTime)*15);
	Waist_ang_y = sin(runTime)*15;


	// ĳ���Ͱ� �޸��鼭 ���Ϸ� �����̴� ���� ǥ��
	float i=0;
	i=abs(sin(runTime)*2.0);					// i ���� �� ����
	glTranslatef(0.0, i-0.5, 0);				// �κ��� ��ü�� y�� �������� ���� i��ŭ �̵�
	
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
	
 if (TextureImage[0]=LoadBMP("ground.bmp"))//�̹��� �ε�
 {
  glGenTextures(1, &texture[0]);     //�ؽ��� ����
  //�ؽ��Ŀ� �̹��� �ֱ�
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

 if (TextureImage[1]=LoadBMP("sky1.bmp"))//�̹��� �ε�
 {
  glGenTextures(1, &texture[1]);     //�ؽ��� ����
  //�ؽ��Ŀ� �̹��� �ֱ�
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

 if (TextureImage[2]=LoadBMP("sky2.bmp"))//�̹��� �ε�
 {
  glGenTextures(1, &texture[2]);     //�ؽ��� ����
  //�ؽ��Ŀ� �̹��� �ֱ�
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

  if (TextureImage[3]=LoadBMP("sky3.bmp"))//�̹��� �ε�
 {
  glGenTextures(1, &texture[3]);     //�ؽ��� ����
  //�ؽ��Ŀ� �̹��� �ֱ�
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

  if (TextureImage[4]=LoadBMP("sky4.bmp"))//�̹��� �ε�
 {
  glGenTextures(1, &texture[4]);     //�ؽ��� ����
  //�ؽ��Ŀ� �̹��� �ֱ�
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

 if (TextureImage[5]=LoadBMP("sky5.bmp"))//�̹��� �ε�
 {
  glGenTextures(1, &texture[5]);     //�ؽ��� ����
  //�ؽ��Ŀ� �̹��� �ֱ�
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
 
  //�ؽ��� ���
 glEnable(GL_TEXTURE_2D);//gl�� �ؽ�ó ��� Ȱ��ȭ ,�ݴ�δ� glDisalbe���
 glPushMatrix();
 glDisable(GL_DEPTH_TEST);
 glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);//GL_DECAL�� ��ü�� ���� �ؽ�ó ���� ��ĥ�ϴ� ȿ���� �ֱ� �����̴�. p.613����
 glColor3f(0,0,0);




 glBindTexture ( GL_TEXTURE_2D, texture[1] );//��
 glBegin ( GL_QUADS );
    glTexCoord2f(0.0f,0.0f); glVertex3f(-300.0f, 0.0f, -150.0f); 
	glTexCoord2f(1.0f,0.0f); glVertex3f(300.0f, 0.0f, -150.0f);
	glTexCoord2f(1.0f,1.0f); glVertex3f(300.0f, 100.0f, -150.0f);
	glTexCoord2f(0.0f,1.0f); glVertex3f(-300.0f, 100.0f, -150.0f);
  glEnd();
  
  /*
 glBindTexture ( GL_TEXTURE_2D, texture[2] );//��
 glBegin ( GL_QUADS );
	glTexCoord2f(0.0f,0.0f); glVertex3f(300.0f, 0.0f, 150.0f);
	glTexCoord2f(1.0f,0.0f); glVertex3f(-300.0f, 0.0f, 150.0f);
	glTexCoord2f(1.0f,1.0f); glVertex3f(-300.0f, 100.0f,150.0f);
	glTexCoord2f(0.0f,1.0f); glVertex3f(300.0f, 100.0f, 150.0f);
  glEnd();
  */
  glBindTexture ( GL_TEXTURE_2D, texture[3] );//��
 glBegin ( GL_QUADS );
	glTexCoord2f(0.0f,0.0f); glVertex3f(300.0f, 0.0f, -150.0f);
	glTexCoord2f(1.0f,0.0f); glVertex3f(300.0f, 0.0f, 150.0f);
	glTexCoord2f(1.0f,1.0f); glVertex3f(300.0f, 100.0f, 150.0f);
	glTexCoord2f(0.0f,1.0f); glVertex3f(300.0f, 100.0f, -150.0f);
  glEnd();

  glBindTexture ( GL_TEXTURE_2D, texture[4] );//��
 glBegin ( GL_QUADS );
    glTexCoord2f(0.0f,0.0f); glVertex3f(-300.0f, 0.0f, 150.0f); 
	glTexCoord2f(1.0f,0.0f); glVertex3f(-300.0f, 0.0f, -150.0f);
	glTexCoord2f(1.0f,1.0f); glVertex3f(-300.0f, 100.0f, -150.0f);
	glTexCoord2f(0.0f,1.0f); glVertex3f(-300.0f, 100.0f, 150.0f);
  glEnd();

 glBindTexture ( GL_TEXTURE_2D, texture[5] );//��
 glBegin ( GL_QUADS );
	glTexCoord2f(0.0f,0.0f); glVertex3f(-300.0f, 100.0f, -150.0f);
	glTexCoord2f(1.0f,0.0f); glVertex3f(300.0f, 100.0f, -150.0f);
	glTexCoord2f(1.0f,1.0f); glVertex3f(300.0f, 100.0f,150.0f);
	glTexCoord2f(0.0f,1.0f); glVertex3f(-300.0f, 100.0f, 150.0f);
  glEnd();
   glBindTexture ( GL_TEXTURE_2D, texture[0] );
 glBegin ( GL_QUADS );

	glTexCoord2f(0.0f,1.0f); glVertex3f(-300.0f, 0.0f, -150.0f);//�Ʒ�
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
	
	//scorep(int��)�� string(string��)���� 10���� ���·� ������ �ְڴ�.
	itoa(scorep1,string1,10);
	itoa(scorep2,string2,10);
	char stringp1[10]="KOR : ";
	char stringp2[10]="JAP : ";
	
	char *result1=strcat(stringp1,string1);//KOR : 1 �̷������� ������ ������ �ٿ��ֱ� ����
	char *result2=strcat(stringp2,string2);//JAP : 1 �̷������� ������ ������ �ٿ��ֱ� ����

	textOut(character_pos_x-40,45,character_pos_z,result1,GLUT_BITMAP_TIMES_ROMAN_24);
	textOut(character_pos_x-41,40,character_pos_z,result2,GLUT_BITMAP_TIMES_ROMAN_24);
	textOut(character_pos_x+35,45,character_pos_z,"SBS",GLUT_BITMAP_TIMES_ROMAN_24);
}



// Display Callback Function
void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);			// ���� �׽�Ʈ Ȱ��ȭ

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	// gluLookAt() �Լ��� ī�޶��� ������ ���� ���� (���� : ī�޶���ġ / ���´�� / ī�޶� ������ ����)
	// ī�޶�� ĳ���͸� �����ٹ�����. (���� ���ٷ� ����)
	gluLookAt(character_pos_x+0.0, 30.0, character_pos_z+60.0,   character_pos_x, 0.0, character_pos_z,   0.0, 6.0, -3.0);
	//gluLookAt(character_pos_x-30, 40.0, character_pos_z+0.0,   character_pos_x, 0.0, character_pos_z,   -30.0, 50.0, 0.0);
	
	//DrawGround();						// ������ �׸���.
	DrawTexture();
	DrawText();							//�ؽ�Ʈ ���.

	// ** ĳ���� �� ��ǥ�� ( ĳ������ �� ���� �߽��� ���� )
	glTranslatef(character_pos_x, 0.0f, character_pos_z);
	// >> �߷��� �ٶ� ����ǥ�� y���� 0�϶��� �ٴ����� �ν��� �� ����

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
	// ����Ű�� ���� ��� 1. ĳ������ ������ �ٲ۴�. 2. ĳ������ ���⺯���� �����Ѵ�. 3. �ִϸ��̼��� �ȱ� ���� ��ȯ�Ѵ�.
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
	// �ȴ� �ִϸ��̼ǰ� �ٴ� �ִϸ��̼� �ӵ� ����
	walkTime = walkTime + 0.12;
	runTime = runTime + 0.15;
	
	// ���⿡ ���� �ӵ��� �־� ĳ���Ͱ� �����̵��� ��
	if (SpecialKeyStates[0] && SpecialKeyStates[1]) {					// �»�� �̵�
		character_angle = 225.0;
		if (KeyStates[0])
			ani = ANI_RUN;
		else
			ani = ANI_WALK;
		character_pos_x -= speed/sqrt(2.0);
		character_pos_z -= speed/sqrt(2.0);
	}else if (SpecialKeyStates[1] && SpecialKeyStates[2]) {				// ���� �̵�
		character_angle = 135.0;
		if (KeyStates[0])
			ani = ANI_RUN;
		else
			ani = ANI_WALK;
		character_pos_x += speed/sqrt(2.0);
		character_pos_z -= speed/sqrt(2.0);
	}else if (SpecialKeyStates[2] && SpecialKeyStates[3]) {				// ���ϴ� �̵�
		character_angle = 45.0;
		if (KeyStates[0])
			ani = ANI_RUN;
		else
			ani = ANI_WALK;
		character_pos_x += speed/sqrt(2.0);
		character_pos_z += speed/sqrt(2.0);
	}else if (SpecialKeyStates[3] && SpecialKeyStates[0]) {				// ���ϴ� �̵�
		character_angle = 315.0;
		if (KeyStates[0])
			ani = ANI_RUN;
		else
			ani = ANI_WALK;
		character_pos_x -= speed/sqrt(2.0);
		character_pos_z += speed/sqrt(2.0);
	}else if (SpecialKeyStates[0]) {					// �·��̵�
		character_angle = 270.0;
		if (KeyStates[0])
			ani = ANI_RUN;
		else
			ani = ANI_WALK;
		character_pos_x -= speed;
	}else if (SpecialKeyStates[1]) {					// ����̵�
		character_angle = 180.0;
		if (KeyStates[0])
			ani = ANI_RUN;
		else
			ani = ANI_WALK;
		character_pos_z -= speed;
	}else if (SpecialKeyStates[2]) {					// ����̵�
		character_angle = 90.0;
		if (KeyStates[0])
			ani = ANI_RUN;
		else
			ani = ANI_WALK;
		character_pos_x += speed;
	}else if (SpecialKeyStates[3])  {					// �ϴ��̵�
		character_angle = 0.0;
		if (KeyStates[0])
			ani = ANI_RUN;
		else
			ani = ANI_WALK;
		character_pos_z += speed;
	}

	// �����Ǿ����� Invalidate ��Ű��
	glutPostRedisplay();
	glutTimerFunc(10, MyTimer, 1);
}

// Idle Callback Function
void MyIdle() {
	// Idle �����϶� ani�� Ȯ���Ͽ� speed�� ������
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

	glMatrixMode(GL_PROJECTION);				// ���� ��� ����
	glLoadIdentity();							// �ʱ�ȭ
	// orthograpic ���� (����ī�޶�. �� Ȯ�ο� ����)
	// glOrtho(-30.0, 30.0, -30.0, 30.0, -30.0, 60.0);
	// perspective ���� ( �þ߰�(FOV), ��Ⱦ��, Near, Far ) 
	gluPerspective(90.0, 1.0, 1.0, 200.0);
	// ī�޶��� ������ ���÷��� �ݹ� �Լ����� �������ְڴ�. (�׸������� ��ġ�� ����ǹǷ�)
	
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