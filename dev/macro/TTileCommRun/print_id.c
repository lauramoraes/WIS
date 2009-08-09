#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>


int main(void)
{
	
int retcode;
	printf("%s%c%c\n", "Content-Type:text/html;charset=iso-8859-1",13,10);


    printf("Real UID\t= %d\n", getuid());
    printf("Effective UID\t= %d\n", geteuid());
    printf("Real GID\t= %d\n", getgid());
    printf("Effective GID\t= %d\n", getegid());
    
  retcode=  setreuid(13977,13977);
if(retcode != 0)
	printf("<br><br>TRY USE setreuid <br>error: %s\n<br>", strerror(errno));
   retcode= setregid(1307, 1307);
if(retcode != 0)
	printf("TRY USE setregid<br >error: %s\n<br>", strerror(errno));

	printf("Real UID\t= %d\n", getuid());
    printf("Effective UID\t= %d\n", geteuid());
    printf("Real GID\t= %d\n", getgid());
    printf("Effective GID\t= %d\n", getegid());

    return EXIT_SUCCESS;
}
