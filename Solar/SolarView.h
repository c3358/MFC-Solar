// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://go.microsoft.com/fwlink/?LinkId=238214。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// SolarView.h : CSolarView 类的接口
//

#pragma once
//包含文件、结构体及宏定义
#include "Texture.h"

#define MAXTEXTURE			    9
#define PI						3.141592654
#define DEG2RAD(angle)			angle * (PI / 180)	// 将角度转换为弧度

// 结构体：星球
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
protected: // 仅从序列化创建
	CSolarView();
	DECLARE_DYNCREATE(CSolarView)

// 特性
public:
	CSolarDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CSolarView();

public:
	// 添加成员函数与成员变量
	HGLRC		                   m_hRC; // OpenGL绘制描述表
	HPALETTE	              m_hPalette; // OpenGL调色板
	CDC*	                       m_pDC; // OpenGL设备描述表

	GLuint    m_nTextures_id[MAXTEXTURE]; // 纹理标号数组
	int           iCometAngle, iCometPos; // 
	CTextures                 m_Textures; // 纹理类对象
	CPlanet         mercury, venus, earth, // 水星， 金星， 地球
		           mars, jupiter, saturn, // 火星， 木星， 土星
				  uranus, neptune, pluto; // 天王星,海王星,冥王星
	GlobalPosition        globalPosition; // 全局控制变量
	GLfloat             m_fRotationSpeed; // 旋转速度
	/////////////////////////////////////////////////////////////////
	BOOL RenderScene();
	BOOL SetupPixelFormat(void);
	void SetLogicalPalette(void);
	BOOL InitializeOpenGL(CDC* pDC);
	void DrawSun();                       // 画太阳
	void DrawAllOrbits();                 // 画所有轨道
	void DrawOrbit(CPlanet planet);        // 画单个轨道
	void DrawAllPlanets();                // 画所有星球
	void DrawPlanet(CPlanet planet);       // 画单个星球
	void DrawComet();                     // 画彗星
	void init(void);                      // 初始化
	void InitPlanets();                   // 对星球初始化
	void rotate();                        // 旋转

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
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

#ifndef _DEBUG  // SolarView.cpp 中的调试版本
inline CSolarDoc* CSolarView::GetDocument() const
   { return reinterpret_cast<CSolarDoc*>(m_pDocument); }
#endif

