#include "stdafx.h"
#include "StockGraph.h"
#include <iostream>
//#include <string>
#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/****************************************************************************
	Desc: コンストラクタ.
	 Arg: ---
  Return: ---
****************************************************************************/
CStockGraph::CStockGraph()
{
	m_nNumDayDisplay = 0;
	m_nLastIndex = 0;
	m_fLowPrice = 0;
	m_fHighPrice = 0;
	m_nWidth = 0;
	m_nHeight = 0;
	m_fPlotSpace = 0;
	m_fMargin = 0;
	m_nMarkStart = m_nMarkCount = 0;

	SetColor(defRgbBack, defRgbYAxis, defRgbGraph);

	SetDateRange(0, NUM_LINE_DISPLAY);
	

	m_mapKPatternTrend.clear();
	m_nFindFrom = 0;

	m_nAveragePeriod[0] = ap5Days;
	m_nAveragePeriod[1] = ap10Days;
	m_nAveragePeriod[2] = ap20Days;
	m_nAveragePeriod[3] = ap30Days;
	m_nAveragePeriod[4] = ap60Days;
}

/****************************************************************************
	Desc: デストラクタ.
	 Arg: ---
  Return: ---
****************************************************************************/
CStockGraph::~CStockGraph()
{
	m_penYAxis.DeleteObject();
	m_penRed.DeleteObject();
	m_penGreen.DeleteObject();
	for (int iLine = 0; iLine < MA_LINE_MAX; iLine++)
	{
		m_penMA[iLine].DeleteObject();
	}

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
void CStockGraph::SetColor(COLORREF rgbBack, COLORREF rgbYAxis, COLORREF rgbGraph)
{
	m_penYAxis.DeleteObject();
	m_penRed.DeleteObject();
	m_penGreen.DeleteObject();
	for (int iLine = 0; iLine < MA_LINE_MAX; iLine++)
	{
		m_penMA[iLine].DeleteObject();
	}
	m_brushRed.DeleteObject();
	m_brushGreen.DeleteObject();

	m_penYAxis.CreatePen(PS_DOT, 1, defRgbYAxis);
	m_penRed.CreatePen(PS_SOLID, 2, defRgbRed);
	m_penGreen.CreatePen(PS_SOLID, 2, defRgbGreen);
	m_penMA[0].CreatePen(PS_SOLID, 1, defRgbLine1);
	m_penMA[1].CreatePen(PS_SOLID, 1, defRgbLine2);
	m_penMA[2].CreatePen(PS_SOLID, 1, defRgbLine3);
	m_penMA[3].CreatePen(PS_SOLID, 1, defRgbLine4);
	m_penMA[4].CreatePen(PS_SOLID, 1, defRgbLine5);
	m_penMark.CreatePen(PS_DOT, 1, defRgbRed);
	m_brushRed.CreateSolidBrush(defRgbRed);
	m_brushGreen.CreateSolidBrush(defRgbGreen);
}


/****************************************************************************
	Desc: プロット数を設定する.
	 Arg: プロット数
  Return: ---
****************************************************************************/
void CStockGraph::SetDateRange(int iLast, int nNum)
{
	m_nNumDayDisplay = nNum;
	m_nLastIndex = iLast;
}

void CStockGraph::AnalyzeData(SUMMARY* pHistory, int nCount)
{
	m_pHistory = pHistory;
	m_nDaysTotal = nCount;

	CalculateMovingAverages();
	//CalculateKLinePattern();
	//MakeKPatternMap();
}

void CStockGraph::CalculateMovingAverages()
{
	if (!m_pHistory)
		return;

	for (int iDay = 0; iDay < m_nDaysTotal; iDay++)
	{
		for (int iLine = 0; iLine < MA_LINE_MAX; iLine++)
		{
			double priceSum = 0;
			long volumeSum = 0;
			for (int iPeriod = 0; iPeriod < m_nAveragePeriod[iLine]; iPeriod++)
			{
				if (iDay + iPeriod >= m_nDaysTotal)
					break;
				priceSum += (m_pHistory + iDay + iPeriod)->close;
				volumeSum += (m_pHistory + iDay + iPeriod)->volume;
			}
			(m_pHistory + iDay)->priceMA[iLine] = priceSum / m_nAveragePeriod[iLine];
			(m_pHistory + iDay)->volumeMA[iLine] = volumeSum / m_nAveragePeriod[iLine];
		}
	}
}
//void CStockGraph::CalculateMovingAverages()
//{
//	if (!m_pHistory)
//		return;
//
//	for (int i = 0; i < m_nDaysTotal; i++) {
//		int j = 0;
//		double priceSum = 0;
//		long volumeSum = 0;
//		for (; j < 5; j++)
//		{
//			if (i + j >= m_nDaysTotal)
//				break;
//			priceSum += (m_pHistory + i + j)->close;
//			volumeSum += (m_pHistory + i + j)->volume;
//		}
//		(m_pHistory + i)->priceMA_1 = priceSum / j;
//		(m_pHistory + i)->volumeMA_1 = volumeSum / j;
//		for (; j < 10; j++)
//		{
//			if (i + j >= m_nDaysTotal)
//				break;
//			priceSum += (m_pHistory + i + j)->close;
//			volumeSum += (m_pHistory + i + j)->volume;
//		}
//		(m_pHistory + i)->priceMA_2 = priceSum / j;
//		(m_pHistory + i)->volumeMA_2 = volumeSum / j;
//		for (; j < 20; j++)
//		{
//			if (i + j >= m_nDaysTotal)
//				break;
//			priceSum += (m_pHistory + i + j)->close;
//			volumeSum += (m_pHistory + i + j)->volume;
//		}
//		(m_pHistory + i)->priceMA_3 = priceSum / j;
//		(m_pHistory + i)->volumeMA_3 = volumeSum / j;
//		for (; j < 30; j++)
//		{
//			if (i + j >= m_nDaysTotal)
//				break;
//			priceSum += (m_pHistory + i + j)->close;
//			volumeSum += (m_pHistory + i + j)->volume;
//		}
//		(m_pHistory + i)->priceMA_4 = priceSum / j;
//		(m_pHistory + i)->volumeMA_4 = volumeSum / j;
//		for (; j < 60; j++)
//		{
//			if (i + j >= m_nDaysTotal)
//				break;
//			priceSum += (m_pHistory + i + j)->close;
//			volumeSum += (m_pHistory + i + j)->volume;
//		}
//		(m_pHistory + i)->priceMA_5 = priceSum / j;
//		(m_pHistory + i)->volumeMA_5 = volumeSum / j;
//	}
//}

void CStockGraph::CalculateKLinePattern()
{
	if (!m_pHistory)
		return;

	for (int i = 0; i < m_nDaysTotal; i++)
	{
		(m_pHistory + i)->kPattern1 = EncodeKPattern(m_pHistory + i, 1);
	}
	for (int i = 0; i < m_nDaysTotal; i++)
	{
		(m_pHistory + i)->kPattern2 = EncodeKPattern(m_pHistory + i, 2);
	}
	for (int i = 0; i < m_nDaysTotal; i++)
	{
		(m_pHistory + i)->kPattern3 = EncodeKPattern(m_pHistory + i, 3);
	}
	for (int i = 0; i < m_nDaysTotal; i++)
	{
		(m_pHistory + i)->kPattern4 = EncodeKPattern(m_pHistory + i, 4);
	}
}

void CStockGraph::MakeKPatternMap()
{
	m_mapKPatternTrend.clear();
	m_nFindFrom = 0;
	for (int i = 0; i < m_nDaysTotal; i++)
	{
		TREND trend = { 0, 0, 0, 0, 0, 0, 0 };
		trend.nIndex = i;
		if (i >= 1)
		{
			trend.bRiseNextDay = ((m_pHistory + i - 1)->close - (m_pHistory + i)->close) >= 0 ? true : false;
			trend.fRateNextDay = ((m_pHistory + i - 1)->close - (m_pHistory + i)->close) / (m_pHistory + i)->close;
		}
		if (i >= 5)
		{
			trend.bRiseIn5Days = ((m_pHistory + i - 5)->priceMA[0] - (m_pHistory + i)->priceMA[0]) >= 0 ? true : false;
			trend.fRateIn5Days = ((m_pHistory + i - 5)->priceMA[0] - (m_pHistory + i)->priceMA[0]) / (m_pHistory + i)->priceMA[0];
		}
		if (i >= 10)
		{
			trend.bRiseIn10Days = ((m_pHistory + i - 10)->priceMA[1] - (m_pHistory + i)->priceMA[1]) >= 0 ? true : false;
			trend.fRateIn10Days = ((m_pHistory + i - 10)->priceMA[1] - (m_pHistory + i)->priceMA[1]) / (m_pHistory + i)->priceMA[1];
		}
		m_mapKPatternTrend.insert(std::make_pair((m_pHistory + i)->kPattern3, trend));
	}
}

#define BOUNDARY_RATE 0.03  // 3%。EncodeRateToTwoBit用。
BYTE CStockGraph::EncodeRateToTwoBit(double fChangeRate)
{
	BYTE byRet;
	if (fChangeRate >= 0)
		byRet = (fChangeRate > BOUNDARY_RATE) ? 1 : 0;
	else
		byRet = (-fChangeRate > BOUNDARY_RATE) ? 3 : 0;
		//byRet = (-fChangeRate > BOUNDARY_RATE) ? 3 : 2;
	return byRet;
}

BYTE CStockGraph::EncodeOneDayToBYTE(SUMMARY* pData)
{
	//double fValueBefore = (pData + 1)->close;
	//double fRateOpen = (pData->open - fValueBefore) / fValueBefore;
	//double fRateHigh = (pData->high - fValueBefore) / fValueBefore;
	//double fRateLow = (pData->low - fValueBefore) / fValueBefore;
	//double fRateClose = (pData->close - fValueBefore) / fValueBefore;

	double fRateOpen = (pData->open - (pData + 1)->close) >= 0 ? 1 : -1;	// 前日closeとの比較は数値を見ず、正負だけをみる
	//double fRateOpen = (pData->open - (pData + 1)->close) / (pData + 1)->close;
	double fRateHigh = (pData->high - pData->open) / pData->open;
	double fRateLow = (pData->low - pData->open) / pData->open;
	double fRateClose = (pData->close - pData->open) / pData->open;
	//BYTE byRet = (EncodeRateToTwoBit(fRateClose) << 6)
	//	+ (EncodeRateToTwoBit(fRateLow) << 4)
	//	+ (EncodeRateToTwoBit(fRateHigh) << 2)
	//	+ EncodeRateToTwoBit(fRateOpen);
	BYTE byRet = (EncodeRateToTwoBit(fRateOpen) << 6)
		+ (EncodeRateToTwoBit(fRateHigh) << 4)
		+ (EncodeRateToTwoBit(fRateLow) << 2)
		+ EncodeRateToTwoBit(fRateClose);
	return byRet;
}

KPATTERN CStockGraph::EncodeKPattern(SUMMARY* pData, int nPeriod)
{
	ASSERT(nPeriod > 0);
	ASSERT(nPeriod <= 4);
	KPATTERN dwRet = 0;
	for (int i = 0; i < nPeriod; i++)
	{
		//dwRet += (EncodeOneDayToBYTE(pData + i) << (24 - i * 8));
		dwRet += (EncodeOneDayToBYTE(pData + i) << (i * 8));
	}
	return dwRet;
}

ePriceChange CStockGraph::GetClosePriceTrend(SUMMARY* pData, int nPeriod)
{
	ASSERT(nPeriod > 1);
	if (pData->close > (pData + nPeriod)->close)	// Rise
	{
		for (int i = 0; i < nPeriod; i++)
		{
			if ((pData + i)->close < (pData + i + 1)->close)
			{
				return pcIncrease;
			}
		}
		return pcIncreaseMono;
	}
	else	// Fall
	{
		for (int i = 0; i < nPeriod; i++)
		{
			if ((pData + i)->close >(pData + i + 1)->close)
			{
				return pcDecrease;
			}
		}
		return pcDecreaseMono;
	}
}

ePriceChange CStockGraph::GetVolumeTrend(SUMMARY* pData, int nPeriod)
{
	ASSERT(nPeriod > 1);
	if (pData->volume > (pData + nPeriod)->volume)	// Rise
	{
		for (int i = 0; i < nPeriod; i++)
		{
			if ((pData + i)->volume < (pData + i + 1)->volume)
			{
				return pcIncrease;
			}
		}
		return pcIncreaseMono;
	}
	else	// Fall
	{
		for (int i = 0; i < nPeriod; i++)
		{
			if ((pData + i)->volume >(pData + i + 1)->volume)
			{
				return pcDecrease;
			}
		}
		return pcDecreaseMono;
	}
}

ePriceChange CStockGraph::GetMA5Trend(SUMMARY* pData, int nPeriod)
{
	ASSERT(nPeriod > 1);
	if (pData->priceMA[0] > (pData + nPeriod)->priceMA[0])	// Rise
	{
		for (int i = 0; i < nPeriod; i++)
		{
			if ((pData + i)->priceMA[0] < (pData + i + 1)->priceMA[0])
			{
				return pcIncrease;
			}
		}
		return pcIncreaseMono;
	}
	else	// Fall
	{
		for (int i = 0; i < nPeriod; i++)
		{
			if ((pData + i)->priceMA[0] >(pData + i + 1)->priceMA[0])
			{
				return pcDecrease;
			}
		}
		return pcDecreaseMono;
	}
}

int CStockGraph::FindCount(SUMMARY* pDate, int nPeriod)
{
	ASSERT(nPeriod <= 4);
	KPATTERN kPattern = EncodeKPattern(pDate, nPeriod);

	return m_mapKPatternTrend.count(kPattern);
}

int CStockGraph::FindNext(SUMMARY* pDate, int nPeriod)
{
	ASSERT(nPeriod <= 4);
	ASSERT(nPeriod == 3);	// 現状mapはkPattern3しか対応していないため
	KPATTERN kPattern = EncodeKPattern(pDate, nPeriod);

	if (m_nFindFrom >= (int)m_mapKPatternTrend.count(kPattern))
	{
		m_nFindFrom = 0;
		return -1;
	}

	std::multimap<KPATTERN, TREND>::iterator it = m_mapKPatternTrend.find(kPattern);
	if (it != m_mapKPatternTrend.end())
	{
		for (int i = 0; i < m_nFindFrom; i++)
			it++;
		TREND trend = it->second;
		m_nFindFrom++;
		return trend.nIndex;
	}


	//if (m_mapIterator == m_mapKPatternTrend.end())
	//{
	//	m_mapIterator = m_mapKPatternTrend.find(kPattern);
	//	//switch (nPeriod)
	//	//{
	//	//case 1:
	//	//	m_mapIterator = m_mapKPatternTrend.find(pDate->kPattern1);
	//	//	break;
	//	//case 2:
	//	//	m_mapIterator = m_mapKPatternTrend.find(pDate->kPattern2);
	//	//	break;
	//	//case 3:
	//	//	m_mapIterator = m_mapKPatternTrend.find(pDate->kPattern3);
	//	//	break;
	//	//case 4:
	//	//default:
	//	//	m_mapIterator = m_mapKPatternTrend.find(pDate->kPattern4);
	//	//	break;
	//	//}
	//}
	//
	//if (it != m_mapKPatternTrend.end())
	//{
	//	TREND trend = it->second;
	//	it++;
	//	return trend.nIndex;
	//}

	return -1;
}

void CStockGraph::SetMark(int iStart, int nCount)
{
	m_nMarkStart = iStart;
	m_nMarkCount = nCount;
}

/****************************************************************************
	Desc: グラフを描画する.
	 Arg: グラフの幅
		  グラフの高さ
  Return: ---
****************************************************************************/
void CStockGraph::Draw(CDC* pDC)
{
	DrawBackground(pDC);
	DrawKLine(pDC);
	DrawVolume(pDC);
	for (int iLine = 0; iLine < MA_LINE_MAX; iLine++)
	{
		DrawMovingAverages(pDC, iLine);
	}

	if (m_nMarkCount > 0)
		DrawMark(pDC);
}

void CStockGraph::DrawBackground(CDC* pDC)
{
	//-----------------------------------------------------------------------
	// 背景
	pDC->FillSolidRect(0, 0, m_nWidth, m_nHeight, defRgbBack);

	//-----------------------------------------------------------------------
	// Y 軸

	m_fPlotSpace = (double)m_nWidth / (m_nNumDayDisplay - 0);	// プロットの間隔
	m_fMargin = m_fPlotSpace * 0.5;
	double widthDraw = (double)m_nWidth - m_fPlotSpace; // m_fMargin*2

	double YAxisX;	// Y 軸の X 位置
	int nNumYAxis = min(m_nNumDayDisplay - 1, 8);		// Y軸の数、最大8にする
	double YAxisSpace = (double)widthDraw / (double)nNumYAxis; // Y 軸の間隔

	CPen* pOldPen = pDC->SelectObject(&m_penYAxis);
	//CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

	for (int i = 0; i <= nNumYAxis; i++) {
		YAxisX = (double)m_nWidth - YAxisSpace * i - m_fMargin;
		pDC->MoveTo((int)YAxisX, 0);
		pDC->LineTo((int)YAxisX, m_nHeight);
	}

	if (!m_pHistory)
	{
		m_fLowPrice = 0;
		m_fHighPrice = 10;
		m_nLowVolume = 0;
		m_nHighVolume = 0;
	}
	else
	{
		// 描画範囲計算
		SUMMARY* pData = m_pHistory + m_nLastIndex;
		m_fLowPrice = pData->low;
		m_fHighPrice = pData->high;
		m_nLowVolume = m_nHighVolume = pData->volume;
		for (int i = 1; i < m_nNumDayDisplay/*m_nDaysTotal*/; i++)
		{
			int index = m_nLastIndex + i;
			if (index >= m_nDaysTotal)
				continue;

			pData = m_pHistory + index;
			if (m_fLowPrice > pData->low)
				m_fLowPrice = pData->low;
			if (m_fHighPrice < pData->high)
				m_fHighPrice = pData->high;
			if (m_nLowVolume > pData->volume)
				m_nLowVolume = pData->volume;
			if (m_nHighVolume < pData->volume)
				m_nHighVolume = pData->volume;
		}
	}
	
	if (m_fLowPrice == m_fHighPrice)
		return;

	// 3本のPriceLineを描画
	double fMidPrice = ((int)((m_fLowPrice + m_fHighPrice) / 2.0 * 100)) / 100.0;
	pDC->MoveTo(0, PriceToYPos(m_fLowPrice));
	pDC->LineTo(m_nWidth, PriceToYPos(m_fLowPrice));
	pDC->MoveTo(0, PriceToYPos(fMidPrice));
	pDC->LineTo(m_nWidth, PriceToYPos(fMidPrice));
	pDC->MoveTo(0, PriceToYPos(m_fHighPrice));
	pDC->LineTo(m_nWidth, PriceToYPos(m_fHighPrice));
	
	std::ostringstream oss;
	oss << m_fLowPrice;
	CString strLowPrice(oss.str().c_str());
	pDC->SetTextColor(defRgbYAxis);
	pDC->TextOutW(10, PriceToYPos(m_fLowPrice) - 10, strLowPrice);
	oss.str("");
	oss.clear(std::ostringstream::goodbit);
	oss << fMidPrice;
	CString strMidPrice(oss.str().c_str());
	pDC->TextOutW(10, PriceToYPos(fMidPrice) - 10, strMidPrice);
	oss.str("");
	oss.clear(std::ostringstream::goodbit);
	oss << m_fHighPrice;
	CString strHighPrice(oss.str().c_str());
	pDC->TextOutW(10, PriceToYPos(m_fHighPrice) - 10, strHighPrice);

	if (m_nLowVolume == m_nHighVolume)
		return;

	// 2本のVolumeLineを描画
	pDC->MoveTo(0, VolumeToYPos(m_nLowVolume));
	pDC->LineTo(m_nWidth, VolumeToYPos(m_nLowVolume));
	pDC->MoveTo(0, VolumeToYPos(m_nHighVolume));
	pDC->LineTo(m_nWidth, VolumeToYPos(m_nHighVolume));
	int y1 = VolumeToYPos(m_nLowVolume);
	int y2 = VolumeToYPos(m_nHighVolume);

	oss.str("");
	oss.clear(std::ostringstream::goodbit);
	oss << m_nLowVolume;
	CString strLowVolume(oss.str().c_str());
	pDC->SetTextColor(defRgbYAxis);
	pDC->TextOutW(10, VolumeToYPos(m_nLowVolume) - 10, strLowVolume);
	oss.str("");
	oss.clear(std::ostringstream::goodbit);
	oss << m_nHighVolume;
	CString strHighVolume(oss.str().c_str());
	pDC->TextOutW(10, VolumeToYPos(m_nHighVolume) - 10, strHighVolume);

	pDC->SelectObject(pOldPen);
}

void CStockGraph::DrawKLine(CDC* pDC)
{
	ASSERT(m_nNumDayDisplay > 1);
	if (!m_pHistory)
		return;

	if (m_fLowPrice == m_fHighPrice)
		return;

	CPen* pOldPen = (CPen*)pDC->SelectStockObject(NULL_PEN);
	CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

	double XPos;	// 描画する X 位置
	int rcWidth = (int)(m_fPlotSpace * 1.5 / 2.0);
	int rcTop, rcLeft, rcRight, rcBottom, lineTop, lineBottom;
	SUMMARY* pData = NULL;

	for (int i = 0; i < m_nNumDayDisplay; i++) {
		int index = m_nLastIndex + i;
		if (index >= m_nDaysTotal)
			continue;

		pData = m_pHistory + index;

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

		XPos = IndexToXPos(i);
		rcLeft = (int)(XPos - rcWidth / 2.0);
		rcRight = (int)(XPos + rcWidth / 2.0);
		rcTop = PriceToYPos(pData->open);
		rcBottom = PriceToYPos(pData->close);
		lineTop = PriceToYPos(pData->high);
		lineBottom = PriceToYPos(pData->low);

		if (rcTop == rcBottom)
		{
			pDC->MoveTo(rcLeft, (int)rcTop);
			pDC->LineTo(rcRight, (int)rcTop);
		}
		else
		{
			CRect rect(rcLeft, rcTop, rcRight, rcBottom);
			pDC->Rectangle(rect);
		}
		pDC->MoveTo((int)XPos, (int)lineTop);
		pDC->LineTo((int)XPos, (int)lineBottom);
	}

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}

void CStockGraph::DrawMovingAverages(CDC* pDC, int iLine)
{
	ASSERT(m_nNumDayDisplay > 1);
	if (!m_pHistory)
		return;

	CPen* pOldPen = (CPen*)pDC->SelectObject(&m_penMA[iLine]);
	if (m_fLowPrice < m_fHighPrice)
	{
		pDC->MoveTo(IndexToXPos(0), PriceToYPos((m_pHistory + m_nLastIndex)->priceMA[iLine]));
		for (int i = 1; i < m_nNumDayDisplay; i++)
		{
			pDC->LineTo(IndexToXPos(i), PriceToYPos((m_pHistory + m_nLastIndex + i)->priceMA[iLine]));
		}
	}
	if (m_nLowVolume < m_nHighVolume)
	{
		pDC->MoveTo(IndexToXPos(0), VolumeToYPos((m_pHistory + m_nLastIndex)->volumeMA[iLine]));
		for (int i = 1; i < m_nNumDayDisplay; i++)
		{
			pDC->LineTo(IndexToXPos(i), VolumeToYPos((m_pHistory + m_nLastIndex + i)->volumeMA[iLine]));
		}
	}

	pDC->SelectObject(pOldPen);
}

void CStockGraph::DrawMark(CDC* pDC)
{
	if (m_nMarkCount < 1)
		return;

	if (m_fLowPrice == m_fHighPrice)
		return;

	CPen* pOldPen = pDC->SelectObject(&m_penMark);
	CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

	int XPosLeft = IndexToXPos(m_nMarkStart - m_nLastIndex + m_nMarkCount - 1);
	int XPosRight = IndexToXPos(m_nMarkStart - m_nLastIndex);
	int rcLeft = (int)(XPosLeft - m_fPlotSpace / 2.0);
	int rcRight = (int)(XPosRight + m_fPlotSpace / 2.0);
	int rcTop = PriceToYPos(max((m_pHistory + m_nMarkStart)->high, (m_pHistory + m_nMarkStart + m_nMarkCount - 1)->high));
	int rcBottom = PriceToYPos(min((m_pHistory + m_nMarkStart)->low, (m_pHistory + m_nMarkStart + m_nMarkCount - 1)->low));
	pDC->Rectangle(rcLeft, rcTop, rcRight, rcBottom);

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
}

void CStockGraph::DrawVolume(CDC* pDC)
{
	ASSERT(m_nNumDayDisplay > 1);
	if (!m_pHistory)
		return;

	if (m_nLowVolume == m_nHighVolume)
		return;

	CPen* pOldPen = (CPen*)pDC->SelectStockObject(NULL_PEN);
	CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

	double XPos;	// 描画する X 位置
	int rcWidth = (int)(m_fPlotSpace * 1.5 / 2.0);
	int rcTop, rcLeft, rcRight, rcBottom;
	SUMMARY* pData = NULL;

	for (int i = 0; i < m_nNumDayDisplay; i++) {
		int index = m_nLastIndex + i;
		if (index >= m_nDaysTotal)
			continue;

		pData = m_pHistory + index;

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

		XPos = IndexToXPos(i);
		rcLeft = (int)(XPos - rcWidth / 2.0);
		rcRight = (int)(XPos + rcWidth / 2.0);
		rcTop = VolumeToYPos(pData->volume);
		rcBottom = VolumeToYPos(0);
		CRect rect(rcLeft, rcTop, rcRight, rcBottom);
		pDC->Rectangle(rect);
	}

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
}

int CStockGraph::PriceToYPos(double fPrice)
{
	ASSERT(m_fHighPrice - m_fLowPrice != 0);
	return (int)(m_nYSpace + (double)m_nHeightPrice * (1.0 - (fPrice - m_fLowPrice) / (m_fHighPrice - m_fLowPrice)));
}

int CStockGraph::VolumeToYPos(long nVolume)
{
	ASSERT(m_nHighVolume - m_nLowVolume != 0);
	return (int)(m_nYSpace * 2 + m_nHeightPrice + (double)m_nHeightVolume * (1.0 - (double)(nVolume - m_nLowVolume) / (double)(m_nHighVolume - m_nLowVolume)));
}

int CStockGraph::IndexToXPos(int nIndex)
{
	return m_nWidth - (int)(m_fPlotSpace * (double)nIndex + m_fMargin);
}


/****************************************************************************

	Message Handlers

****************************************************************************/
BEGIN_MESSAGE_MAP(CStockGraph, CStatic)
	//{{AFX_MSG_MAP(CStockGraph)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/****************************************************************************
	Desc: グラフの描画.
	 Arg: ---
  Return: ---
****************************************************************************/
void CStockGraph::OnPaint() 
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト

	CDC MemDC;
	CBitmap Bitmap;
	CBitmap* pOldBitmap;
	CRect rcClient;

	GetClientRect(&rcClient);
	m_nWidth = rcClient.Width();
	m_nHeight = rcClient.Height();
	m_nYSpace = m_nHeight / 40;
	m_nHeightVolume = (m_nHeight - m_nYSpace * 2) / 4;
	m_nHeightPrice = m_nHeight - m_nHeightVolume - m_nYSpace * 2;

	MemDC.CreateCompatibleDC(&dc);
	Bitmap.CreateCompatibleBitmap(&dc, m_nWidth, m_nHeight);
	pOldBitmap = MemDC.SelectObject(&Bitmap);

	// グラフ描画
	Draw(&MemDC);

	dc.BitBlt(0, 0, m_nWidth, m_nHeight, &MemDC, 0, 0, SRCCOPY);

	MemDC.SelectObject(pOldBitmap);

}