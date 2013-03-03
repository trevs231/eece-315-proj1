#include "../../definitions.h"

#include "../../fileGet.h"

// mock processes we may need.
void parse();
void process();

void test_doesDirExist(char* dir);
void test_doesFileExist(char* dir);
void test_tildaToHome(char* dir);
void test_homeToTilda(char* dir);
void test_getFilePath(char* fileName);
void test_getPaths(void);

// Main function.
// Gets the command line arguments from the user.
// ANd runs the desired commands.
int main(int argc, char* argv[]) {

	test_doesDirExist("/usr/bin/");
	doesDirExist(".");
	test_doesDirExist("../");

	test_doesFileExist("testrun");
	test_doesFileExist("/bin/ls");
	test_doesFileExist("../../shell.c");
	test_doesFileExist("/usr/bin");
	test_doesFileExist("lksjdf");
	test_doesFileExist("mkdir");

	test_tildaToHome("~/Documents");
	test_tildaToHome("/Documents");
	

	test_homeToTilda("/home/trevor/Downloads");
	test_homeToTilda("/usr/bin/soemthing/blah");

	
	test_getFilePath("ls");
	test_getFilePath("mkdir");
	test_getFilePath("firefox");
	test_getFilePath("somelksdjf");
	test_getFilePath("/bin/ls");
	test_getFilePath("../../shell.c");
	test_getFilePath("fileGetTest.c");
	test_getFilePath("~/.vimrc");

	//Can be used for more custom tests
/*
	
	while(true){
		printf("\nEnter a program / command (one param) or exit:\n");
		scanf("%s", command);
		if((strcmp(command, "exit") != 0)){
			test_getFilePath(command);
		}else{
			break;
		}
	}

*/
	return 0;
}

void test_doesDirExist(char* dir){
	printf("test_doesDirExist ? %s: ", dir);
	if(doesDirExist(dir)){
		printf("yes\n");
	}else{
		printf("no\n");
	}
}

void test_doesFileExist(char* dir){
	printf("test_doesFileExist ? %s: ", dir);
	if(doesFileExist(dir)){
		printf("yes\n");
	}else{
		printf("no\n");
	}
}

void test_tildaToHome(char* dir){
	char* newPath = tildaToHome(dir);
	printf("test_tildaToHome: %s -> %s\n", dir, newPath);
}

void test_homeToTilda(char* dir){
	char* newPath = homeToTilda(dir);
	printf("test_homeToTilda: %s -> %s\n", dir, newPath);
}

void test_getFilePath(char* fileName){
	printf("test_getFilePath: %s\n", fileName);
	char* filePath = getFilePath(fileName);
	if(filePath){
		printf("%s\n", filePath);
	}else{
		printf("File does not exist\n");
	}
}

void test_getPaths(void){
	printf("test_getPaths:\n");
	char** paths = getPaths();
	int i = 0;
	while(NULL != paths[i]){
		printf("%s\n", paths[i]);
		i++;
	}
}
