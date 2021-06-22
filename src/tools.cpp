#include "tools.h"
#include <cstdlib>
#include <algorithm>

Data parse(string in, char delim){
   string contain;
   string end = in;
   Data parsed;
   int index = 0;
   int separator = 0;
   int count = std::count(in.begin(),in.end(),delim);
   parsed.size = count;
   parsed.content = (string*)malloc(count);
   while(separator!=-1){
      // printf("<Count: %d, Index: %d>\n",count,index);
      separator = end.find(delim);
      contain = end.substr(0,separator);
      end = end.substr(separator+1);
      parsed.content[index] = contain;
      index++;
   }
   return parsed;
}