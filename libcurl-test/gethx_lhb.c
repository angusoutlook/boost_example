#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include <curl/curl.h>

const char * urls[]={"http://quote.eastmoney.com/stocklist.html",//东方财富网股票列表
	"http://hq.sinajs.cn/list=sh600000,sh600004",//新浪股票实时行情
	"http://vip.stock.finance.sina.com.cn/corp/go.php/vMS_MarketHistory/stockid/000001/type/S.phtml?year=2016&jidu=1",//新浪股票指数历史行情
	"http://vip.stock.finance.sina.com.cn/corp/go.php/vMS_MarketHistory/stockid/600000.phtml?year=2016&jidu=1",//新浪股票股票历史行情
	"http://vip.stock.finance.sina.com.cn/corp/go.php/vISSUE_ShareBonus/stockid/600000.phtml",//新浪财经股票除权除息
	
};
 
struct MemoryStruct {
  char *memory;
  size_t size;
};
 
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
  mem->memory = realloc(mem->memory, mem->size + realsize + 1);
  if(mem->memory == NULL) {
    /* out of memory! */ 
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}
 
int main(void)
{
  CURL *curl_handle;
  CURLcode res;
 
  struct MemoryStruct chunk;
 
  chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */ 
  chunk.size = 0;    /* no data at this point */ 
 
  curl_global_init(CURL_GLOBAL_ALL);
 
  /* init the curl session */ 
  curl_handle = curl_easy_init();
 
  /* specify URL to get */ 
  curl_easy_setopt(curl_handle, CURLOPT_URL, "http://quote.eastmoney.com/stocklist.html");
 
  /* send all data to this function  */ 
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
 
  /* we pass our 'chunk' struct to the callback function */ 
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
 
  /* some servers don't like requests that are made without a user-agent
 *      field, so we provide one */ 
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
 
  /* get it! */ 
  res = curl_easy_perform(curl_handle);
 
  /* check for errors */ 
  if(res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
  }
  else {
    /*
 *      * Now, our chunk.memory points to a memory block that is chunk.size
 *           * bytes big and contains the remote file.
 *                *
 *                     * Do something nice with it!
 *                          */ 
    
    printf("%lu bytes retrieved\n", (long)chunk.size);
    printf("content:\n%s\n",chunk.memory);
  }

  curl_easy_setopt(curl_handle,CURLOPT_URL,"http://hq.sinajs.cn/list=sh600000,sh600004");
  res = curl_easy_perform(curl_handle);
  if(res!=CURLE_OK){
    fprintf(stderr,"curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
  }
  else{
    printf("%lu bytes retrieved\n", (long)chunk.size);
    printf("content:\n%s\n",chunk.memory);

  }

  curl_easy_setopt(curl_handle,CURLOPT_URL,"http://data.10jqka.com.cn/market/longhu/data/2016-08015/");
  res=curl_easy_perform(curl_handle);
  if(res!=CURLE_OK){
    fprintf(stderr,"curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
  }
  else{
    printf("%lu bytes retrieved\n",(long)chunk.size);
    printf("content:\n%s\n",chunk.memory);
  }
 
  /* cleanup curl stuff */ 
  curl_easy_cleanup(curl_handle);
 
  free(chunk.memory);
 
  /* we're done with libcurl, so clean it up */ 
  curl_global_cleanup();
 
  return 0;
}
