// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://go.microsoft.com/fwlink/?LinkId=238214��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// SolarView.cpp : CSolarView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "Solar.h"
#endif

#include "SolarDoc.h"
#include "SolarView.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSolarView

IMPLEMENT_DYNCREATE(CSolarView, CView)

BEGIN_MESSAGE_MAP(CSolarView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSolarView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_COMMAND(ID_CTRL_PAUSE, &CSolarView::OnCtrlPause)
	ON_UPDATE_COMMAND_UI(ID_CTRL_PAUSE, &CSolarView::OnUpdateCtrlPause)
	ON_COMMAND(ID_CHANGE_ROTATE_SPEED, &CSolarView::OnChangeRotateSpeed)
END_MESSAGE_MAP()

// CSolarView ����/����

GLfloat LightAmbient[]   = {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat LightDiffuse[]   = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat LightPosition[]  = {0.0f, 0.0f, 0.0f, 1.0f};

GLfloat LightAmbient2[]  = {0.2f, 0.2f, 0.2f, 1.0f};
GLfloat LightDiffuse2[]  = {0.5f, 0.5f, 0.5f, 1.0f};
GLfloat LightPosition2[] = {0.0f, 0.0f, 2.0f, 1.0f};

CSolarView::CSolarView()
{
	// TODO: �ڴ˴���ӹ������
	iCometAngle = 0;
	iCometPos = 0;
	m_fRotationSpeed = 5;
}

CSolarView::~CSolarView()
{
}

BOOL CSolarView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CSolarView ����

void CSolarView::OnDraw(CDC* /*pDC*/)
{
	CSolarDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	RenderScene();	//��Ⱦ����
}


// CSolarView ��ӡ


void CSolarView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CSolarView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CSolarView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CSolarView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CSolarView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSolarView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CSolarView ���

#ifdef _DEBUG
void CSolarView::AssertValid() const
{
	CView::AssertValid();
}

void CSolarView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSolarDoc* CSolarView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSolarDoc)));
	return (CSolarDoc*)m_pDocument;
}
#endif //_DEBUG

// CSolarView ��Ϣ�������

int CSolarView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	//��ʼ��OpenGL�����ö�ʱ��
	m_pDC = new CClientDC(this);
	SetTimer(1, 20, NULL);
	InitializeOpenGL(m_pDC);
	init();
	InitPlanets();
	return 0;
}

void CSolarView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������

	//ɾ����ɫ�����Ⱦ�����ġ���ʱ��
	::wglMakeCurrent(0,0);
	::wglDeleteContext( m_hRC);
	if (m_hPalette)
		DeleteObject(m_hPalette);
	if ( m_pDC )
	{
		delete m_pDC;
	}
	KillTimer(1);
}

void CSolarView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������

	//��Ӵ�������ʱ��ͼ�α任����
	glViewport(0,0,cx,cy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();	
	if (cy == 0)	
	{
		cy = 1;	
	}
	gluPerspective(45, (GLfloat)cx/(GLfloat)cy, 1.0, 20.0) ;
	glTranslatef(0.0,0.0,-5.0);
	glMatrixMode(GL_MODELVIEW);	
}


void CSolarView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//��Ӷ�ʱ����Ӧ�����ͳ������º���
	Invalidate(FALSE);	
	rotate();
	CView::OnTimer(nIDEvent);
}

// �����߼���ɫ��
void CSolarView::SetLogicalPalette(void)
{
	struct
	{
		WORD Version;
		WORD NumberOfEntries;
		PALETTEENTRY aEntries[256];
	} logicalPalette = { 0x300, 256 };

	BYTE reds[] = {0, 36, 72, 109, 145, 182, 218, 255};
	BYTE greens[] = {0, 36, 72, 109, 145, 182, 218, 255};
	BYTE blues[] = {0, 85, 170, 255};

	for (int colorNum=0; colorNum<256; ++colorNum)
	{
		logicalPalette.aEntries[colorNum].peRed =
			reds[colorNum & 0x07];
		logicalPalette.aEntries[colorNum].peGreen =
			greens[(colorNum >> 0x03) & 0x07];
		logicalPalette.aEntries[colorNum].peBlue =
			blues[(colorNum >> 0x06) & 0x03];
		logicalPalette.aEntries[colorNum].peFlags = 0;
	}

	m_hPalette = CreatePalette ((LOGPALETTE*)&logicalPalette);
}

// ��ʼ��openGL����
BOOL CSolarView::InitializeOpenGL(CDC* pDC)
{
	m_pDC = pDC;
	SetupPixelFormat();
	//���ɻ���������
	m_hRC = ::wglCreateContext(m_pDC->GetSafeHdc());
	//�õ�ǰ����������
	::wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);

	return TRUE;
}

// �������ظ�ʽ
BOOL CSolarView::SetupPixelFormat()
{
	PIXELFORMATDESCRIPTOR pfd = { 
		sizeof(PIXELFORMATDESCRIPTOR),    // pfd�ṹ�Ĵ�С 
		1,                                // �汾�� 
		PFD_DRAW_TO_WINDOW |              // ֧���ڴ����л�ͼ 
		PFD_SUPPORT_OPENGL |              // ֧�� OpenGL 
		PFD_DOUBLEBUFFER,                 // ˫����ģʽ 
		PFD_TYPE_RGBA,                    // RGBA ��ɫģʽ 
		24,                               // 24 λ��ɫ��� 
		0, 0, 0, 0, 0, 0,                 // ������ɫλ 
		0,                                // û�з�͸���Ȼ��� 
		0,                                // ������λλ 
		0,                                // ���ۼӻ��� 
		0, 0, 0, 0,                       // �����ۼ�λ 
		32,                               // 32 λ��Ȼ���     
		0,                                // ��ģ�建�� 
		0,                                // �޸������� 
		PFD_MAIN_PLANE,                   // ���� 
		0,                                // ���� 
		0, 0, 0                           // ���Բ�,�ɼ��Ժ������ģ 
	}; 	
	int pixelformat;
	pixelformat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);//ѡ�����ظ�ʽ
	::SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd);	 //�������ظ�ʽ
	if(pfd.dwFlags & PFD_NEED_PALETTE)
		SetLogicalPalette();	                                 //�����߼���ɫ��
	return TRUE;
}

// ������������Ⱦ
BOOL CSolarView::RenderScene() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glTranslatef(globalPosition.fPosX, 0.0, 0.0);
	glTranslatef(0.0, globalPosition.fPosY, 0.0);
	LightPosition[0]		= LightPosition[0]  + globalPosition.fPosX;
	LightPosition[1]		= LightPosition[1]  + globalPosition.fPosX;
	LightPosition2[0]		= LightPosition2[0] + globalPosition.fPosX;
	LightPosition2[1]		= LightPosition2[1] + globalPosition.fPosX;
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
	glLightfv(GL_LIGHT2, GL_POSITION,LightPosition2);	
	globalPosition.fPosX	= 0;
	globalPosition.fPosY	= 0;

	DrawSun();			                    // ����̫��
	DrawAllOrbits();                        // �������й��
	DrawAllPlanets();                       // ������������
	DrawComet();                            // ��������

	::SwapBuffers(m_pDC->GetSafeHdc());		// ����������
	return TRUE;
}

void CSolarView::DrawSun() 
{
	glPushMatrix();
	glutSolidSphere(0.05, 10, 10);
	glPopMatrix();
}

void CSolarView::DrawAllOrbits()
{
	DrawOrbit(mercury);
	DrawOrbit(venus);
	DrawOrbit(earth);
	DrawOrbit(mars);
	DrawOrbit(jupiter);
	DrawOrbit(saturn);
	DrawOrbit(uranus);
	DrawOrbit(neptune);
	DrawOrbit(pluto);
}

void CSolarView::DrawOrbit(CPlanet planet)
{
	glDisable(GL_LIGHTING);
	glColor3f (0.3, 0.3, 0.3);

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 360; i++)
	{
		glVertex3f(cos(DEG2RAD(i))*planet.posZ, 0, sin(DEG2RAD(i))*planet.posZ);
	}
	glEnd();

	glEnable(GL_LIGHTING);
}

void CSolarView::DrawAllPlanets()
{
	glEnable(GL_TEXTURE_2D);

	DrawPlanet(mercury);
	DrawPlanet(venus);
	DrawPlanet(earth);
	DrawPlanet(mars);
	DrawPlanet(jupiter);
	DrawPlanet(saturn);
	DrawPlanet(uranus);
	DrawPlanet(neptune);
	DrawPlanet(pluto);

	glDisable(GL_TEXTURE_2D);
}

void CSolarView::DrawPlanet(CPlanet planet)
{
	glPushMatrix();

	glRotatef(planet.SolarAngle, 0, -1, 0);
	glTranslatef(planet.posX, planet.posY, planet.posZ);
	glRotatef(planet.OwnAxisAngle, 0, -1, 0);
	glRotatef(90.0, 1.0, 0.0, 0.0); 

	glBindTexture(GL_TEXTURE_2D, m_nTextures_id[planet.TextureID]);
	GLUquadricObj*  q = gluNewQuadric();
	gluQuadricDrawStyle(q, GLU_FILL);
	gluQuadricNormals (q, GLU_SMOOTH);
	gluQuadricTexture (q, GL_TRUE);
	gluSphere(q, planet.size, planet.sections, planet.sections);
	gluDeleteQuadric(q);

	glPopMatrix();
}

void CSolarView::DrawComet()
{
	glPushMatrix();

	glRotatef(iCometAngle, 0, 0, 1);
	glTranslatef(1, 0, 0);
	glutSolidSphere(0.02, 20, 20);

	glPopMatrix();
}

void CSolarView::init(void)
{
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
	glEnable(GL_LIGHT1);

	glLightfv(GL_LIGHT2, GL_AMBIENT, LightAmbient2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, LightDiffuse2);
	glLightfv(GL_LIGHT2, GL_POSITION,LightPosition2);

	glEnable(GL_LIGHT2);

	glClearColor(0, 0, 0, 0);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);

	m_Textures.LoadTextures(m_nTextures_id, MAXTEXTURE);

	globalPosition.fPosX = 0;
	globalPosition.fPosY = 0;
	globalPosition.iDegreesX = 30;
	globalPosition.iDegreesY = 30;
	globalPosition.bRotationOn = true;

	glRotatef(globalPosition.iDegreesX, 0.0, 1.0, 0.0);
	glRotatef(globalPosition.iDegreesY, 1.0, 0.0, 0.0);
}

void CSolarView::InitPlanets()
{
	mercury.size			= 0.04;
	mercury.sections		= 16;
	mercury.posX			= 0;
	mercury.posY			= 0;
	mercury.posZ			= 0.1;
	mercury.OwnAxisAngle	= 0;
	mercury.SolarAngle		= 0;
	mercury.TextureID		= 0;

	venus.size				= 0.09;
	venus.sections			= 16;
	venus.posX				= 0;
	venus.posY				= 0;
	venus.posZ				= 0.3;
	venus.OwnAxisAngle		= 0;
	venus.SolarAngle		= 225;
	venus.TextureID			= 1;

	earth.size				= 0.09;
	earth.sections			= 16;
	earth.posX				= 0;
	earth.posY				= 0;
	earth.posZ				= 0.5;
	earth.OwnAxisAngle		= 0;
	earth.SolarAngle		= 45;
	earth.TextureID			= 2;

	mars.size				= 0.07;
	mars.sections			= 16;
	mars.posX				= 0;
	mars.posY				= 0;
	mars.posZ				= 0.8;
	mars.OwnAxisAngle		= 0;
	mars.SolarAngle			= 270;
	mars.TextureID			= 3;

	jupiter.size			= 0.26;
	jupiter.sections		= 16;
	jupiter.posX			= 0;
	jupiter.posY			= 0;
	jupiter.posZ			= 1.3;
	jupiter.OwnAxisAngle	= 0;
	jupiter.SolarAngle		= 90;
	jupiter.TextureID		= 4;

	saturn.size				= 0.22;
	saturn.sections			= 16;
	saturn.posX				= 0;
	saturn.posY				= 0;
	saturn.posZ				= 1.9;
	saturn.OwnAxisAngle		= 0;
	saturn.SolarAngle		= 315;
	saturn.TextureID		= 5;

	uranus.size				= 0.18;
	uranus.sections			= 16;
	uranus.posX				= 0;
	uranus.posY				= 0;
	uranus.posZ				= 2.5;
	uranus.OwnAxisAngle		= 0;
	uranus.SolarAngle		= 135;
	uranus.TextureID		= 6;

	neptune.size			= 0.2;
	neptune.sections		= 16;
	neptune.posX			= 0;
	neptune.posY			= 0;
	neptune.posZ			= 3.3;
	neptune.OwnAxisAngle	= 0;
	neptune.SolarAngle		= 0;
	neptune.TextureID		= 7;

	pluto.size				= 0.05;
	pluto.sections			= 16;
	pluto.posX				= 0;
	pluto.posY				= 0;
	pluto.posZ				= 3.9;
	pluto.OwnAxisAngle		= 0;
	pluto.SolarAngle		= 180;
	pluto.TextureID			= 8;
}

void CSolarView::rotate()
{
	if (globalPosition.bRotationOn)
	{
		// comet
		iCometAngle += 2;
		if (iCometAngle >= 360)
		{
			iCometAngle -= 360;
		}

		// solar rotation
		mercury.SolarAngle += 1.6 * m_fRotationSpeed;
		if (mercury.SolarAngle >= 360){mercury.SolarAngle -= 360;}

		venus.SolarAngle += 1.2 * m_fRotationSpeed;
		if (venus.SolarAngle >= 360){venus.SolarAngle -= 360;}

		earth.SolarAngle += 1.1 * m_fRotationSpeed;
		if (earth.SolarAngle >= 360){earth.SolarAngle -= 360;}

		mars.SolarAngle += 0.9 * m_fRotationSpeed;
		if (mars.SolarAngle >= 360){mars.SolarAngle -= 360;}

		jupiter.SolarAngle += 0.1 * m_fRotationSpeed;
		if (jupiter.SolarAngle >= 360){jupiter.SolarAngle -= 360;}

		saturn.SolarAngle += 0.3 * m_fRotationSpeed;
		if (saturn.SolarAngle >= 360){saturn.SolarAngle -= 360;}

		uranus.SolarAngle += 0.06 * m_fRotationSpeed;
		if (uranus.SolarAngle >= 360){uranus.SolarAngle -= 360;}

		neptune.SolarAngle += 0.03 * m_fRotationSpeed;
		if (neptune.SolarAngle >= 360){neptune.SolarAngle -= 360;}

		pluto.SolarAngle += 0.01 * m_fRotationSpeed;
		if (pluto.SolarAngle >= 360){pluto.SolarAngle -= 360;}

		// axis rotation
		mercury.OwnAxisAngle += 0.1;
		if (mercury.OwnAxisAngle >= 360){mercury.OwnAxisAngle -= 360;}

		venus.OwnAxisAngle += 0.2;
		if (venus.OwnAxisAngle >= 360){venus.OwnAxisAngle -= 360;}

		earth.OwnAxisAngle += 0.6;
		if (earth.OwnAxisAngle >= 360){earth.OwnAxisAngle -= 360;}

		mars.OwnAxisAngle += 0.5;
		if (mars.OwnAxisAngle >= 360){mars.OwnAxisAngle -= 360;}

		jupiter.OwnAxisAngle += 2.6;
		if (jupiter.OwnAxisAngle >= 360){jupiter.OwnAxisAngle -= 360;}

		saturn.OwnAxisAngle += 2.5;
		if (saturn.OwnAxisAngle >= 360){saturn.OwnAxisAngle -= 360;}

		uranus.OwnAxisAngle += 1.5;
		if (uranus.OwnAxisAngle >= 360){uranus.OwnAxisAngle -= 360;}

		neptune.OwnAxisAngle += 1.7;
		if (neptune.OwnAxisAngle >= 360){neptune.OwnAxisAngle -= 360;}

		pluto.OwnAxisAngle +=3;
		if (pluto.OwnAxisAngle >= 360){pluto.OwnAxisAngle -= 360;}
	}
}

//
//*** ��Ϣ��Ӧ ***
//

void CSolarView::OnCtrlPause()
{
	globalPosition.bRotationOn = !globalPosition.bRotationOn;
}

void CSolarView::OnUpdateCtrlPause(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(!globalPosition.bRotationOn);
}


void CSolarView::OnChangeRotateSpeed()
{
}