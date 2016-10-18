#include "helper.h"

string Helper::getDate()
{ 
	char times[80];
	time_t rawTime;
	struct tm* timeInfo;

	time(&rawTime);
	timeInfo = localtime(&rawTime);
	strftime(times,sizeof(times)," %Y-%m-%d %I:%M:%S\n",timeInfo);
	 
	return string(times);
}

