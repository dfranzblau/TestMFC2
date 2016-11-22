
// TestMFC2View.cpp : implementation of the CTestMFC2View class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "TestMFC2.h"
#endif

#include "TestMFC2Doc.h"
#include "TestMFC2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestMFC2View

IMPLEMENT_DYNCREATE(CTestMFC2View, CView)
BEGIN_MESSAGE_MAP(CTestMFC2View, CView)
	ON_WM_LBUTTONDOWN()
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CTestMFC2View construction/destruction

CTestMFC2View::CTestMFC2View()
{
}

CTestMFC2View::~CTestMFC2View()
{
}

BOOL CTestMFC2View::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

void CTestMFC2View::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	ResizeWindow();
}

void CTestMFC2View::OnDraw(CDC* pDC)
{
	CTestMFC2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	int nDCSave = pDC->SaveDC();// save device context

	CRect rcClient;
	GetClientRect(&rcClient);

	// setup white pen for drawing borders between cells
	CBrush br;
	br.CreateStockObject(WHITE_PEN);
	CBrush* pbrOld = pDC->SelectObject(&br);

	// draw the cells for board state
	int clr;
	CImage* image;
	CRect rcBlock;
	int nrows = pDoc->GetRows();
	int ncols = pDoc->GetColumns();
	for (int row = 0; row < nrows; row++)
	{
		for (int col = 0; col < ncols; col++)
		{
			rcBlock.top = row * pDoc->GetHeight();	// size and position of cell
			rcBlock.left = col * pDoc->GetWidth();
			rcBlock.right = rcBlock.left + pDoc->GetWidth();
			rcBlock.bottom = rcBlock.top + pDoc->GetHeight();

			clr = pDoc->GetCell(row, col);			// black, white, or empty
			image = pDoc->GetImage(clr);
			image->Draw(*pDC, rcBlock.left, rcBlock.top);

			pDC->FrameRect(&rcBlock, &br);			// draw white outline
		}
	}
	
	pDC->RestoreDC(nDCSave);	// restore device context
	br.DeleteObject();
}

void CTestMFC2View::ResizeWindow()
{
	CTestMFC2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// client area and window
	CRect rcClient, rcWindow;
	GetClientRect(&rcClient);
	GetParentFrame()->GetWindowRect(&rcWindow);

	int nWidthDiff = rcWindow.Width() - rcClient.Width();
	int nHeightDiff = rcWindow.Height() - rcClient.Height();

	rcWindow.right = rcWindow.left +
		pDoc->GetWidth() * pDoc->GetColumns() + nWidthDiff;
	rcWindow.bottom = rcWindow.top +
		pDoc->GetHeight() * pDoc->GetRows() + nHeightDiff + 24; // allow for height of status control

	GetParentFrame()->MoveWindow(&rcWindow);
}

void CTestMFC2View::OnLButtonDown(UINT nFlags, CPoint point)
{
	CTestMFC2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (pDoc->IsGameOver())
		return;

	// get row and column of cell clicked on
	int row = point.y / pDoc->GetHeight();
	int col = point.x / pDoc->GetWidth();

	if (!pDoc->IsValidMove(row, col))
		return;	// TODO: if move was not valid then message or beep or something

	pDoc->MakeMove(row, col);

	// else a valid move was made so refresh display
	Invalidate();
	UpdateWindow();

	// call base method
	CView::OnLButtonDown(nFlags, point);
}





// CTestMFC2View diagnostics

#ifdef _DEBUG
void CTestMFC2View::AssertValid() const
{
	CView::AssertValid();
}

void CTestMFC2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestMFC2Doc* CTestMFC2View::GetDocument() const 
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestMFC2Doc)));
	return (CTestMFC2Doc*)m_pDocument;
}
#endif //_DEBUG

