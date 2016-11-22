
// TestMFC2Doc.h : interface of the CTestMFC2Doc class
//


#pragma once

#include "GameBoard.h"

class CTestMFC2Doc : public CDocument
{
protected: // create from serialization only
	CTestMFC2Doc();
	DECLARE_DYNCREATE(CTestMFC2Doc)

// Attributes
public:

// Operations
public:
	int GetCell(int row, int col)
	{
		return m_board.GetCell(row, col);
	}

	CImage* GetImage(int player)
	{
		return m_board.GetImage(player);
	}

	void SetupBoard(void) { m_board.InitBoard(); }
	int GetWidth(void) { return m_board.GetWidth(); }
	int GetHeight(void) { return m_board.GetHeight(); }
	int GetColumns(void) { return m_board.GetColumns(); }
	int GetRows(void) { return m_board.GetRows(); }
	void DeleteBoard(void) { m_board.DeleteBoard(); }
	CString GetStatus(void) { return m_board.GetStatus(); }
	bool IsGameOver(void) { return m_board.IsGameOver(); }
	bool IsValidMove(int row, int col) { return m_board.IsValidMove(row, col); }
	void MakeMove(int row, int col) { m_board.MakeMove(row, col); }

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CTestMFC2Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:	
	CGameBoard m_board;	// Instance of the game board

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
