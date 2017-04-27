#ifndef _HELPER_H_
#define _HELPER_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strategy.h"

char *strtok_r(char *s, const char *delim, char **save_ptr) {
	char *token;

	if (s == NULL) s = *save_ptr;

	/* Scan leading delimiters.  */
	s += strspn(s, delim);
	if (*s == '\0')
		return NULL;

	/* Find the end of the token.  */
	token = s;
	s = strpbrk(token, delim);
	if (s == NULL)
		/* This token finishes the string.  */
		*save_ptr = strchr(token, '\0');
	else {
		/* Terminate the token and make *SAVE_PTR point past it.  */
		*s = '\0';
		*save_ptr = s + 1;
	}

	return token;
}

bool loadcsv(const char* filename, std::map<std::string, strategy_4_stock_klines>& klines)
{
	FILE *fp;
#ifdef WIN32
	errno_t err = fopen_s(&fp, filename, "r");
	if (err != 0) return false;
#else
	fp = fopen(filename,"r");
	if (fp==NULL) return false;
#endif
	

	char line[1024];
	int count = 0;
	
	strategy_4_stock_klines stocklines;
	strategy_4_kline kl_record;
	std::map<std::string, strategy_4_stock_klines>::iterator it;

	while (fgets(line, sizeof(line), fp)){
		char *save_ptr;
		char *symbol = strtok_r(line, ",", &save_ptr);
		char *symbol1 = strtok_r(NULL, ",", &save_ptr);
		char *date = strtok_r(NULL, ",", &save_ptr);
		char *time = strtok_r(NULL, ",", &save_ptr);
		char *open = strtok_r(NULL, ",", &save_ptr);
		char *high = strtok_r(NULL, ",", &save_ptr);
		char *low = strtok_r(NULL, ",", &save_ptr);
		char *close = strtok_r(NULL, ",", &save_ptr);
		char *volume = strtok_r(NULL, ",", &save_ptr);
		char *amount = strtok_r(NULL, ",", &save_ptr);

		kl_record.kline.symbol = symbol;
		kl_record.kline.date = atoi(date);
		kl_record.kline.time = atoi(time)/1000;
		kl_record.kline.open = atof(open)/10000;
		kl_record.kline.high = atof(high) / 10000;
		kl_record.kline.low = atof(low) / 10000;
		kl_record.kline.close = atof(close) / 10000;
		kl_record.kline.volume = atof(volume);
		kl_record.kline.amount = atof(amount);

		kl_record.kdj = { 0, 0, 0, 0 };
		kl_record.ma = { 0, 0, 0, 0 };
		kl_record.macd = { 0, 0, 0, 0 };
		kl_record.sar = { 0, 0, 0 };

		it = klines.find(kl_record.kline.symbol);

		if (it == klines.end()){
			klines[kl_record.kline.symbol] = stocklines;
			klines[kl_record.kline.symbol].klines.push_back(kl_record);
		}
		else{
			klines[kl_record.kline.symbol].klines.push_back(kl_record);
		}

	}
	
	return true;
}

#endif