#include <stdio.h>

int main()
{
FILE *fp;
 char str[100];

 fp = fopen("C:\\a.txt", "w");
 fputs("Hello, Psi!!", fp);

 fp = freopen("C:\\a.txt", "r", fp);
 fgets(str, 99, fp);

 printf("%s", str);
 return 0;
}
