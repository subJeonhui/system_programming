/* 

   표준 출력(stdout) 을 myfile.txt 의 스트림으로 변경한다. 
   이 예제는 http://www.cplusplus.com/reference/clibrary/cstdio/freopen/
   에서 가져왔습니다. 

 */
#include <stdio.h>
int main ()
{
	freopen ("myfile.txt","w",stdout);
	printf ("This sentence is redirected to a file.");
	fclose (stdout);
	return 0;
}
