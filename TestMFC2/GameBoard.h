#pragma once


class CGameBoard
{
public:
  //  Default Constructor
	CGameBoard(void);

  //  Destructor
  ~CGameBoard(void);

  void InitBoard(void);

  //  Accessor functions
  int GetWidth(void) const { return cell_width; }
  int GetHeight(void) const { return cell_height; }
  int GetColumns(void) const { return boardsize; }
  int GetRows(void) const { return boardsize; }
  int GetCell(int row, int col);
  CImage* GetImage(int player);
  CString GetStatus();

  
  void DeleteBoard(void);	// Delete the board and free memory
  bool IsGameOver(void);
  //bool MovesAvailable(void);
  bool IsValidMove(int row, int col);
  bool IsValidMove(int player, int row, int col);
  void MakeMove(int row, int col);


private:
  void CreateBoard(void);
  bool IsValidCoordinates(int row, int col);
  int OtherPlayer(int player);		// return the other player for given player
  void ReverseMarkedCells(void);	// reverse  cells that have been marked for flipping
  void MarkFlips(int count);
  void ClearFlips(void);

  //  List of colors, 0 is background and 1-2 are piece colors (see GamePiece enum below)
  static COLORREF m_Colors[3];

  //  Board size information
  static const int boardsize = 8;
  int cell_height = 80;
  int cell_width = 80;

  int** gameboard;			// 2D array of game board cells [row][col]
  CImage** cellimages;		// array of images for painting cells (empty, black, white)
  int current_player;		// player whose move it is
  int score[2];				// indexed by Player enum value for black or white
  bool gameover = false;	// updated after each move; changes status display and click processing
  CString boardstatus;
  CMap<int, int, CString, CString> playernames;
  
  // decided on fixed arrays for these rather than dynamic allocation during move processing, 
  //	these get reused for each move, flips is cleared at start of processing a move
  int tweenrows[boardsize -2];	// for accumulating coordiantes of cells to reverse when walking board in a direction, 
  int tweencols[boardsize -2];	//		first and last must be one color, leaving at most 6 in between of other color
  bool flips[boardsize][boardsize];	// hold all flips found when testing for valid move

  const CPoint m_startWhite[2] = { CPoint(3,3), CPoint(4,4) };	// default starting placement for white and black discs
  const CPoint m_startBlack[2] = { CPoint(3,4), CPoint(4,3) };
  const int m_numStart = 2;	// number of starting pieces of each color

  // the 8 directions of cells that could surround a cell (cells on edge of board would not have neighbors in all directions)
  const CPoint m_directions[8] = { CPoint(-1,-1), CPoint(0,-1), CPoint(1,-1), CPoint(-1,0), CPoint(1,0), CPoint(-1,1), CPoint(0,1), CPoint(1,1) }; 
};

enum Player
{
	PlayerBlack = 0,
	PlayerWhite = 1,
	PlayerNone = 2,
	PlayerMAX = 2
};
