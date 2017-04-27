// csv-test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <iostream>     
#include <boost/format.hpp>     
#include <boost/tokenizer.hpp>     
#include <boost/algorithm/string.hpp>     

int main(int argc,char ** argv)
{
	std::wcout.imbue(std::locale("chs"));
	//待分割的字符串     
	std::wstring strTag = _T("I Come from China");

	std::vector<std::wstring> vecSegTag;
	// boost::is_any_of这里相当于分割规则了     
	boost::split(vecSegTag, strTag, boost::is_any_of(_T(" ,，")));

	for (size_t i = 0; i<vecSegTag.size(); i++)
	{
		std::wcout << vecSegTag[i] << std::endl;
	}

	vecSegTag.clear();
	std::wstring strTag2 = _T("我叫小马，你呢,今天天气不错，我很高兴");
	boost::split(vecSegTag, strTag2, boost::is_any_of(_T(" ,，")));

	for (size_t i = 0; i<vecSegTag.size(); i++)
	{
		std::wcout << vecSegTag[i] << std::endl;
	}

	getchar();
	return 0;
}
