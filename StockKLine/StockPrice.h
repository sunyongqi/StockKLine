#pragma once
#include "curl/multi.h"
#include <string>

//#define PRICEFILE_PATH "E:\\StockPrices\\"
#define PRICEFILE_PATH "StockData\\"

class CStockPrice
{
public:
	CStockPrice();
	~CStockPrice();

	int DownloadSingleStockPrices(std::string strCode);
};

