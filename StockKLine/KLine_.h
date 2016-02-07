#pragma once

typedef struct StockPrices {
	long date;
	double open;
	double high;
	double low;
	double close;
	long volume;
	double adjClose;
} PRICES;

class CKLine : public CStatic
{
public:
	CKLine();
	~CKLine();

	enum {
		// 色設定
		defRgbBack = RGB(255, 255, 255),
		defRgbYAxis = RGB(160, 160, 160),
		defRgbGraph = RGB(192, 0, 0),

		// グラフの要素
		defNumPlot = 18,			// プロット数
		defNumYAxis = 16,			// Y 軸の数

									// 範囲
		defLower = 0,				// 下限 		
		defUpper = 100, 			// 上限
	};

	virtual void SetColor(COLORREF rgbBack, COLORREF rgbYAxis, COLORREF rgbGraph);
	virtual void SetNumPlot(unsigned int nNum);
	virtual void SetNumYAxis(unsigned int nNum);
	virtual void SetRange(double fLower, double fUpper);
	virtual void Plot(double fValue);
	virtual void SetKLineData(PRICES* pHistory, int nCount = 1);


protected:

	CPen		m_penYAxis; 		// Y 軸描画用ペン
	CPen		m_penGraph; 		// ライン描画用ペン
	CPen		m_penRed;
	CPen		m_penGreen;
	CBrush		m_brushRed;
	CBrush		m_brushGreen;

	COLORREF	m_rgbBack;			// 背景色
	COLORREF	m_rgbYAxis; 		// Y 軸の色
	COLORREF	m_rgbGraph; 		// 線の色

	int 		m_nNumPlot; 		// プロット数
	int 		m_nNumYAxis;		// Y 軸の数
	int			m_nLower;			// プロットする値の範囲の下限
	int			m_nUpper;			// プロットする値の範囲の上限

	double* 	m_pPlotData;		// プロットする値
	PRICES*		m_pHistory;
	int			m_nDataCount;

	virtual void Draw(CDC* pDC, int nWidth, int nHeight);


	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CKLine)
	afx_msg void OnPaint();
	//}}AFX_MSG
};

