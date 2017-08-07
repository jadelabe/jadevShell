#include <sys/stat.h>
#include <stdio.h>

int makedir(const char argv){
	if(mkdir(argv, ACCESSPERMS)==0){ 
		return 0;
	} else{
		return -1;
	}
}

int main( int argc, const char* argv[] )
{
	if(argc>=3){
		if(makedir(argv[1])!=0){
			printf(stderr, "makedir error\n");
		}
	}
}
