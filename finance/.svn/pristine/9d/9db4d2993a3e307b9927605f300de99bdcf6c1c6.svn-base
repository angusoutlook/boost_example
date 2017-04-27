#include "strategy.h"
#include "helper.h"
#include <boost/timer/timer.hpp>
#include <iostream>

using namespace std;
bool CacuMA(std::vector<strategy_4_kline>::iterator beg, std::vector<strategy_4_kline>::iterator &end, strategy_4__indicator_params& params);

int main(int argc, char ** argv){

	if (strcmp("angus", argv[1]) == 0) return -1;

	std::map<std::string,strategy_4_stock_klines> klines;
	strategy_4__indicator_params params;

	{
		boost::timer::auto_cpu_timer t;
		loadcsv(argv[2], klines);
	}
	
	std::map<std::string, strategy_4_stock_klines>::iterator klines_beg, klines_end, klines_iter;

	klines_beg = klines.begin();

	{

		boost::timer::auto_cpu_timer t;

		for (klines_iter = klines.begin(), klines_end = klines.end(); klines_iter != klines_end; ++klines_iter)
		{
			//std::cout << "process" << (klines_iter->first).c_str();

			std::vector<strategy_4_kline>::iterator kline_beg, kline_end, kline_iter;

			kline_beg = klines_iter->second.klines.begin();
			for (kline_iter = klines_iter->second.klines.begin(), kline_end = klines_iter->second.klines.end(); kline_iter != kline_end; ++kline_iter){

				CacuMA(kline_beg, kline_iter, params);
				//cout << kline_iter->kline.symbol << ",ma55=" <<  kline_iter->ma.longMA << ",ma18=" << kline_iter->ma.middleMA << ",ma7=" << kline_iter->ma.shortMA << endl;;

			}
		}
	}


	return 0;

}

bool CacuMA(std::vector<strategy_4_kline>::iterator beg, std::vector<strategy_4_kline>::iterator &end, strategy_4__indicator_params& params){
	double total = 0.0;
	double ma = 0.0;
	unsigned int count = 1;
	std::vector<strategy_4_kline>::iterator iter = end;

	if ((end - beg) <= params.ma_long)
	{
		return false;
	}
	else{
		while (count <=params.ma_long){
			total += iter->kline.close;
			if (count == params.ma_short){
				end->ma.shortMA = total / params.ma_short;
			}
			else if (count == params.ma_middle){
				end->ma.middleMA = total / params.ma_middle;
			}
			else if (count == params.ma_long){
				end->ma.longMA = total / params.ma_long;
			}
			count++;
			++iter;
		}
		return true;
	}
}