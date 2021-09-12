#include "types.h"
#include "user.h"

char buffer[1024];

int main(void)
{
	int n = draw(buffer,1024);
	if (n < 0)
		printf(1,"Error in fetching\n");
	else
	{
		int val = write(1,buffer,n);
		if (val != n)
			printf(1, "Error in drawing\n");
	}
	exit();
}