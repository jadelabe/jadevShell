#include <sys/stat.h>
#include <stdio.h>

int changemode(const char* argv[]){
	if(chmod(argv[1], argv[2])==0){ 
		return 0;
	} else{
		return -1;
	}
}

int main( int argc, const char* argv[] )
{
	if(argc>=4){
		if(changemode($argv[])!=0){
			printf(stderr, "changemode error\n");
		}
	}
}
