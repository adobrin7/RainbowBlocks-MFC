#pragma once

#include "RainbowBlocksBoard.h"

#include <stack>

class CRainbowBlocksDoc : public CDocument
{
protected:
	CRainbowBlocksDoc() noexcept;
	virtual ~CRainbowBlocksDoc();
	DECLARE_DYNCREATE(CRainbowBlocksDoc)
	DECLARE_MESSAGE_MAP()

// Override
public:
	virtual BOOL OnNewDocument();

// Implementation
public:
	void SetupBoard() { m_board->SetupBoard(); }

	void DeleteBoard() { m_board->DeleteBoard(); }

	int GetRows() const { return m_board->GetRows(); }
	int GetColumns() const { return m_board->GetColumns(); }
	int GetHeight() const { return m_board->GetHeight(); }
	int GetWidth() const { return m_board->GetWidth(); }
	int GetRemainingCount() const { return m_board->GetRemainingCount(); }
	int GetNumColors(void) const { return m_board->GetNumColors(); }
	COLORREF GetBoardSpace(int row, int col) const
	{ 
		return m_board->GetBoardSpace(row, col);
	}

	void SetNumColors(int nColors);
	void SetRows(int nRows) { m_board->SetRows(nRows); }
	void SetColumns(int nColumns) { m_board->SetColumns(nColumns); }
	void SetHeight(int nHeight) { m_board->SetHeight(nHeight); }
	void SetWidth(int nWidth) { m_board->SetWidth(nWidth); }

	int DeleteBlocks(int row, int col);

	bool CanUndo();
	bool CanRedo();

	void UndoLast();
	void RedoLast();

	bool IsGameOver() const { return m_board->IsGameOver(); }

protected:
	void ClearUndo();
	void ClearRedo();

	CRainbowBlocksBoard* m_board;

	std::stack<CRainbowBlocksBoard*> m_undo;
	std::stack<CRainbowBlocksBoard*> m_redo;
};
