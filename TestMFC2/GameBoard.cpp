#include "StdAfx.h"
#include "GameBoard.h"


CGameBoard::CGameBoard(void)
: gameboard(NULL)
{
	InitBoard();
}

CGameBoard::~CGameBoard(void)
{
  DeleteBoard();
}

void CGameBoard::InitBoard(void)
{
  if(gameboard == NULL)
	CreateBoard();

  //  init cells to empty
  for (int row = 0; row < boardsize; row++)
  {
	for (int col = 0; col < boardsize; col++)
	{
		gameboard[row][col] = PlayerNone;
	}
  }

  // set default starting discs for Black and White
  CPoint point;
  for (int i = 0; i < m_numStart; i++)
  {
	  point = m_startBlack[i];
	  gameboard[point.y][point.x] = PlayerBlack;

	  point = m_startWhite[i];
	  gameboard[point.y][point.x] = PlayerWhite;
  }

  score[PlayerWhite] = 2;				// init game state
  score[PlayerBlack] = 2;
  current_player = PlayerBlack;
}

int CGameBoard::GetCell(int row, int col)
{
  if(row < 0 || row >= boardsize || col < 0 || col >= boardsize)	// bounds test
    return PlayerNone;
  return gameboard[row][col];
}

CImage* CGameBoard::GetImage(int player)
{
	if (player < 0 || player > PlayerMAX)	// bounds test
		return NULL;
	return cellimages[player];
}


CString CGameBoard::GetStatus()
{
	CString move;
	playernames.Lookup(current_player, move);
	if (IsGameOver())
		boardstatus.Format(_T("GAME OVER    Black %d   White %d"), score[PlayerBlack], score[PlayerWhite]);
	else
		boardstatus.Format(_T("Move: %s    Black %d   White %d"), move, score[PlayerBlack], score[PlayerWhite]);

	return boardstatus;
}

bool CGameBoard::IsValidMove(int row, int col)
{
	return IsValidMove(current_player, row, col);
}


bool CGameBoard::IsValidMove(int player, int row, int col)
{
	// return false if move was not valid,
	// remember flips for this move if we want to make the move afterward

	// perform the fast checks for valid move:

	// if game is over, return false
	if (IsGameOver())
		return false;

	// row and column must be valid coordinates
	if (!IsValidCoordinates(row, col))
		return false;

	// player must be valid
	if (player != PlayerBlack && player != PlayerWhite)
		return false;

	// cell must be empty
	if (gameboard[row][col] != PlayerNone)
		return false;

	// cell must have neighbor of other color, and distant neighbor in same direction of my color

	int other = OtherPlayer(player);

	// search for neighbor of other color that is between this move and existing cell owned by player
	// if found, then current move is valid: make move for player and reverse in-between cells owned by other
	bool done;
	int nrow, ncol;
	int testrow, testcol, testcell;
	int tweencount;		// count of potential flips traversed while walking board in a direction
	bool valid = false;
	ClearFlips();				// start with no flips marked
	for (int i = 0; i < 8; i++)	// the 8 possible directions from a cell 
	{
		// generate coordinates of a neighbor cell, and skip if it is not on the gameboard
		nrow = row + m_directions[i].y;	// neighbor row
		ncol = col + m_directions[i].x;	// neighbor column
		if (!IsValidCoordinates(nrow, ncol))
			continue;

		if (gameboard[nrow][ncol] != other)	// if neighbor is not held by other player, keep looking
			continue;

		testrow = nrow;	// testrow and testcol "walk" past a neighbor in same direction to test what's beyond
		testcol = ncol;	
		tweencount = 0;

		tweenrows[tweencount] = testrow;	// cache the coords of the 'tween' chip found and then walk until my color found
		tweencols[tweencount] = testcol;
		tweencount++;
		done = false;

		do
		{
			// generate new coordinates along direction of neighbor
			testrow += m_directions[i].y;
			testcol += m_directions[i].x;

			// if test coords are not valid then done
			if (!IsValidCoordinates(testrow, testcol))
			{
				done = true;
				break;
			}

			testcell = gameboard[testrow][testcol];

			// if cell at test coords is not occupied then done
			if (testcell == PlayerNone)
			{
				done = true;
				break;
			}

			// if cell at test coords is occupied by other then save between and continue walking
			if (testcell == other)
			{
				tweenrows[tweencount] = testrow;	// cache the coords of the 'tween' chip found
				tweencols[tweencount] = testcol;
				tweencount++;
				continue;
			}

			// else cell at test coords is occupied by self, indicating in-betweens are flip-worthy
			MarkFlips(tweencount);
			valid = true;	// flips were found; move is valid

			done = true;

		} while ( !done );
	}

	return valid;
}

void CGameBoard::MarkFlips(int count)
{
	for (int i = 0; i < count; i++)
	{
		flips[tweenrows[i]][tweencols[i]] = true;
	}
}

void CGameBoard::ClearFlips(void)
{
	for (int row = 0; row < boardsize; row++)
		for (int col = 0; col < boardsize; col++)
			flips[row][col] = false;
}

int CGameBoard::OtherPlayer(int player)
{
	return player == PlayerBlack ? PlayerWhite : PlayerBlack;
}

void CGameBoard::ReverseMarkedCells(void)
{
	int count = 0;
	for (int row = 0; row < boardsize; row++)
	{
		for (int col = 0; col < boardsize; col++)
		{
			if (flips[row][col])
			{
				gameboard[row][col] = current_player;
				count++;	// count number of flips made 
			}
		}
	}
	score[current_player] += count;				// maintain count for each
	score[OtherPlayer(current_player)] -= count;
}

bool CGameBoard::IsValidCoordinates(int row, int col)
{
	if (row < 0 || col < 0 || row >= boardsize || col >= boardsize)
		return false;
	return true;
}

bool CGameBoard::IsGameOver()
{
	// game is over when no valid moves for either player
	return gameover;
}

void CGameBoard::MakeMove(int row, int col)
{
	// note: assumes that move has been tested for validity already

	gameboard[row][col] = current_player;	// set cell for current player
	score[current_player] += 1;				// current player score increases for new cell

	ReverseMarkedCells();	// reverse using cache of flips found, updates score on every flip
	

	// (this is brute-force approach with fail-fast tests in IsValidMove)
	// check whether either player has valid moves remaining
	//		if not, then game over
	current_player = OtherPlayer(current_player);	// set play to other player and test for valid move
	for (int row = 0; row < boardsize; row++)
		for (int col = 0; col < boardsize; col++)
			if (IsValidMove(current_player, row, col))	// has at least one valid move, so continue play with other player
				return;

	current_player = OtherPlayer(current_player);	// set play back to first player and test for valid move
	for (int row = 0; row < boardsize; row++)
		for (int col = 0; col < boardsize; col++)
			if (IsValidMove(current_player, row, col))	// has at least one valid move, so continue play with first player
				return;

	// didn't find a valid move, so mark game over	// no valid moves remain so game is over
	gameover = true;
}


void CGameBoard::DeleteBoard(void)
{
  if(gameboard != NULL)	//  must have a board
  {
    for(int row = 0; row < boardsize; row++)
    {
      if(gameboard[row] != NULL)
      {        
        delete [] gameboard[row];	// delete array of cells
		gameboard[row] = NULL;
      }
    }    
    delete [] gameboard;		// delete array of rows
	gameboard = NULL;
  }

  if (cellimages != NULL)
  {
	  for (int i = 0; i <= PlayerMAX; i++)
		  delete cellimages[i];
	  delete[] cellimages;		// delete array of cell images
	  cellimages = NULL;
  }
}

void CGameBoard::CreateBoard(void)
{
	//  If there is already a board, delete it
	if (gameboard != NULL)
		DeleteBoard();

	gameboard = new int*[boardsize];	//  allocate row array

	//  Create each row
	for (int row = 0; row < boardsize; row++)
	{
		gameboard[row] = new int[boardsize];	// allocate columns in row

		for (int col = 0; col < boardsize; col++)
			gameboard[row][col] = 0;
	}

	// load cell images
	HRESULT hresult;
	cellimages = new CImage*[3];
	cellimages[PlayerNone] = new CImage();
	cellimages[PlayerBlack] = new CImage();
	cellimages[PlayerWhite] = new CImage();
	hresult = cellimages[PlayerNone]->Load(_T("img\\Empty.png"));
	hresult = cellimages[PlayerBlack]->Load(_T("img\\Black4.png"));
	hresult = cellimages[PlayerWhite]->Load(_T("img\\White4.png"));

	// init player names for status display
	playernames.SetAt(PlayerNone, CString("None"));
	playernames.SetAt(PlayerBlack, CString("Black"));
	playernames.SetAt(PlayerWhite, CString("White"));

}
