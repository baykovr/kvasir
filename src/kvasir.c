#include "kvasir.h"
#include "scan.h"

int main(int argc, char* argv[])
{
	const char* ACTIVE_MSG = "GET /fish.jpg?&t=4426 HTTP/1.1\nUser-Agent: Update\nHost: www.dianwofacai.com\nCache-Control: no-cache\r\n\r\n";

	printf("%d",scan("206.190.36.45", ACTIVE_MSG) );
	return 0;
}