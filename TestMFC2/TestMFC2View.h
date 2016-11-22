
// TestMFC2View.h : interface of the CTestMFC2View class
//

#pragma once


class CTestMFC2View : public CView
{
protected: // create from serialization only
	CTestMFC2View();
	DECLARE_DYNCREATE(CTestMFC2View)

// Attributes
public:
	CTestMFC2Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:


// Implementation
public:
	virtual ~CTestMFC2View();
	void ResizeWindow();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // debug version in TestMFC2View.cpp
inline CTestMFC2Doc* CTestMFC2View::GetDocument() const
   { return reinterpret_cast<CTestMFC2Doc*>(m_pDocument); }
#endif

