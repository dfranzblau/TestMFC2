Notes during the development of the Reversi demo, documenting approach


- considered objects, actions, and conditions (outlined these on paper to start):
	Board: n x n array of cells ( 8 x 8 )
	Cell: 3 states (empty, black, white)
	Current player : initially Black, alternates between Black and White after player has a turn
	Move: 
		cell must be empty 
		cell must have neighbor cell occupied by other player
		cell "beyond" occupied neighbor must be occupied by same player
	Direction:
		8 possible "beyond" directions from a cell (N, NE, E, SE, S, SW, W, NW)
		if cell on edge of board not all directions are available.
	Game:
		initialize board to default starting positions
		initalize score
		black is current player
		while(either player has valid moves)
		{
			if(current player has valid move)
			{
				do(get next move for current player)
					while(move not valid and still playing)
				update board with given move
					increment player's score
				reverse pieces affected by move
					increase player's score, decrease other player's score
				redisplay updated board and scores.
			}
			switch current player
		}
		display game over message

		-- need to reorganize the above to fit in MFC orchestrated click handling (see OnLButtonDown in the view class)

- created an MFC app from template for SDI application
	- reviewed MFC app components

- considered classes for game, display, and app (keeping MFC in mind)
	game board class for maintaining game state
	view class for displaying and interacting with board
	document class to manage a board instance
	
- considered data structures for board and pieces
	board could be 2D array of values, need three values (empty, white, black)
		could use byte, use int to make it simple and because only 64 will be allocated and space is not critical
	define 3 player values using enum with values 0, 1, 2 for 
		these can be used to index into arrays (e.g. the images for rendering cells)
	define array of three images for rendering board cells, 
		initialize these once from files on disk (or could embed as resources)
		
- wrote board initialization

- wrote board display using inital placement of pieces
	to verify and tune the images and board rendering

- wrote status/score display

- wrote routine to make a valid move

	- need a test for IsValidMove for any cell.
		- test multiple conditions for each cell
		- order tests so that the easiest to test that will eliminate the most are tested first (fail fast)
		- stop on the first failed test for a cell
		- find all flips and remember them so move can be taken later
			- use 2D array of bools
				- other possible structures: dynamic array, bit map, array of ints

	- wrote coordinates valid test
	- wrote player valid test
	- wrote cell empty test
	- wrote directional search that remembers possible flips
		- if any flips found, then 
	
	- wrote directional walk and flip while other player

- wrote user input to make a move

- determine when game is over:
	- over if no valid moves remain for either player
	- algorithm ideas for how to test for valid moves remaining
		- brute force search each time (not most efficient, but since set is small it may be efficient enough)
			- could focus the search on obviously possible cells
				(e.g. must have neighbor occupied by other color, etc.)
			- test obvious cases early (fail fast when testing if cell is a vaild move)
		- algorithm to walk the boundary of the occupied cells, and limit search to these
			- rules are such that there would be no islands of cells occupied;
			- probably less efficient that brute force that fails fast
		- maintain a list of empty cells, no sense checking those we know are not empty
			- only saves the first test in the brute force approach, won't have much impact since set is small


	- stop testing on first valid cell found - if at least one is found then game not over

- test and debug as required

- clean up code
	