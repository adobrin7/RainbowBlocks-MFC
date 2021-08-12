#include "pch.h"
#include "RainbowBlocksDoc.h"
#include "RainbowBlocksView.h"
#include "OptionsDialog.h"

// CRainbowBlocksView

IMPLEMENT_DYNCREATE(CRainbowBlocksView, CView)

BEGIN_MESSAGE_MAP(CRainbowBlocksView, CView)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_DIFFICULTY_3COLORS,
		&CRainbowBlocksView::OnDifficulty3colors)
	ON_COMMAND(ID_DIFFICULTY_4COLORS,
		&CRainbowBlocksView::OnDifficulty4colors)
	ON_COMMAND(ID_DIFFICULTY_5COLORS,
		&CRainbowBlocksView::OnDifficulty5colors)
	ON_COMMAND(ID_DIFFICULTY_6COLORS,
		&CRainbowBlocksView::OnDifficulty6colors)
	ON_COMMAND(ID_DIFFICULTY_7COLORS,
		&CRainbowBlocksView::OnDifficulty7colors)
	ON_UPDATE_COMMAND_UI(ID_DIFFICULTY_3COLORS,
		&CRainbowBlocksView::OnUpdateDifficulty3colors)
	ON_UPDATE_COMMAND_UI(ID_DIFFICULTY_4COLORS,
		&CRainbowBlocksView::OnUpdateDifficulty4colors)
	ON_UPDATE_COMMAND_UI(ID_DIFFICULTY_5COLORS,
		&CRainbowBlocksView::OnUpdateDifficulty5colors)
	ON_UPDATE_COMMAND_UI(ID_DIFFICULTY_6COLORS,
		&CRainbowBlocksView::OnUpdateDifficulty6colors)
	ON_UPDATE_COMMAND_UI(ID_DIFFICULTY_7COLORS,
		&CRainbowBlocksView::OnUpdateDifficulty7colors)
	ON_COMMAND(ID_SETUP_BLOCKCOUNT,
		&CRainbowBlocksView::OnSetupBlockcount)
	ON_COMMAND(ID_SETUP_BLOCKSIZE,
		&CRainbowBlocksView::OnSetupBlocksize)
	ON_COMMAND(ID_EDIT_UNDO,
		&CRainbowBlocksView::OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO,
		&CRainbowBlocksView::OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO,
		&CRainbowBlocksView::OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO,
		&CRainbowBlocksView::OnUpdateEditRedo)
END_MESSAGE_MAP()

CRainbowBlocksView::CRainbowBlocksView() noexcept
{
}

CRainbowBlocksView::~CRainbowBlocksView()
{
}

BOOL CRainbowBlocksView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

void CRainbowBlocksView::OnDraw(CDC* pDC)
{
	CRainbowBlocksDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	int nDCSave = pDC->SaveDC();

	CRect rcClient;
	GetClientRect(&rcClient);
	COLORREF clr = pDoc->GetBoardSpace(-1, -1);

	pDC->FillSolidRect(rcClient, clr);

	CBrush br;
	br.CreateStockObject(HOLLOW_BRUSH);
	CBrush* pBrOld = pDC->SelectObject(&br);

	for (int row = 0; row < pDoc->GetRows(); ++row)
	{
		for (int col = 0; col < pDoc->GetColumns(); ++col)
		{
			clr = pDoc->GetBoardSpace(row, col);
			CRect rcBlock;

			rcBlock.top = row * pDoc->GetHeight();
			rcBlock.left = col * pDoc->GetWidth();
			rcBlock.right = rcBlock.left + pDoc->GetWidth();
			rcBlock.bottom = rcBlock.top + pDoc->GetHeight();

			pDC->FillSolidRect(&rcBlock, clr);
			pDC->Rectangle(&rcBlock);
		}
	}
	pDC->RestoreDC(nDCSave);
	br.DeleteObject();
}

CRainbowBlocksDoc* CRainbowBlocksView::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRainbowBlocksDoc)));
	return (CRainbowBlocksDoc*)m_pDocument;
}

void CRainbowBlocksView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	ResizeWindow();
}

// Implementation

void CRainbowBlocksView::ResizeWindow()
{
	CRainbowBlocksDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rcClient, rcWindow;
	GetClientRect(&rcClient);
	GetParentFrame()->GetWindowRect(&rcWindow);

	int nWidthDiff = rcWindow.Width() - rcClient.Width();
	int nHeightDiff = rcWindow.Height() - rcClient.Height();

	rcWindow.right = rcWindow.left +
		pDoc->GetWidth() * pDoc->GetColumns() + nWidthDiff;
	rcWindow.bottom = rcWindow.top +
		pDoc->GetHeight() * pDoc->GetRows() + nHeightDiff;

	GetParentFrame()->MoveWindow(&rcWindow);
}

void CRainbowBlocksView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRainbowBlocksDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	int row = point.y / pDoc->GetHeight();
	int col = point.x / pDoc->GetWidth();

	bool isClickedOnBg = pDoc->GetBoardSpace(row, col) == 0;
	if (isClickedOnBg)
		return;

	int deletedCount = pDoc->DeleteBlocks(row, col);

	if (deletedCount > 0)
	{
		Invalidate();
		UpdateWindow();

		if (pDoc->IsGameOver())
		{
			CString message, windowName;
			int remaining = pDoc->GetRemainingCount();

			if (remaining > 0)
			{
				windowName.Format(_T("Game Over"));
				message.Format(_T("No more moves left\nBlocks remaining: %d."), remaining);
			}
			else
			{
				windowName.Format(_T("Game Won"));
				message.Format(_T("Congratulations!"));
			}
			MessageBox(message, windowName, MB_OK | MB_ICONINFORMATION);
		}
	}
	CView::OnLButtonDown(nFlags, point);
}

void CRainbowBlocksView::OnDifficulty3colors() { SetColorCount(3); }
void CRainbowBlocksView::OnDifficulty4colors() { SetColorCount(4); }
void CRainbowBlocksView::OnDifficulty5colors() { SetColorCount(5); }
void CRainbowBlocksView::OnDifficulty6colors() { SetColorCount(6); }
void CRainbowBlocksView::OnDifficulty7colors() { SetColorCount(7); }

void CRainbowBlocksView::OnUpdateDifficulty3colors(CCmdUI* pCmdUI)
{
	bool isSelected = IsColorSelected(3);
	pCmdUI->SetCheck(isSelected);

	if (isSelected)
		pCmdUI->Enable(false);
}
void CRainbowBlocksView::OnUpdateDifficulty4colors(CCmdUI* pCmdUI)
{
	bool isSelected = IsColorSelected(4);
	pCmdUI->SetCheck(isSelected);

	if (isSelected)
		pCmdUI->Enable(false);
}
void CRainbowBlocksView::OnUpdateDifficulty5colors(CCmdUI* pCmdUI)
{
	bool isSelected = IsColorSelected(5);
	pCmdUI->SetCheck(isSelected);

	if (isSelected)
		pCmdUI->Enable(false);
}
void CRainbowBlocksView::OnUpdateDifficulty6colors(CCmdUI* pCmdUI)
{
	bool isSelected = IsColorSelected(6);
	pCmdUI->SetCheck(isSelected);

	if (isSelected)
		pCmdUI->Enable(false);
}
void CRainbowBlocksView::OnUpdateDifficulty7colors(CCmdUI* pCmdUI)
{
	bool isSelected = IsColorSelected(7);
	pCmdUI->SetCheck(isSelected);

	if (isSelected)
		pCmdUI->Enable(false);
}

void CRainbowBlocksView::OnSetupBlockcount()
{
	CRainbowBlocksDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	COptionsDialog dlg(true, this);

	dlg.m_nValueUpside = pDoc->GetRows();
	dlg.m_nValueDownside = pDoc->GetColumns();

	if (dlg.DoModal() == IDOK)
	{
		pDoc->DeleteBoard();

		pDoc->SetRows(dlg.m_nValueUpside);
		pDoc->SetColumns(dlg.m_nValueDownside);

		pDoc->SetupBoard();

		ResizeWindow();
	}
}
void CRainbowBlocksView::OnSetupBlocksize()
{
	CRainbowBlocksDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	COptionsDialog dlg(false, this);

	dlg.m_nValueUpside = pDoc->GetWidth();
	dlg.m_nValueDownside = pDoc->GetHeight();

	if (dlg.DoModal() == IDOK)
	{
		pDoc->DeleteBoard();

		pDoc->SetWidth(dlg.m_nValueUpside);
		pDoc->SetHeight(dlg.m_nValueDownside);

		pDoc->SetupBoard();

		ResizeWindow();
	}
}

void CRainbowBlocksView::SetColorCount(int nColors)
{
	CRainbowBlocksDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pDoc->SetNumColors(nColors);

	Invalidate();
	UpdateWindow();
}

bool CRainbowBlocksView::IsColorSelected(int nColor) const
{
	CRainbowBlocksDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return FALSE;

	return pDoc->GetNumColors() == nColor;
}

void CRainbowBlocksView::OnEditUndo()
{
	CRainbowBlocksDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pDoc->UndoLast();

	Invalidate();
	UpdateWindow();
}
void CRainbowBlocksView::OnEditRedo()
{
	CRainbowBlocksDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pDoc->RedoLast();

	Invalidate();
	UpdateWindow();
}

void CRainbowBlocksView::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
	CRainbowBlocksDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pCmdUI->Enable(pDoc->CanUndo());
}
void CRainbowBlocksView::OnUpdateEditRedo(CCmdUI* pCmdUI)
{
	CRainbowBlocksDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pCmdUI->Enable(pDoc->CanRedo());
}
