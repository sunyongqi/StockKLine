#pragma once

#include <map>

#define NUM_LINE_DISPLAY	81

typedef DWORD KPATTERN;

typedef struct daySummary
{
	long date;
	double open;
	double high;
	double low;
	double close;
	long volume;
	double adjClose;
	double price5PMA;
	double price10PMA;
	double price20PMA;
	double price30PMA;
	double price60PMA;
	long volume5PMA;
	long volume10PMA;
	long volume20PMA;
	long volume30PMA;
	long volume60PMA;
	KPATTERN kPattern1;
	KPATTERN kPattern2;
	KPATTERN kPattern3;
	KPATTERN kPattern4;
} SUMMARY;

enum ePriceChange
{
	pcIncreaseMono = 0,
	pcIncrease = 1,
	pcDecreaseMono = 2,
	pcDecrease = 3,
};

enum eAveragePeriod
{
	ap5Days = 5,
	ap10Days = 10,
	ap20Days = 20,
	ap30Days = 30,
	ap60Days = 60,
};

typedef struct futureTrend
{
	int nIndex;
	bool bRiseNextDay;		// close値で判断
	double fRateNextDay;
	bool bRiseIn5Days;		// price5PMA値で判断
	double fRateIn5Days;
	bool bRiseIn10Days;		// price10PMA値で判断
	double fRateIn10Days;
} TREND;

class CKLine : public CStatic
{
public:

	CKLine();
	virtual ~CKLine();

	enum{
		// 色設定
		defRgbRed = RGB(240, 48, 48),
		defRgbGreen = RGB(32, 240, 230),
		defRgbBack	= RGB(0,0,0),
		defRgbYAxis = RGB(160, 160, 160),
		defRgbGraph = RGB(192, 0, 0),
		defRgbLine1 = RGB(216, 44, 235),
		defRgbLine2 = RGB(218, 222, 58),
		defRgbLine3 = RGB(51, 218, 37),
		defRgbLine4 = RGB(111, 129, 251),
		defRgbLine5 = RGB(222, 111, 5),
		//defRgbLine6 = RGB(218, 222, 58),
		//defRgbLine7 = RGB(218, 222, 58),
		//defRgbLine8 = RGB(218, 222, 58),
		//defRgbLine9 = RGB(218, 222, 58),
		//defRgbLine10 = RGB(218, 222, 58),
	};

	virtual void SetColor(COLORREF rgbBack, COLORREF rgbYAxis, COLORREF rgbGraph);
	virtual void SetDateRange(int iLast, int nNum);
	virtual void AnalyzeData(SUMMARY* pHistory, int nCount = 1);
	virtual int PriceToYPos(double fPrice);
	virtual int VolumeToYPos(long nVolume);
	virtual int IndexToXPos(int nIndex);
	virtual int FindCount(SUMMARY* pDate, int nPeriod);
	virtual int FindNext(SUMMARY* pDate, int nPeriod);
	virtual void SetMark(int iStart, int nCount);

	// オーバーライド
	//{{AFX_VIRTUAL(CKLine)
	//}}AFX_VIRTUAL

protected:

	CPen		m_penYAxis; 		// Y 軸描画用ペン
	CPen		m_penRed;
	CPen		m_penGreen;
	CPen		m_penMA5;
	CPen		m_penMA10;
	CPen		m_penMA20;
	CPen		m_penMA30;
	CPen		m_penMA60;
	CPen		m_penMark;			// 赤い点線

	CBrush		m_brushRed;
	CBrush		m_brushGreen;

	COLORREF	m_rgbBack;			// 背景色
	COLORREF	m_rgbYAxis; 		// Y 軸の色
	COLORREF	m_rgbGraph; 		// 線の色

	int 		m_nNumDayDisplay; 		// 画面上表示されるデータ数
	int			m_nLastIndex;			// 画面上表示される最新のデータ（一番右の）
	//int 		m_nNumYAxis;		// Y 軸の数
	double		m_fLowPrice;			// 株価描画範囲の下限
	double		m_fHighPrice;			// 株価描画範囲の上限
	long		m_nLowVolume;			// 株価描画範囲の上限
	long		m_nHighVolume;			// 株価描画範囲の上限
	int			m_nWidth;
	int			m_nHeight;
	int			m_nHeightVolume;
	int			m_nHeightPrice;
	int			m_nYSpace;
	double		m_fPlotSpace;	// プロットの間隔
	double		m_fMargin;
	int			m_nMarkStart;
	int			m_nMarkCount;

	SUMMARY*	m_pHistory;
	int			m_nDaysTotal;
	std::multimap<KPATTERN, TREND> m_mapKPatternTrend;
	int			m_nFindFrom;
	//std::multimap<KPATTERN, TREND>::iterator m_mapIterator;

	virtual void Draw(CDC* pDC);
	virtual void DrawBackground(CDC* pDC);
	virtual void DrawKLine(CDC* pDC);
	virtual void DrawPriceMA(CDC* pDC, eAveragePeriod ePeriod);
	virtual void DrawMark(CDC* pDC);
	virtual void DrawVolume(CDC* pDC);

	virtual void CalculateMovingAverages();
	virtual void CalculateKLinePattern();
	virtual void MakeKPatternMap();
	virtual BYTE EncodeRateToTwoBit(double fChangeRate);
	virtual BYTE EncodeOneDayToBYTE(SUMMARY* pData);
	virtual KPATTERN EncodeKPattern(SUMMARY* pData, int nPeriod);
	virtual ePriceChange GetClosePriceTrend(SUMMARY* pData, int nPeriod);
	virtual ePriceChange GetVolumeTrend(SUMMARY* pData, int nPeriod);
	virtual ePriceChange GetMA5Trend(SUMMARY* pData, int nPeriod);

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CKLine)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
