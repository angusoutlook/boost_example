#ifndef _STRATEGY_H_
#define _STRATEGY_H_

#include <vector>
#include <map>

enum stock_state {
	stock_sell = -1,
	stock_no_ops = 0,
	stock_buy = 1,
};

enum indicator_state {
	indic_sell = -1,
	indic_no_ops = 0,
	indic_buy = 1,
};

struct MA_ {
	double shortMA;
	double middleMA;
	double longMA;
	double otherMA;
};

struct MACD_ {
	double ema1;
	double ema2;
	double dea;
	double diff;
	double macd;
};

struct KDJ_ {
	double rsv;
	double k, d, j;
};

struct SAR_ {
	double v;
	double ep;
	double sar;
};

enum KlineInter {
	tick = 0,
	second = 10,
	second_5 = 11,
	second_10 = 12,
	second_15 = 13,
	second_30 = 14,
	minute = 20,
	minute_5 = 21,
	minute_10 = 22,
	minute_15 = 23,
	minute_30 = 24,
	hour = 30,
	day = 40,
	day_5 = 41,
	day_10 = 42,
	day_15 = 43,
	day_30 = 44,
	day_60 = 45,
};

struct KLine_ {
	std::string symbol;
	unsigned int date, time;
	double open, high, low, close;
	double amount, volume;
};

struct divident_ {
	std::string symbol;
	unsigned int ex_date;
	double bonus_ratio;
	double divident;
	double allotamount;
	double allotprice;
};

struct strategy_4_kline{
	KLine_ kline;
	MA_ ma;
	KDJ_ kdj;
	MACD_ macd;
	SAR_ sar;
};

struct strategy_4_stock_klines{
	unsigned int length ;
	std::vector<strategy_4_kline>::iterator ma_head_iter, ma_beg_iter, ma_current_iter, ma_end_iter;
	std::vector<strategy_4_kline>::iterator macd_head_iter, macd_beg_iter, macd_current_iter, macd_end_iter;
	std::vector<strategy_4_kline>::iterator kdj_head_iter, kdj_beg_iter, kdj_current_iter, kdj_end_iter;
	std::vector<strategy_4_kline>::iterator sar_head_iter, sar_beg_iter, sar_current_iter, sar_end_iter;
	std::vector<strategy_4_kline> klines;
};

struct strategy_4_params{

};

struct strategy_4__indicator_params{
	const unsigned int ma_short = 7;
	const unsigned int ma_middle = 18;
	const unsigned int ma_long = 55;
	const unsigned int ma_other = 0;

	const unsigned int macd_short = 9;
	const unsigned int macd_middle = 12;
	const unsigned int macd_long = 26;

	const unsigned int kdj_n = 9;
	const unsigned int kdj_m1 = 3;
	const unsigned int kdj_m2 = 3;

	const unsigned int sar_m = 10;
	const double sar_af = 0.02;
	const double sar_top = 0.2;
};

struct strategy_4_inventory_params{

};

enum strategy_4_ma_priority_params{
	ppriority1 = 1,
	ppriority2 = 2,
	ppriority3 = 3,
	ppriority4 = 4,
	ppriority5 = 5,
	ppriority6 = 6,
	ppriority7 = 7,
	ppriority8 = 8,


};

#endif