#include "stdafx.h"
#include "KLine.h"


CKLine::CKLine()
{
	SetColor(defRgbBack, defRgbYAxis, defRgbGraph);

	m_pPlotData = NULL;
	SetNumPlot(defNumPlot);

	m_nNumYAxis = defNumYAxis;
	m_penRed.CreatePen(PS_SOLID, 1, 0x0000FF);
	m_penGreen.CreatePen(PS_SOLID, 1, 0x00FF00);
	m_brushRed.CreateSolidBrush(0x0000FF);
	m_brushGreen.CreateSolidBrush(0x00FF00);

	SetRange(defLower, defUpper);
}


CKLine::~CKLine()
{
	m_penRed.DeleteObject();
	m_penGreen.DeleteObject();
	m_brushRed.DeleteObject();
	m_brushGreen.DeleteObject();
}

/****************************************************************************
Desc: 色を設定する.
Arg: 背景の色
Y 軸の色
グラフの色
Return: ---
****************************************************************************/
void CKLine::SetColor(COLORREF rgbBack, COLORREF rgbYAxis, COLORREF rgbGraph)
{
	m_rgbBack = rgbBack;
	m_rgbYAxis = rgbYAxis;
	m_rgbGraph = rgbGraph;

	m_penYAxis.DeleteObject();
	m_penGraph.DeleteObject();

	m_penYAxis.CreatePen(PS_DOT, 1, m_rgbYAxis);
	m_penGraph.CreatePen(PS_SOLID, 1, m_rgbGraph);
}


/****************************************************************************
Desc: プロット数を設定する.
Arg: プロット数
Return: ---
****************************************************************************/
void CKLine::SetNumPlot(unsigned int nNum)
{
	ASSERT(nNum > 1);

	m_nNumPlot = nNum;

	if (m_pPlotData) delete[] m_pPlotData;
	m_pPlotData = new double[m_nNumPlot];

	::ZeroMemory(m_pPlotData, sizeof(double) * m_nNumPlot);
}

/****************************************************************************
Desc: Y 軸の数を設定する.
Arg: Y 軸の数
Return: ---
****************************************************************************/
void CKLine::SetNumYAxis(unsigned int nNum)
{
	m_nNumYAxis = nNum;
}

/****************************************************************************
Desc: グラフの取る値の範囲を設定する.
Arg: 下限
上限
Return: ---
****************************************************************************/
void CKLine::SetRange(double fLower, double fUpper)
{
	if (fLower > fUpper) {
		double tmp = fLower;
		fLower = fUpper;
		fUpper = tmp;
	}

	ASSERT(fUpper - fLower != 0);

	m_nLower = fLower * 100;
	m_nUpper = fUpper * 100;

}

/****************************************************************************
Desc: プロットする.
Arg: 値
Return: ---
****************************************************************************/
void CKLine::Plot(double fValue)
{
	for (int i = m_nNumPlot - 1; i > 0; i--) {
		m_pPlotData[i] = m_pPlotData[i - 1];
	}

	m_pPlotData[0] = fValue;

	InvalidateRect(NULL, FALSE);
}

void CKLine::SetKLineData(PRICES* pHistory, int nCount)
{
	ASSERT(m_nNumPlot > 1);
	ASSERT(m_nUpper - m_nLower != 0);

	m_pHistory = pHistory;
	m_nDataCount = nCount;

	InvalidateRect(NULL, FALSE);
}

/****************************************************************************
Desc: グラフを描画する.
Arg: グラフの幅
グラフの高さ
Return: ---
****************************************************************************/
void CKLine::Draw(CDC* pDC, int nWidth, int nHeight)
{
	CPen* pOldPen;

	//-----------------------------------------------------------------------
	// 背景
	pDC->FillSolidRect(0, 0, nWidth, nHeight, m_rgbBack);

	//-----------------------------------------------------------------------
	// Y 軸

	//double YAxisX;	// Y 軸の X 位置
	//double YAxisSpace = (double)nWidth / (m_nNumYAxis + 1); // Y 軸の間隔

	//pOldPen = pDC->SelectObject(&m_penYAxis);
	//CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

	//for(int i = 1; i <= m_nNumYAxis; i++){
	//	YAxisX = nWidth - YAxisSpace * i;
	//	pDC->MoveTo((int)YAxisX, 0);
	//	pDC->LineTo((int)YAxisX, nHeight);
	//}

	//-----------------------------------------------------------------------
	// グラフ

	ASSERT(m_nNumPlot > 1);
	ASSERT(m_nUpper - m_nLower != 0);

	double PlotX;	// プロットする X 位置
	double PlotSpace = (double)nWidth / (m_nNumPlot - 1);	// プロットの間隔
	int rcWidth = (int)(PlotSpace / 2.0);
	int rcTop, rcLeft, rcRight, rcBottom, lineTop, lineBottom;

	nHeight--;	// PlotY = nHeight の位置には線を描画できないため



	for (int j = 0; j < m_nNumPlot; j++) {
		if (j >= m_nDataCount)
			continue;
		PRICES* pData = m_pHistory + j;

		if (pData->close >= pData->open)
		{
			pDC->SelectObject(&m_penRed);
			pDC->SelectObject(&m_brushRed);
		}
		else
		{
			pDC->SelectObject(&m_penGreen);
			pDC->SelectObject(&m_brushGreen);
		}

		PlotX = nWidth - PlotSpace * j;
		rcLeft = PlotX - rcWidth / 2;
		rcRight = PlotX + rcWidth / 2;
		rcTop = nHeight * (1.0 - (double)(pData->open * 100 - m_nLower) / (double)(m_nUpper - m_nLower));
		rcBottom = nHeight * (1.0 - (double)(pData->close * 100 - m_nLower) / (double)(m_nUpper - m_nLower));
		lineTop = nHeight * (1.0 - (double)(pData->high * 100 - m_nLower) / (double)(m_nUpper - m_nLower));
		lineBottom = nHeight * (1.0 - (double)(pData->low * 100 - m_nLower) / (double)(m_nUpper - m_nLower));
		CRect rect(rcLeft, rcTop, rcRight, rcBottom);

		pDC->Rectangle(rect);
		pDC->MoveTo((int)PlotX, (int)lineTop);
		pDC->LineTo((int)PlotX, (int)lineBottom);
	}

	//pDC->SelectObject(pOldBrush);
	//pDC->SelectObject(pOldPen);

}


/****************************************************************************

Message Handlers

****************************************************************************/
BEGIN_MESSAGE_MAP(CKLine, CStatic)
	//{{AFX_MSG_MAP(CKLine)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/****************************************************************************
Desc: グラフの描画.
Arg: ---
Return: ---
****************************************************************************/
void CKLine::OnPaint()
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト

	CDC MemDC;
	CBitmap Bitmap;
	CBitmap* pOldBitmap;
	CRect rcClient;

	GetClientRect(&rcClient);
	int nWidth = rcClient.Width();
	int nHeight = rcClient.Height();

	MemDC.CreateCompatibleDC(&dc);
	Bitmap.CreateCompatibleBitmap(&dc, nWidth, nHeight);
	pOldBitmap = MemDC.SelectObject(&Bitmap);

	// グラフ描画
	Draw(&MemDC, nWidth, nHeight);

	dc.BitBlt(0, 0, nWidth, nHeight, &MemDC, 0, 0, SRCCOPY);

	MemDC.SelectObject(pOldBitmap);

}