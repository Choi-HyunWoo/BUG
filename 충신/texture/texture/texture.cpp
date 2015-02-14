#include <gl\glut.h>
#include <gl\GLU.h>
#include <gl\GLAux.h>
#include <stdio.h>
#pragma COMMENT <lib, "glaux.lib">

GLfloat xrot,yrot,zrot;
unsigned int MyTextureObject[1];
AUX_RGBImageRec *pTextureImage[1];			//텍스쳐 저장 공간을 가리키는 포인터

void MyReshape(int w, int h){
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, (GLfloat)w/(GLfloat)h, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0,0,5,0,0,0,0,1,0);
}

void MyDisplay(){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
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
	glTexCoord2f(0.0f,1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);//윗면
	glTexCoord2f(0.0f,0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f,0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f,1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	
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
	glutSwapBuffers();
}

void MyTimer(int value){
	glRotatef(xrot,1.0f,0.0f,0.0f);
	glRotatef(yrot,0.0f,1.0f,0.0f);
	glRotatef(zrot,0.0f,0.0f,1.0f);
	xrot+=0.3f;
	yrot+=0.2f;
	zrot+=0.4f;
	glutPostRedisplay();
	glutTimerFunc(500,MyTimer,1);
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

void main(int argc, char *argv[]){
	/*
	if(argc<=1){
		printf("\n%s\n\n","Usage : TextrueDLG3_Consol.exe[BMPFileName.bmp]");
		exit(1);
	}
	else if(argc>2){
		printf("\nToo Many Arguments!\nArgument is Only one\n");
		exit(1);
	}
	*/
	//else{
		glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
		glutCreateWindow("OpenGL Sample Program");
		glutDisplayFunc(MyDisplay);
		glutReshapeFunc(MyReshape);
		glutTimerFunc(500,MyTimer,1);
		if(LoadGLTextures(argv[1])){
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_SMOOTH);
			glClearDepth(1.0);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
			glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
			glutMainLoop();
	//	}
	}
}

