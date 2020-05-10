
// MyMFCApplicationView.h: CMyMFCApplicationView 类的接口
//

#pragma once


class CMyMFCApplicationView : public CView
{
protected: // 仅从序列化创建
	CMyMFCApplicationView() noexcept;
	DECLARE_DYNCREATE(CMyMFCApplicationView)

// 特性
public:
	CMyMFCApplicationDoc* GetDocument() const;

// 操作
public:
	int PointR = 5;
	CPoint GetPoints[100];
	int PointsNum = 0;

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
	virtual ~CMyMFCApplicationView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg
	void ShowText(CDC* pDC, CPoint p);
	void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnLButtonDown(UINT, CPoint);
	void DrawEllipse(CDC* pDC, CPoint point, int r);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCleanWin();
	afx_msg void OnDrawNBezier();
	void BezierN(CDC* pDC, CPoint* m_BezierN, int m_numBezierN);
	afx_msg void OnDrawThreeBezier();
	afx_msg void OnCleanCurve();
};

#ifndef _DEBUG  // MyMFCApplicationView.cpp 中的调试版本
inline CMyMFCApplicationDoc* CMyMFCApplicationView::GetDocument() const
   { return reinterpret_cast<CMyMFCApplicationDoc*>(m_pDocument); }
#endif

