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


/*========== Texture ���� ==========*/
unsigned int MyTextureObject[1];
AUX_RGBImageRec *pTextureImage[1];			//�ؽ��� ���� ������ ����Ű�� ������

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
	glBindTexture(GL_TEXTURE_2D, MyTextureObject[0]);
	glBegin(GL_QUADS);

	/*
	glTexCoord2f(0.0f,0.0f); glVertex3f(-1.0f, -1.0f, 1.0f); //�ո�
	glTexCoord2f(1.0f,0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f,1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f,1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	
	glTexCoord2f(1.0f,0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);//�޸�
	glTexCoord2f(1.0f,1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f,1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f,0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	*/
	glTexCoord2f(0.0f,1.0f); glVertex3f(-300.0f, 0.0f, -150.0f);//����
	glTexCoord2f(0.0f,0.0f); glVertex3f(-300.0f, 0.0f, 150.0f);
	glTexCoord2f(1.0f,0.0f); glVertex3f(300.0f,  0.0f, 150.0f);
	glTexCoord2f(1.0f,1.0f); glVertex3f(300.0f,  0.0f, -150.0f);
	
	/*
	glTexCoord2f(1.0f,1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);//�Ʒ���
	glTexCoord2f(0.0f,1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f,0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f,0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);

	
	glTexCoord2f(1.0f,0.0f); glVertex3f(1.0f, -1.0f, -1.0f);//������ ����
	glTexCoord2f(1.0f,1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f,1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f,0.0f); glVertex3f(1.0f, -1.0f, 1.0f);

	glTexCoord2f(0.0f,0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);//���� ����
	glTexCoord2f(1.0f,0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f,1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f,1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	*/
	glEnd();

		/* ����
		glColor3f(0.0f, 1.0f, 0.0f);			//R,G,B ����
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
	glEnable(GL_DEPTH_TEST);			// ���� �׽�Ʈ Ȱ��ȭ

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	// gluLookAt() �Լ��� ī�޶��� ������ ���� ���� (���� : ī�޶���ġ / ���´�� / ī�޶� ������ ����)
	// ī�޶�� ĳ���͸� �����ٹ�����. (���� ���ٷ� ����)
	gluLookAt(character_pos_x+0.0, 30.0, character_pos_z+60.0,   character_pos_x, 0.0, character_pos_z,   0.0, 6.0, -3.0);

	DrawGround();						// ������ �׸���. �ؽ�ó�̿��� ������ ����
	DrawText();							//�ؽ�Ʈ ���
	
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


AUX_RGBImageRec * LoadBMP(char *szFilename){
	FILE *pFile = NULL;
	if(!szFilename){
		return NULL;
	}
	pFile = fopen(szFilename,"r");
	if(pFile){
		fclose(pFile);
		return auxDIBImageLoad(szFilename);	//���Ͽ��� �޸𸮷�
	}
	return NULL;
}

int LoadGLTextures(char *szFilePath){		//������ �ε��ϰ� �ؽ��ķ� ��ȯ�ϴ� ����
	int Status = FALSE;
	glClearColor(0,0,0,0.5);
	memset(pTextureImage,0,sizeof(void *)*1);//�����͸� �η� 
	
	if(pTextureImage[0]=LoadBMP(szFilePath)){
		Status=TRUE;						 //���� �÷��� TRUE�� ����.
		glGenTextures(1,&MyTextureObject[0]);//�ؽ��� ����
		glBindTexture(GL_TEXTURE_2D, MyTextureObject[0]);
		glTexImage2D(GL_TEXTURE_2D,0,3,pTextureImage[0]->sizeX,pTextureImage[0]->sizeY,0,GL_RGB,GL_UNSIGNED_BYTE,pTextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glEnable(GL_TEXTURE_2D);
	}

	if(pTextureImage[0]){					//�ؽ�ó�� �����ϸ�
		if(pTextureImage[0]->data){			//�ؽ�ó ������ �����ϸ�
			free(pTextureImage[0]->data);	//�ؽ�ó ���� ���� �ݳ�
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

	glMatrixMode(GL_PROJECTION);				// ���� ��� ����
	glLoadIdentity();							// �ʱ�ȭ
	// orthograpic ���� (����ī�޶�. �� Ȯ�ο� ����)
	// glOrtho(-30.0, 30.0, -30.0, 30.0, -30.0, 60.0);
	// perspective ���� ( �þ߰�(FOV), ��Ⱦ��, Near, Far ) 
	gluPerspective(90.0, 1.0, 1.0, 200.0);
	// ī�޶��� ������ ���÷��� �ݹ� �Լ����� �������ְڴ�. (�׸������� ��ġ�� ����ǹǷ�)
	

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