#include "pch.h"
#include "RainbowBlocksBoard.h"

CRainbowBlocksBoard::CRainbowBlocksBoard(void)
	:
	m_arrBoard(nullptr),
	m_nRows(15),
	m_nColumns(15),
	m_nHeight(35),
	m_nWidth(35),
	m_nBlocksRemaining(0),
	m_nColors(3)
{
	m_arrColors[0] = RGB(0, 0, 0);
	m_arrColors[1] = RGB(255, 0, 0);
	m_arrColors[2] = RGB(255, 140, 0);
	m_arrColors[3] = RGB(255, 255, 0);
	m_arrColors[4] = RGB(0, 255, 0);
	m_arrColors[5] = RGB(0, 191, 255);
	m_arrColors[6] = RGB(0, 0, 255);
	m_arrColors[7] = RGB(170, 0, 170);

	CreateBoard();
}

CRainbowBlocksBoard::CRainbowBlocksBoard(const CRainbowBlocksBoard& board)
{
	m_arrBoard = nullptr;
	m_nRows = board.m_nRows;
	m_nColumns = board.m_nColumns;
	m_nHeight = board.m_nHeight;
	m_nWidth = board.m_nWidth;
	m_nBlocksRemaining = board.m_nBlocksRemaining;
	m_nColors = board.m_nColors;

	for (int iii = 0; iii < 8; ++iii)
		m_arrColors[iii] = board.m_arrColors[iii];

	CreateBoard();

	for (int row = 0; row < m_nRows; ++row)
	{
		for (int col = 0; col < m_nColumns; ++col)
		{
			m_arrBoard[row][col] = board.m_arrBoard[row][col];
		}
	}
}

CRainbowBlocksBoard::~CRainbowBlocksBoard(void)
{
	DeleteBoard();
}

void CRainbowBlocksBoard::CreateBoard(void)
{
	if (m_arrBoard != nullptr)
		DeleteBoard();

	m_arrBoard = new int* [m_nRows];

	for (int row = 0; row < m_nRows; ++row)
	{
		m_arrBoard[row] = new int[m_nColumns];

		for (int col = 0; col < m_nColumns; ++col)
			m_arrBoard[row][col] = 0;
	}
}

void CRainbowBlocksBoard::SetupBoard(void)
{
	if (m_arrBoard == nullptr)
		CreateBoard();

	for (int row = 0; row < m_nRows; ++row)	
		for (int col = 0; col < m_nColumns; ++col)		
			m_arrBoard[row][col] = (rand() % m_nColors) + 1;

	m_nBlocksRemaining = m_nRows * m_nColumns;
}

void CRainbowBlocksBoard::DeleteBoard(void)
{
	if (m_arrBoard != nullptr)
	{
		for (int row = 0; row < m_nRows; ++row)
		{
			if (m_arrBoard != nullptr)
			{
				delete[] m_arrBoard[row];
				m_arrBoard[row] = nullptr;
			}
		}
		delete[] m_arrBoard;
		m_arrBoard = nullptr;
	}
}

COLORREF CRainbowBlocksBoard::GetBoardSpace(int row, int col) const
{
	if ((row < 0 || row >= m_nRows) || (col < 0 || col >= m_nColumns))
		return m_arrColors[0];

	return m_arrColors[m_arrBoard[row][col]];
}

int CRainbowBlocksBoard::DeleteBlocks(int row, int col)
{
	if (row < 0 || row >= m_nRows || col < 0 || col >= m_nColumns)
		return -1;

	int nColor = m_arrBoard[row][col];
	if (nColor == 0)
		return -1;

	// Adjoining blocks have same nColor
	int nCount = -1;
	if ((row - 1 >= 0 && m_arrBoard[row - 1][col] == nColor) ||
		(row + 1 < m_nRows && m_arrBoard[row + 1][col] == nColor) ||
		(col - 1 >= 0 && m_arrBoard[row][col - 1] == nColor) ||
		(col + 1 < m_nColumns && m_arrBoard[row][col + 1] == nColor))
	{
		// Delete current...
		m_arrBoard[row][col] = 0;
		nCount = 1;

		// ...and surrounding
		nCount +=
			DeleteNeighborBlocks(row, col - 1, nColor, Direction::FROM_RIGHT);
		nCount += 
			DeleteNeighborBlocks(row, col + 1, nColor, Direction::FROM_LEFT);
		nCount +=
			DeleteNeighborBlocks(row - 1, col, nColor, Direction::FROM_ABOVE);
		nCount +=
			DeleteNeighborBlocks(row + 1, col, nColor, Direction::FROM_DOWN);

		m_nBlocksRemaining -= nCount;

		CompactBoard();
	}
	return nCount;
}

int CRainbowBlocksBoard::DeleteNeighborBlocks (int row, int col, 
	int color, Direction direction)
{
	if (row < 0 || row >= m_nRows || col < 0 || col >= m_nColumns)
		return 0;
	if (m_arrBoard[row][col] != color)
		return 0;

	int count = 1;
	m_arrBoard[row][col] = 0;

	if (direction != Direction::FROM_RIGHT)
		count += DeleteNeighborBlocks(row, col + 1, color, Direction::FROM_LEFT);
	if (direction != Direction::FROM_LEFT)
		count += DeleteNeighborBlocks(row, col - 1, color, Direction::FROM_RIGHT);
	if (direction != Direction::FROM_ABOVE)
		count += DeleteNeighborBlocks(row + 1, col, color, Direction::FROM_DOWN);
	if (direction != Direction::FROM_DOWN)
		count += DeleteNeighborBlocks(row - 1, col, color, Direction::FROM_ABOVE);

	return count;
}

void CRainbowBlocksBoard::CompactBoard(void)
{
	// Vertical compression
	for (int col = 0; col < m_nColumns; ++col)
	{
		int nEmptyRow = m_nRows - 1;
		int nOccupiedRow = nEmptyRow;
		while (nEmptyRow >= 0 && nOccupiedRow >= 0)
		{
			while (nEmptyRow >= 0 &&
				m_arrBoard[nEmptyRow][col] != 0)
			{
				--nEmptyRow;
			}
			if (nEmptyRow >= 0)
			{
				nOccupiedRow = nEmptyRow - 1;
				while (nOccupiedRow >= 0 &&
					m_arrBoard[nOccupiedRow][col] == 0)
				{
					--nOccupiedRow;
				}
				if (nOccupiedRow >= 0)
				{
					m_arrBoard[nEmptyRow][col] =
						m_arrBoard[nOccupiedRow][col];
					m_arrBoard[nOccupiedRow][col] = 0;
				}
			}
		}
	}
	// Horizontal compression
	int nEmptyColumn = 0;
	int nOccupiedColumn = nEmptyColumn;
	while (nEmptyColumn < m_nColumns && nOccupiedColumn < m_nColumns)
	{
		while (nEmptyColumn < m_nColumns && 
			m_arrBoard[m_nRows - 1][nEmptyColumn] != 0)
		{
			++nEmptyColumn;
		}
		if (nEmptyColumn < m_nColumns)
		{
			nOccupiedColumn = nEmptyColumn + 1;
			while (nOccupiedColumn < m_nColumns && 
				m_arrBoard[m_nRows - 1][nOccupiedColumn] == 0)
			{
				++nOccupiedColumn;
			}
			if (nOccupiedColumn < m_nColumns)
			{
				for (int row = m_nRows - 1; row >= 0; --row)
				{					
					if (m_arrBoard[row][nOccupiedColumn] == 0)
						break;
					
					m_arrBoard[row][nEmptyColumn] = 
						m_arrBoard[row][nOccupiedColumn];
					m_arrBoard[row][nOccupiedColumn] = 0;
				}
			}
		}
	}
}

bool CRainbowBlocksBoard::IsGameOver(void) const
{
	// Neighbors of the same nColor exist
	for (int col = 0; col < m_nColumns; col++)
	{
		for (int row = m_nRows - 1; row >= 0; row--)
		{
			int nColor = m_arrBoard[row][col];

			if (nColor == 0)
				break;
			
			if (row - 1 >= 0 && 
				m_arrBoard[row - 1][col] == nColor)
			{
				return false;
			}
			if (col + 1 < m_nColumns && 
				m_arrBoard[row][col + 1] == nColor)
			{
				return false;
			}
		}
	}
	return true;
}
