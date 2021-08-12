#include "pch.h"
#include "RainbowBlocksDoc.h"

// CRainbowBlocksDoc

IMPLEMENT_DYNCREATE(CRainbowBlocksDoc, CDocument)

BEGIN_MESSAGE_MAP(CRainbowBlocksDoc, CDocument)
END_MESSAGE_MAP()

CRainbowBlocksDoc::CRainbowBlocksDoc() noexcept
{
	m_board = new CRainbowBlocksBoard();
}

CRainbowBlocksDoc::~CRainbowBlocksDoc()
{
	ClearUndo();
	ClearRedo();

	delete m_board;
}

BOOL CRainbowBlocksDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// Last game data
	ClearUndo();
	ClearRedo();

	m_board->SetupBoard();	

	return TRUE;
}

// Implimentation

void CRainbowBlocksDoc::SetNumColors(int nColors)
{
	m_board->SetNumColors(nColors);
	m_board->SetupBoard();
}

int CRainbowBlocksDoc::DeleteBlocks(int row, int col)
{
	m_undo.push(new CRainbowBlocksBoard(*m_board));
	ClearRedo();

	int deletedCount = m_board->DeleteBlocks(row, col);

	if (m_board->IsGameOver())
		ClearUndo();

	return deletedCount;
}

bool CRainbowBlocksDoc::CanUndo()
{
	return !m_undo.empty();
}

bool CRainbowBlocksDoc::CanRedo()
{
	return !m_redo.empty();
}

void CRainbowBlocksDoc::UndoLast()
{
	if (m_undo.empty())
		return;

	m_redo.push(m_board);

	m_board = m_undo.top();
	m_undo.pop();
}

void CRainbowBlocksDoc::RedoLast()
{
	if (m_redo.empty())
		return;

	m_undo.push(m_board);

	m_board = m_redo.top();
	m_redo.pop();
}

void CRainbowBlocksDoc::ClearUndo()
{
	while (!m_undo.empty())
	{
		delete m_undo.top();
		m_undo.pop();
	}
}

void CRainbowBlocksDoc::ClearRedo()
{
	while (!m_redo.empty())
	{
		delete m_redo.top();
		m_redo.pop();
	}
}
