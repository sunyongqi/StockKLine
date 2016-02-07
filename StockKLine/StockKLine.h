
// StockKLine.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル

// CStockKLineApp:
// このクラスの実装については、StockKLine.cpp を参照してください。
//

class CStockKLineApp : public CWinApp
{
public:
	CStockKLineApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CStockKLineApp theApp;