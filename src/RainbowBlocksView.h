#pragma once

class CRainbowBlocksView : public CView
{
protected:
	CRainbowBlocksView() noexcept;
	virtual ~CRainbowBlocksView();
	DECLARE_DYNCREATE(CRainbowBlocksView)
	DECLARE_MESSAGE_MAP()

// Attributes
public:
	CRainbowBlocksDoc* GetDocument() const;

// Override
public:
	virtual void OnDraw(CDC* pDC); 
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();

// Implementation
public:
	void ResizeWindow();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	afx_msg void OnDifficulty3colors();
	afx_msg void OnDifficulty4colors();
	afx_msg void OnDifficulty5colors();
	afx_msg void OnDifficulty6colors();
	afx_msg void OnDifficulty7colors();

	afx_msg void OnUpdateDifficulty3colors(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDifficulty4colors(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDifficulty5colors(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDifficulty6colors(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDifficulty7colors(CCmdUI* pCmdUI);

	afx_msg void OnSetupBlockcount();
	afx_msg void OnSetupBlocksize();

	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);

protected:
	void SetColorCount(int nColors);

	bool IsColorSelected(int nColor) const;	
};
