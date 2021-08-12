#pragma once

class CRainbowBlocksBoard
{
public:
	CRainbowBlocksBoard(void);
	CRainbowBlocksBoard(const CRainbowBlocksBoard& board);

	~CRainbowBlocksBoard(void);

	void SetupBoard(void);

	void DeleteBoard(void);

	int GetRows(void) const { return m_nRows; }
	int GetColumns(void) const { return m_nColumns; }
	int GetHeight(void) const { return m_nHeight; }
	int GetWidth(void) const { return m_nWidth; }
	int GetRemainingCount(void) const { return m_nBlocksRemaining; }
	int GetNumColors(void) const { return m_nColors; }
	COLORREF GetBoardSpace(int row, int col) const;

	void SetNumColors(int nColors)
	{
		m_nColors = (nColors >= 3 && nColors <= 7) ?
			nColors : m_nColors;
	}
	void SetRows(int nRows)
	{
		m_nRows = (nRows >= 5) ? nRows : 5;
	}
	void SetColumns(int nColumns)
	{
		m_nColumns = (nColumns >= 5) ? nColumns : 5;
	}
	void SetHeight(int nHeight)
	{
		m_nHeight = (nHeight >= 15) ? nHeight : 15;
	}
	void SetWidth(int nWidth)
	{
		m_nWidth = (nWidth >= 15) ? nWidth : 15;
	}

	int DeleteBlocks(int row, int col);

	bool IsGameOver(void) const;

private:
	void CreateBoard(void);

	enum class Direction
	{
		FROM_ABOVE,
		FROM_DOWN,
		FROM_LEFT,
		FROM_RIGHT
	};

	// Recursive
	int DeleteNeighborBlocks(int row, int col, 
		int color, Direction direction);

	void CompactBoard(void);

	int** m_arrBoard;
	int m_nRows;
	int m_nColumns;

	int m_nHeight;
	int m_nWidth;

	COLORREF m_arrColors[8];

	int m_nColors;

	int m_nBlocksRemaining;
};
