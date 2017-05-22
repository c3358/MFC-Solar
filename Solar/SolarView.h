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

// SolarView.h : CSolarView ��Ľӿ�
//

#pragma once
//�����ļ����ṹ�弰�궨��
#include "Texture.h"

#define MAXTEXTURE			    9
#define PI						3.141592654
#define DEG2RAD(angle)			angle * (PI / 180)	// ���Ƕ�ת��Ϊ����

// �ṹ�壺����
struct CPlanet
{
	float size, sections, posX, posY, posZ, SolarAngle, OwnAxisAngle;
	int TextureID;
} ;

struct GlobalPosition
{
	float fPosX, fPosY;
	int iDegreesX, iDegreesY;
	bool bRotationOn;
} ;

class CSolarView : public CView
{
protected: // �������л�����
	CSolarView();
	DECLARE_DYNCREATE(CSolarView)

// ����
public:
	CSolarDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CSolarView();

public:
	// ��ӳ�Ա�������Ա����
	HGLRC		                   m_hRC; // OpenGL����������
	HPALETTE	              m_hPalette; // OpenGL��ɫ��
	CDC*	                       m_pDC; // OpenGL�豸������

	GLuint    m_nTextures_id[MAXTEXTURE]; // ����������
	int           iCometAngle, iCometPos; // 
	CTextures                 m_Textures; // ���������
	CPlanet         mercury, venus, earth, // ˮ�ǣ� ���ǣ� ����
		           mars, jupiter, saturn, // ���ǣ� ľ�ǣ� ����
				  uranus, neptune, pluto; // ������,������,ڤ����
	GlobalPosition        globalPosition; // ȫ�ֿ��Ʊ���
	GLfloat             m_fRotationSpeed; // ��ת�ٶ�
	/////////////////////////////////////////////////////////////////
	BOOL RenderScene();
	BOOL SetupPixelFormat(void);
	void SetLogicalPalette(void);
	BOOL InitializeOpenGL(CDC* pDC);
	void DrawSun();                       // ��̫��
	void DrawAllOrbits();                 // �����й��
	void DrawOrbit(CPlanet planet);        // ���������
	void DrawAllPlanets();                // ����������
	void DrawPlanet(CPlanet planet);       // ����������
	void DrawComet();                     // ������
	void init(void);                      // ��ʼ��
	void InitPlanets();                   // �������ʼ��
	void rotate();                        // ��ת

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCtrlPause();
	afx_msg void OnUpdateCtrlPause(CCmdUI *pCmdUI);
	afx_msg void OnChangeRotateSpeed();
};

#ifndef _DEBUG  // SolarView.cpp �еĵ��԰汾
inline CSolarDoc* CSolarView::GetDocument() const
   { return reinterpret_cast<CSolarDoc*>(m_pDocument); }
#endif

