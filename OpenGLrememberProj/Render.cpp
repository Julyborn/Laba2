#include "Render.h"

#include <sstream>
#include <iostream>

#include <windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#include "MyOGL.h"

#include "Camera.h"
#include "Light.h"
#include "Primitives.h"

#include "GUItextRectangle.h"

bool textureMode = true;
bool lightMode = true;
double s = 1;



double A[] = { 1, -0.5, 0 };
double B[] = { 2.5, 2, 0 };
double C[] = { 0.5, 4, 0 };
double D[] = { -4, 2.5, 0 };
double E[] = { -4, -1.5, 0 };
double F[] = { -1, -1.5, 0 };
double G[] = { 0, -4, 0 };
double H[] = { 3, -2.5, 0 };



double A1[] = { 1, -0.5, s };
double B1[] = { 2.5, 2, s };
double C1[] = { 0.5, 4, s };
double D1[] = { -4, 2.5, s };
double E1[] = { -4, -1.5, s };
double F1[] = { -1, -1.5, s };
double G1[] = { 0, -4, s };
double H1[] = { 3, -2.5, s };

double qq[] = { (D[0] + E[0]) / 2, (D[1] + E[1]) / 2, 0 };
double r = sqrt(pow((E[0] - D[0]), 2) + pow((E[1] - D[1]), 2)) / 2;
double angleD = acos((D[0] - (D[0] + E[0]) / 2) / r);
double angleE = acos((E[0] - (D[0] + E[0]) / 2) / r);






double tmp[] = { 0,0,0 };


//����� ��� ��������� ������
class CustomCamera : public Camera
{
public:
	//��������� ������
	double camDist;
	//���� �������� ������
	double fi1, fi2;

	
	//������� ������ �� ���������
	CustomCamera()
	{
		camDist = 15;
		fi1 = 1;
		fi2 = 1;
	}

	
	//������� ������� ������, ������ �� ����� ��������, ���������� �������
	void SetUpCamera()
	{
		//�������� �� ������� ������ ������
		lookPoint.setCoords(0, 0, 0);

		pos.setCoords(camDist*cos(fi2)*cos(fi1),
			camDist*cos(fi2)*sin(fi1),
			camDist*sin(fi2));

		if (cos(fi2) <= 0)
			normal.setCoords(0, 0, -1);
		else
			normal.setCoords(0, 0, 1);

		LookAt();
	}

	void CustomCamera::LookAt()
	{
		//������� ��������� ������
		gluLookAt(pos.X(), pos.Y(), pos.Z(), lookPoint.X(), lookPoint.Y(), lookPoint.Z(), normal.X(), normal.Y(), normal.Z());
	}



}  camera;   //������� ������ ������


//����� ��� ��������� �����
class CustomLight : public Light
{
public:
	CustomLight()
	{
		//��������� ������� �����
		pos = Vector3(1, 1, 3);
	}

	
	//������ ����� � ����� ��� ���������� �����, ���������� �������
	void  DrawLightGhismo()
	{
		glDisable(GL_LIGHTING);

		
		glColor3d(0.9, 0.8, 0);
		Sphere s;
		s.pos = pos;
		s.scale = s.scale*0.08;
		s.Show();
		
		if (OpenGL::isKeyPressed('G'))
		{
			glColor3d(0, 0, 0);
			//����� �� ��������� ����� �� ����������
			glBegin(GL_LINES);
			glVertex3d(pos.X(), pos.Y(), pos.Z());
			glVertex3d(pos.X(), pos.Y(), 0);
			glEnd();

			//������ ���������
			Circle c;
			c.pos.setCoords(pos.X(), pos.Y(), 0);
			c.scale = c.scale*1.5;
			c.Show();
		}

	}

	void SetUpLight()
	{
		GLfloat amb[] = { 0.2, 0.2, 0.2, 0 };
		GLfloat dif[] = { 1.0, 1.0, 1.0, 0 };
		GLfloat spec[] = { .7, .7, .7, 0 };
		GLfloat position[] = { pos.X(), pos.Y(), pos.Z(), 1. };

		// ��������� ��������� �����
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		// �������������� ����������� �����
		// ������� ��������� (���������� ����)
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		// ��������� ������������ �����
		glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
		// ��������� ���������� ������������ �����
		glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

		glEnable(GL_LIGHT0);
	}


} light;  //������� �������� �����




//������ ���������� ����
int mouseX = 0, mouseY = 0;

void mouseEvent(OpenGL *ogl, int mX, int mY)
{
	int dx = mouseX - mX;
	int dy = mouseY - mY;
	mouseX = mX;
	mouseY = mY;

	//������ ���� ������ ��� ������� ����� ������ ����
	if (OpenGL::isKeyPressed(VK_RBUTTON))
	{
		camera.fi1 += 0.01*dx;
		camera.fi2 += -0.01*dy;
	}

	
	//������� ���� �� ���������, � ����� ��� ����
	if (OpenGL::isKeyPressed('G') && !OpenGL::isKeyPressed(VK_LBUTTON))
	{
		LPPOINT POINT = new tagPOINT();
		GetCursorPos(POINT);
		ScreenToClient(ogl->getHwnd(), POINT);
		POINT->y = ogl->getHeight() - POINT->y;

		Ray r = camera.getLookRay(POINT->x, POINT->y);

		double z = light.pos.Z();

		double k = 0, x = 0, y = 0;
		if (r.direction.Z() == 0)
			k = 0;
		else
			k = (z - r.origin.Z()) / r.direction.Z();

		x = k*r.direction.X() + r.origin.X();
		y = k*r.direction.Y() + r.origin.Y();

		light.pos = Vector3(x, y, z);
	}

	if (OpenGL::isKeyPressed('G') && OpenGL::isKeyPressed(VK_LBUTTON))
	{
		light.pos = light.pos + Vector3(0, 0, 0.02*dy);
	}

	
}

void mouseWheelEvent(OpenGL *ogl, int delta)
{

	if (delta < 0 && camera.camDist <= 1)
		return;
	if (delta > 0 && camera.camDist >= 100)
		return;

	camera.camDist += 0.01*delta;

}

void keyDownEvent(OpenGL *ogl, int key)
{
	if (key == 'L')
	{
		lightMode = !lightMode;
	}

	if (key == 'T')
	{
		textureMode = !textureMode;
	}

	if (key == 'R')
	{
		camera.fi1 = 1;
		camera.fi2 = 1;
		camera.camDist = 15;

		light.pos = Vector3(1, 1, 3);
	}

	if (key == 'F')
	{
		light.pos = camera.pos;
	}
}

void keyUpEvent(OpenGL *ogl, int key)
{
	
}



GLuint texId;

//����������� ����� ������ ��������
void initRender(OpenGL *ogl)
{
	//��������� �������

	//4 ����� �� �������� �������
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	//��������� ������ ��������� �������
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//�������� ��������
	glEnable(GL_TEXTURE_2D);
	

	//������ ����������� ���������  (R G B)
	RGBTRIPLE *texarray;

	//������ ��������, (������*������*4      4, ���������   ����, �� ������� ������������ �� 4 ����� �� ������� �������� - R G B A)
	char *texCharArray;
	int texW, texH;
	OpenGL::LoadBMP("texture.bmp", &texW, &texH, &texarray);
	OpenGL::RGBtoChar(texarray, texW, texH, &texCharArray);

	
	
	//���������� �� ��� ��������
	glGenTextures(1, &texId);
	//������ ��������, ��� ��� ����� ����������� � ���������, ����� ����������� �� ����� ��
	glBindTexture(GL_TEXTURE_2D, texId);

	//��������� �������� � �����������, � ���������� ��� ������  ��� �� �����
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray);

	//�������� ������
	free(texCharArray);
	free(texarray);

	//������� ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	//������ � ���� ����������� � "������"
	ogl->mainCamera = &camera;
	ogl->mainLight = &light;

	// ������������ �������� : �� ����� ����� ����� 1
	glEnable(GL_NORMALIZE);

	// ���������� ������������� ��� �����
	glEnable(GL_LINE_SMOOTH); 


	//   ������ ��������� ���������
	//  �������� GL_LIGHT_MODEL_TWO_SIDE - 
	//                0 -  ������� � ���������� �������� ���������(�� ���������), 
	//                1 - ������� � ���������� �������������� ������� ��������       
	//                �������������� ������� � ���������� ��������� ����������.    
	//  �������� GL_LIGHT_MODEL_AMBIENT - ������ ������� ���������, 
	//                �� ��������� �� ���������
	// �� ��������� (0.2, 0.2, 0.2, 1.0)

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);

	camera.fi1 = -1.3;
	camera.fi2 = 0.8;
}


void normal(double A[], double B[], double C[], double* tmp)
{
	double a[] = { 0,0,0 };
	double b[] = { 0,0,0 };
	double len;
	for (int i = 0; i < 3; i++)
	{
		a[i] = A[i] - B[i];
		b[i] = C[i] - B[i];
	}
	tmp[0] = a[1] * b[2] - b[1] * a[2];
	tmp[1] = -a[0] * b[2] + b[0] * a[2];
	tmp[2] = a[0] * b[1] - b[0] * a[1];
	len = sqrt(pow(tmp[0], 2) + pow(tmp[1], 2) + pow(tmp[2], 2));
	tmp[0] /= len;
	tmp[1] /= len;
	tmp[2] /= len;
}

void flor(double z)
{
	if (z == 0) {
		glColor3f(1.0f, 0.1f, 0.0f);
		glNormal3d(0, 0, -1);
	}
	else {
		glColor4d(1.0, 0.1, 0.66,0.9);
		glNormal3d(0, 0, 1);
	}

	for (double a = -angleD; a <= angleE; a += 0.01) {
		glBegin(GL_TRIANGLES);
		glVertex3d(qq[0], qq[1], z);
		glVertex3f(-4 - cos(a) * r, 0.5 - sin(a) * r, z);
		glVertex3f(-4 - cos(a + 0.01) * r, 0.5 - sin(a + 0.01) * r, z);
		glEnd();
	}

	//for (double a = -angleD2; a <= angleC; a += 0.01) {
	//	glBegin(GL_TRIANGLES);
	//	glVertex3d(0, 0, z);
	//	glVertex3f(-1.75 + cos(a) * r2, 3.25 + sin(a) * r2, z);
	//	glVertex3f(-1.75 + cos(a + 0.01) * r2, 3.25 + sin(a + 0.01) * r2, z);
	//	glEnd();
	//}



	glBegin(GL_TRIANGLES);
	glTexCoord2d(0, 0);
	glVertex3d(0, 0, z);
	glTexCoord2d(1, 1);
	glVertex3d(H[0], H[1], z);
	glTexCoord2d(0.5, 0.5);
	glVertex3d(G[0], G[1], z);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2d(0, 0);
	glVertex3d(0, 0, z);
	glTexCoord2d(1, 1);
	glVertex3d(A[0], A[1], z);
	glTexCoord2d(0.5, 0.5);
	glVertex3d(H[0], H[1], z);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2d(0, 0);
	glVertex3d(0, 0, z);
	glTexCoord2d(1, 1);
	glVertex3d(B[0], B[1], z);
	glTexCoord2d(0.5, 0.5);
	glVertex3d(A[0], A[1], z);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2d(0, 0);
	glVertex3d(0, 0, z);
	glTexCoord2d(1, 1);
	glVertex3d(C[0], C[1], z);
	glTexCoord2d(0.5, 0.5);
	glVertex3d(B[0], B[1], z);
	glEnd();

	

	glBegin(GL_TRIANGLES);
	glTexCoord2d(0, 0);
	glVertex3d(0, 0, z);
	glTexCoord2d(1, 1);
	glVertex3d(E[0], E[1], z);
	glTexCoord2d(0.5, 0.5);
	glVertex3d(D[0], D[1], z);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2d(0, 0);
	glVertex3d(0, 0, z);
	glTexCoord2d(1, 1);
	glVertex3d(D[0], D[1], z);
	glTexCoord2d(0.5, 0.5);
	glVertex3d(C[0], C[1], z);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2d(0, 0);
	glVertex3d(0, 0, z);
	glTexCoord2d(1, 1);
	glVertex3d(F[0], F[1], z);
	glTexCoord2d(0.5, 0.5);
	glVertex3d(E[0], E[1], z);
	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord2d(0, 0);
	glVertex3d(0, 0, z);
	glTexCoord2d(1, 1);
	glVertex3d(G[0], G[1], z);
	glTexCoord2d(0.5, 0.5);
	glVertex3d(F[0], F[1], z);
	glEnd();



	
}

void wall(double z)
{
	
	glBegin(GL_QUADS);
	glColor3d(0.2, 0.7, 0.7);
	normal(B, A,A1, tmp);
	glNormal3dv(tmp);
	glVertex3dv(A);
	glVertex3dv(A1);
	glVertex3dv(B1);
	glVertex3dv(B);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(1.0, 0.0, 1.0);
	normal(C, B, C1, tmp);
	glNormal3dv(tmp);
	glVertex3dv(B);
	glVertex3dv(C);
	glVertex3dv(C1);
	glVertex3dv(B1);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(1.0, 0.0, 1.0);
	normal(D, C, D1, tmp);
	glNormal3dv(tmp);
	glVertex3dv(C);
	glVertex3dv(D);
	glVertex3dv(D1);
	glVertex3dv(C1);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0.0, 0.0, 1.0);
	normal(E, E1, F1, tmp);
	glNormal3dv(tmp);
	glVertex3dv(E);
	glVertex3dv(E1);
	glVertex3dv(F1);
	glVertex3dv(F);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0.0, 0.0, 1.0);
	normal(F, F1, G1, tmp);
	glNormal3dv(tmp);
	glVertex3dv(F);
	glVertex3dv(F1);
	glVertex3dv(G1);
	glVertex3dv(G);
	glEnd();



	glBegin(GL_QUADS);
	glColor3d(0.0, 0.0, 1.0);
	normal(H, G, H1, tmp);
	glNormal3dv(tmp);
	glVertex3dv(G);
	glVertex3dv(G1);
	glVertex3dv(H1);
	glVertex3dv(H);
	glEnd();





	glBegin(GL_QUADS);
	glColor3d(0.0, 0.0, 1.0);
	normal(A, H, A1, tmp);
	glNormal3dv(tmp);
	glVertex3dv(A);
	glVertex3dv(A1);
	glVertex3dv(H1);
	glVertex3dv(H);
	glEnd();

	glBegin(GL_QUADS);
	glColor3d(0.5, 0.5, 1);
	for (double a = -angleD; a <= angleE; a += 0.01) {
		double x1 = -4.0 - cos(a) * r;
		double y1 = 0.5 - sin(a) * r;
		double x2 = -4.0 - cos(a + 0.01) * r;
		double y2 = 0.5 - sin(a + 0.01) * r;
		double xx1[3], xx2[3], xx3[3], xx4[3];
		xx1[0] = x1;
		xx1[1] = y1;
		xx1[2] = 0;

		xx2[0] = x1;
		xx2[1] = y1;
		xx2[2] = 1;

		xx3[0] = x2;
		xx3[1] = y2;
		xx3[2] = 0;

		xx4[0] = x2;
		xx4[1] = y2;
		xx4[2] = 1;

		normal(xx1, xx2, xx3, tmp);
		glNormal3dv(tmp);
		glVertex3dv(xx1);
		glVertex3dv(xx2);
		glVertex3dv(xx4);
		glVertex3dv(xx3);
	}
	glEnd();


}




void Render(OpenGL *ogl)
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glEnable(GL_DEPTH_TEST);
	if (textureMode)
		glEnable(GL_TEXTURE_2D);

	if (lightMode)
		glEnable(GL_LIGHTING);

	//��������������
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//��������� ���������
	GLfloat amb[] = { 0.2, 0.2, 0.1, 1. };
	GLfloat dif[] = { 0.4, 0.65, 0.5, 1. };
	GLfloat spec[] = { 0.9, 0.8, 0.3, 1. };
	GLfloat sh = 0.1f * 256;

	//�������
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	//��������
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	//����������
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec); \
		//������ �����
		glMaterialf(GL_FRONT, GL_SHININESS, sh);

	//���� ���� �������, ��� ����������� (����������� ���������)
	glShadeModel(GL_SMOOTH);
	//===================================
	//������� ���  
	glBindTexture(GL_TEXTURE_2D, texId);
	wall(1);
	flor(0);
	flor(1);

	/*double A[2] = { -4, -4 };
	double B[2] = { 4, -4 };
	double C[2] = { 4, 4 };
	double D[2] = { -4, 4 };

	glBindTexture(GL_TEXTURE_2D, texId);

	glColor3d(0.6, 0.6, 0.6);
	glBegin(GL_QUADS);

	glNormal3d(0, 0, 1);
	glTexCoord2d(0, 0);
	glVertex2dv(A);
	glTexCoord2d(1, 0);
	glVertex2dv(B);
	glTexCoord2d(1, 1);
	glVertex2dv(C);
	glTexCoord2d(0, 1);
	glVertex2dv(D);

	glEnd();*/
	//����� ��������� ���������� ��������


   //��������� ������ ������

	
	glMatrixMode(GL_PROJECTION);	//������ �������� ������� ��������. 
	                                //(���� ��������� ��������, ����� �� ������������.)
	glPushMatrix();   //��������� ������� ������� ������������� (������� ��������� ������������� ��������) � ���� 				    
	glLoadIdentity();	  //��������� ��������� �������
	glOrtho(0, ogl->getWidth(), 0, ogl->getHeight(), 0, 1);	 //������� ����� ������������� ��������

	glMatrixMode(GL_MODELVIEW);		//������������� �� �����-��� �������
	glPushMatrix();			  //��������� ������� ������� � ���� (��������� ������, ����������)
	glLoadIdentity();		  //���������� �� � ������

	glDisable(GL_LIGHTING);



	GuiTextRectangle rec;		   //������� ����� ��������� ��� ������� ������ � �������� ������.
	rec.setSize(300, 150);
	rec.setPosition(10, ogl->getHeight() - 150 - 10);


	std::stringstream ss;
	ss << "T - ���/���� �������" << std::endl;
	ss << "L - ���/���� ���������" << std::endl;
	ss << "F - ���� �� ������" << std::endl;
	ss << "G - ������� ���� �� �����������" << std::endl;
	ss << "G+��� ������� ���� �� ���������" << std::endl;
	ss << "�����. �����: (" << light.pos.X() << ", " << light.pos.Y() << ", " << light.pos.Z() << ")" << std::endl;
	ss << "�����. ������: (" << camera.pos.X() << ", " << camera.pos.Y() << ", " << camera.pos.Z() << ")" << std::endl;
	ss << "��������� ������: R="  << camera.camDist << ", fi1=" << camera.fi1 << ", fi2=" << camera.fi2 << std::endl;
	
	rec.setText(ss.str().c_str());
	rec.Draw();

	glMatrixMode(GL_PROJECTION);	  //��������������� ������� �������� � �����-��� �������� �� �����.
	glPopMatrix();


	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}


