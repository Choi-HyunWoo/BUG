#include <gl/glut.h>
#include <gl/gl.h>
#include <gl/GLU.h>
#include <gl/GLAux.h>
#include <math.h>


/* Animation list.
 0. stand
 1. walk
 2. run (����)
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

//�㸮 ����
GLfloat waist_ang = 0.0f;
// �� ����
GLfloat neck_ang = 0.0f;


// ĳ���� ����
GLfloat character_dir= 0.0f;

// time
static double walkTime = 0.0;
static double runTime = 0.0;



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
	
	DrawCharacter(0.0f, 0.0f, 0.0f);
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
	glRotatef(abs(sin(walkTime)*5), 1, 0, 0);	// x���� �������� 16������ ������ Ʋ����
	glRotatef(sin(walkTime)*5, 0, 1, 0);		// y���� �������� 16������ ������ Ʋ���� sin() �Լ��� �ֱ����� ������ ����

	// ĳ���Ͱ� �޸��鼭 ���Ϸ� �����̴� ���� ǥ��
	float i=0;
	i=abs(sin(walkTime)*0.3);					// i ���� �� ����
	glTranslatef(0.0, i-0.2, 0);				// �κ��� ��ü�� y�� �������� ���� i��ŭ �̵�
	
	DrawCharacter(0.0f, 0.0f, 0.0f);
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
	glRotatef(abs(sin(runTime)*15), 1, 0, 0);	// x���� �������� 16������ ������ Ʋ����
	glRotatef(sin(runTime)*15, 0, 1, 0);		// y���� �������� 16������ ������ Ʋ���� sin() �Լ��� �ֱ����� ������ ����

	// ĳ���Ͱ� �޸��鼭 ���Ϸ� �����̴� ���� ǥ��
	float i=0;
	i=abs(sin(runTime)*1.0);					// i ���� �� ����
	glTranslatef(0.0, i-0.5, 0);				// �κ��� ��ü�� y�� �������� ���� i��ŭ �̵�
	
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
 
     // RGB�� ������ �ٲ۴�.
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