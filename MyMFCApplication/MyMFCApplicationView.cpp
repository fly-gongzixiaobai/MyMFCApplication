
// MyMFCApplicationView.cpp: CMyMFCApplicationView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MyMFCApplication.h"
#endif

#include "MyMFCApplicationDoc.h"
#include "MyMFCApplicationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMyMFCApplicationView

IMPLEMENT_DYNCREATE(CMyMFCApplicationView, CView)

BEGIN_MESSAGE_MAP(CMyMFCApplicationView, CView)
	// 标准打印命令
	ON_WM_LBUTTONDOWN() //鼠标点击
	ON_WM_SIZE() //窗口大小变化
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMyMFCApplicationView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_32772, &CMyMFCApplicationView::OnCleanWin)
	ON_COMMAND(ID_32771, &CMyMFCApplicationView::OnDrawNBezier)
	ON_COMMAND(ID_Menu, &CMyMFCApplicationView::OnDrawThreeBezier)
	ON_COMMAND(ID_32773, &CMyMFCApplicationView::OnCleanCurve)
END_MESSAGE_MAP()

// CMyMFCApplicationView 构造/析构

CMyMFCApplicationView::CMyMFCApplicationView() noexcept
{
	// TODO: 在此处添加构造代码

}

CMyMFCApplicationView::~CMyMFCApplicationView()
{
}

BOOL CMyMFCApplicationView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMyMFCApplicationView 绘图

void CMyMFCApplicationView::OnDraw(CDC* pDC/*pDC*/)
{
	CMyMFCApplicationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	GetClientRect(&rect);//获得客户区的大小
	pDC->SetMapMode(MM_ANISOTROPIC);//pDC自定义坐标系
	pDC->SetWindowExt(rect.Width(), rect.Height());//设置窗口范围
	pDC->SetViewportExt(rect.Width(), -rect.Height());//x轴水平向右，y轴垂直向上
	pDC->SetViewportOrg(rect.Width() / 2, rect.Height() / 2);//屏幕中心为原点

	if (flag == TRUE) {
		Repaint(pDC,rect);

	}
	DrawObject(pDC);
}

void CMyMFCApplicationView::Repaint(CDC* pDC, CRect rect) {
	if (PointsNum != 0) {
		GetClientRect(&rect);
		pDC->SetMapMode(MM_ANISOTROPIC);//设置映射模式
		pDC->SetWindowExt(rect.Width(), rect.Height());//设置窗口
		pDC->SetViewportExt(rect.Width(), -rect.Height());//设置视区:x轴水平向右，y轴垂直向上
		pDC->SetViewportOrg(rect.Width() / 2, rect.Height() / 2);//客户区中心为坐标系原点

		//处理第一个点
		pDC->MoveTo(GetPoints[0]);
		ShowText(pDC, GetPoints[0]);
		DrawEllipse(pDC, GetPoints[0], PointR);
		//处理剩余点
		for (int i = 1; i < PointsNum; i++) {
			ShowText(pDC, GetPoints[i]);
			DrawEllipse(pDC, GetPoints[i], PointR);
			pDC->LineTo(GetPoints[i]);
		}

		if (Bezier3) OnDrawThreeBezier();
		if (Bezier_n) BezierN(pDC, GetPoints, PointsNum - 1);

	}
}
void CMyMFCApplicationView::DrawObject(CDC* pDC)
{
	CPen NewPen, * pOldPen;
	NewPen.CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
	pOldPen = pDC->SelectObject(&NewPen);
	pDC->MoveTo(-rect.right / 2, 0);//绘制x轴
	pDC->LineTo(rect.right / 2, 0);
	pDC->MoveTo(rect.right / 2 - 10, 5);//绘制右箭头　
	pDC->LineTo(rect.right / 2, 0);
	pDC->LineTo(rect.right / 2 - 10, -5);
	pDC->LineTo(rect.right / 2, 0);
	pDC->TextOut(rect.right / 2 - 20, 30, (CString)"x");
	pDC->MoveTo(0, -rect.bottom / 2);//绘制y轴
	pDC->LineTo(0, rect.bottom / 2);
	pDC->MoveTo(-5, rect.bottom / 2 - 10);//绘制上箭头
	pDC->LineTo(0, rect.bottom / 2);
	pDC->LineTo(5, rect.bottom / 2 - 10);
	pDC->LineTo(0, rect.bottom / 2);
	pDC->TextOut(-30, rect.bottom / 2 - 20, (CString)"y");
	pDC->TextOut(-20, -10, (CString)"O");
	CString data;
	data.Format(_T("收集到 %d 个点"), PointsNum);
	pDC->TextOut(-rect.right / 2, rect.bottom / 2, data);
	ShowPoints(pDC, rect);
}
void CMyMFCApplicationView::DoubleBuffer()//双缓冲
{
	CDC* pDC = GetDC();
	GetClientRect(&rect);//获得客户区的大小
	pDC->SetMapMode(MM_ANISOTROPIC);//pDC自定义坐标系
	pDC->SetWindowExt(rect.Width(), rect.Height());//设置窗口范围
	pDC->SetViewportExt(rect.Width(), -rect.Height());//x轴水平向右，y轴垂直向上
	pDC->SetViewportOrg(rect.Width() / 2, rect.Height() / 2);//屏幕中心为原点
	CDC MemDC;//内存DC
	CBitmap NewBitmap, * pOldBitmap;//内存中承载图像的临时位图
	MemDC.CreateCompatibleDC(pDC);//建立与屏幕pDC兼容的MemDC 
	NewBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());//创建兼容位图 
	pOldBitmap = MemDC.SelectObject(&NewBitmap); //将兼容位图选入MemDC 
	MemDC.FillSolidRect(&rect, pDC->GetBkColor());//按原来背景填充客户区，否则是黑色
	MemDC.SetMapMode(MM_ANISOTROPIC);//MemDC自定义坐标系
	MemDC.SetWindowExt(rect.Width(), rect.Height());
	MemDC.SetViewportExt(rect.Width(), -rect.Height());
	MemDC.SetViewportOrg(rect.Width() / 2, rect.Height() / 2);
	DrawObject(&MemDC);
	pDC->BitBlt(-rect.Width() / 2, -rect.Height() / 2, rect.Width(), rect.Height(), &MemDC, -rect.Width() / 2, -rect.Height() / 2, SRCCOPY);//将内存位图拷贝到屏幕
	MemDC.SelectObject(pOldBitmap);//恢复位图
	NewBitmap.DeleteObject();//删除位图
	ReleaseDC(pDC);//释放DC	
}
// CMyMFCApplicationView 打印
void CMyMFCApplicationView::ShowText(CDC* pDC, CPoint p){   //打印字符
	CString data;
	data.Format(_T("P%d(x=%d,y=%d)"),PointsNum, p.x, p.y);
	pDC->TextOut(p.x, p.y, data);
}

void CMyMFCApplicationView::ShowText(CDC* pDC, CPoint p, int num) {   //打印字符
	CString data;
	data.Format(_T("P%d(x=%d,y=%d)"), num, p.x, p.y);
	pDC->TextOut(p.x, p.y, data);
}

void CMyMFCApplicationView::ShowPoints(CDC* pDC, CRect rect) {   //打印字符
	CString data;
	CString data2;
	data.Format(_T("收集到 %d 个点"), PointsNum);
	pDC->TextOut(-rect.right / 2, rect.bottom / 2, data);
	for (int i = 0; i < PointsNum; i++) {
		data2.Format(_T("P%d(x=%d,y=%d)"), i, GetPoints[i].x, GetPoints[i].y);
		pDC->TextOut(-rect.right / 2, rect.bottom / 2 - 20 * (i + 1), data2);
	}
}

void CMyMFCApplicationView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMyMFCApplicationView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMyMFCApplicationView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMyMFCApplicationView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CMyMFCApplicationView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMyMFCApplicationView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

void CMyMFCApplicationView::OnLButtonDown(UINT, CPoint point)			//鼠标左键按下
{
	CRect rect;
	CDC* pDC = GetDC();
	GetClientRect(&rect);
	pDC->SetMapMode(MM_ANISOTROPIC);//设置映射模式
	pDC->SetWindowExt(rect.Width(), rect.Height());//设置窗口
	pDC->SetViewportExt(rect.Width(), -rect.Height());//设置视区:x轴水平向右，y轴垂直向上
	pDC->SetViewportOrg(rect.Width() / 2, rect.Height() / 2);//客户区中心为坐标系原点

	point.x =-(rect.Width() / 2 - point.x);
	point.y = rect.Height() / 2 - point.y;

	GetPoints[PointsNum] = point;
	PointsNum++;

	if (PointsNum > 1) {
		pDC->MoveTo(GetPoints[PointsNum-2]);
		pDC->LineTo(GetPoints[PointsNum-1]);
	}


	ShowText(pDC, point);
	ShowPoints(pDC, rect);
	DrawEllipse(pDC, point, PointR);

	//CString data;
	//data.Format(_T("x=%d,y=%d"), GetPoints[PointsNum].x, GetPoints[PointsNum].y);
	//pDC->TextOut(0, PointsNum*20, data);

	ReleaseDC(pDC);
}

void CMyMFCApplicationView::OnSize(UINT, int, int)
{
	flag = TRUE;

	Invalidate();  //更新

}

void CMyMFCApplicationView::DrawEllipse(CDC* pDC, CPoint p ,int r) {
	CBrush bruch, * oldbruch;
	bruch.CreateSolidBrush(RGB(0,0,0));
	oldbruch = pDC->SelectObject(&bruch);
	pDC->Ellipse(p.x - r, p.y - r, p.x + r, p.y + r);
	pDC->SelectObject(oldbruch);
}
// CMyMFCApplicationView 诊断

#ifdef _DEBUG
void CMyMFCApplicationView::AssertValid() const
{
	CView::AssertValid();
}

void CMyMFCApplicationView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMyMFCApplicationDoc* CMyMFCApplicationView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyMFCApplicationDoc)));
	return (CMyMFCApplicationDoc*)m_pDocument;
}
#endif //_DEBUG


// CMyMFCApplicationView 消息处理程序


void CMyMFCApplicationView::OnCleanWin()
{
	PointsNum = 0;
	RedrawWindow();//清屏
	// TODO: 在此添加命令处理程序代码
}


void CMyMFCApplicationView::OnDrawNBezier()
{
	CRect rect;
	CDC* pDC = GetDC();
	GetClientRect(&rect);
	pDC->SetMapMode(MM_ANISOTROPIC);//设置映射模式
	pDC->SetWindowExt(rect.Width(), rect.Height());//设置窗口
	pDC->SetViewportExt(rect.Width(), -rect.Height());//设置视区:x轴水平向右，y轴垂直向上
	pDC->SetViewportOrg(rect.Width() / 2, rect.Height() / 2);//客户区中心为坐标系原点

	//int n = PointsNum;
	//n = (n / 3) * 3 + 1;
	if (PointsNum == 0) return;
	//pDC->PolyBezier(GetPoints, n);
	BezierN(pDC, GetPoints, PointsNum-1);
	//Repaint(pDC, rect);
	Bezier = PointsNum;
	Bezier_n = TRUE;
	// TODO: 在此添加命令处理程序代码

}
long fac(int m) //阶乘
{
	int i;
	long temp = 1;
	if (m == 0)	return 1;
	else
	{
		for (i = 2; i <= m; i++)
			temp = temp * i;
	}
	return temp;
}

double powi(double v, int k) //v的K次方
{
	double temp = 1.0;
	if (k == 0 || v == 0)	return 1;
	else
	{
		for (int i = 1; i <= k; i++)
			temp = temp * v;
	}

	return temp;
}

void CMyMFCApplicationView::BezierN(CDC* pDC, CPoint* m_BezierN, int m_numBezierN)//绘制m_numBezierN阶贝塞尔曲线
{
	CPen Newpen1(PS_SOLID, 2, RGB(0, 0, 0));
	CPen* pOldPen1 = pDC->SelectObject(&Newpen1);

	int x, y, i, j, k = 50, a, b;
	double	t, t1, u, v;
	double temp, temp1, temp2, bi;
	t = 1.0 / k;
	a = m_BezierN[0].x;
	b = m_BezierN[0].y;
	//pDC->MoveTo(a,b);
	for (j = 1; j < k; j++)
	{
		pDC->MoveTo(a, b);
		t1 = j * t;
		u = t1; v = 1 - u;
		x = 0; y = 0;
		for (i = 0; i <= m_numBezierN; i++)
		{
			temp = double(fac(m_numBezierN) / fac(i) / fac(m_numBezierN - i));
			temp1 = powi(u, i);
			temp2 = powi(v, m_numBezierN - i);
			bi = temp * temp1 * temp2;
			x = x + bi * m_BezierN[i].x;
			y = y + bi * m_BezierN[i].y;
		}
		pDC->LineTo(x, y);
		a = x;
		b = y;
	}
	pDC->LineTo(m_BezierN[m_numBezierN].x, m_BezierN[m_numBezierN].y);
	pDC->SelectObject(pOldPen1);
}

void CMyMFCApplicationView::OnDrawThreeBezier()
{

	// TODO: 在此添加命令处理程序代码
	CRect rect;
	CDC* pDC = GetDC();
	GetClientRect(&rect);
	pDC->SetMapMode(MM_ANISOTROPIC);//设置映射模式
	pDC->SetWindowExt(rect.Width(), rect.Height());//设置窗口
	pDC->SetViewportExt(rect.Width(), -rect.Height());//设置视区:x轴水平向右，y轴垂直向上
	pDC->SetViewportOrg(rect.Width() / 2, rect.Height() / 2);//客户区中心为坐标系原点

	CPen Newpen1(PS_SOLID, 2, RGB(0, 0, 0));
	CPen* pOldPen1 = pDC->SelectObject(&Newpen1);

	if (PointsNum < 4) return; //点数小于4个不能画出贝塞尔曲线

	int n = PointsNum-1;
	n = (n / 3) * 3 + 1;		//找出适合的点数参数
	Bezier = 3;
	pDC->PolyBezier(GetPoints, n);
	Bezier3 = TRUE;
}


void CMyMFCApplicationView::OnCleanCurve()
{
	Bezier3 = FALSE;
	Bezier_n = FALSE;
	RedrawWindow();
	CRect rect;
	CDC* pDC = GetDC();
	GetClientRect(&rect);
	pDC->SetMapMode(MM_ANISOTROPIC);//设置映射模式
	pDC->SetWindowExt(rect.Width(), rect.Height());//设置窗口
	pDC->SetViewportExt(rect.Width(), -rect.Height());//设置视区:x轴水平向右，y轴垂直向上
	pDC->SetViewportOrg(rect.Width() / 2, rect.Height() / 2);//客户区中心为坐标系原点

	if (PointsNum == 0) return;
	//处理第一个点
	pDC->MoveTo(GetPoints[0]);
	ShowText(pDC, GetPoints[0], 1);
	DrawEllipse(pDC, GetPoints[0], PointR);
	//处理剩余点
	for (int i = 1; i < PointsNum; i++) {
		ShowText(pDC, GetPoints[i], i+1);
		DrawEllipse(pDC, GetPoints[i], PointR);
		pDC->LineTo(GetPoints[i]);
	}
	// TODO: 在此添加命令处理程序代码
}
