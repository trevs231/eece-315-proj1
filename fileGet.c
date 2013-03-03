/* fileGet.c
 *
 * EECE 315
 * Lab group B5
 *
 * Functions for retrieving/checking files 
 */

#include "definitions.h"
#include "fileGet.h"

/* Checks if directory exists
 * @return: false if no directory, true if exists
 */
bool doesDirExist(char* path){
   DIR* dir = opendir(path);
   if(!dir){
       return false;
   } else{
       return true;
   }
}

/* Checks if file exists
 * @return: false if file does not exist, 
 *	   or path is a directory. true if exists. 
 */
bool doesFileExist(char* path){
   int status = open(path, O_RDONLY);
   if( NEG_ONE == status || doesDirExist(path)){
       return false;
   } else{
       return true;
   }
}

/* If path begins with '~', it replaces it with the current absolute
 * HOME path.
 * @return: A newly allocated string with the full path.
 *   	  If path does not start with a ~, returns path, unaltered.
 */
char* tildaToHome(char* path){
   char* homeDir = getenv("HOME");
   char* fullPath;
   char* tempPath;
   int pathLength = strlen(path);

   if(pathLength && '~' == path[0]){
       tempPath = (char*) malloc(pathLength);
       strcpy(tempPath, &path[1]);
       fullPath = (char*) malloc((strlen(path)+strlen(homeDir)));
       strcpy(fullPath, homeDir); 
       strcat(fullPath, tempPath);
       free(tempPath);
       return fullPath;
   } else{
       return path;
   }
}

/* If path is in the HOME dir, replaces HOME with '~'
 * @return: A newly allocated string, with the abbreviated path
 *	   If path is not in HOME, returns path, unaltered.
 */
char* homeToTilda(char* path){
   char* homeDir = getenv("HOME");
   int homeLength = strlen(homeDir);
   int pathLength = strlen(path);
   char* tildaPath;

   if(strlen(path) >= homeLength && !strncmp(homeDir, path, homeLength)){
       tildaPath = (char*) malloc(pathLength - homeLength +1);
       strcpy(tildaPath, &path[homeLength -1]);	
       tildaPath[0] = '~';
       return tildaPath;
   } else{
       return path;
   }
}

/* Get the absolute path of a file. Searches first in PATH.
 * If not found in PATH, will check if fileName is already 
 * an absolute OR relative path.
 * Accepts absolute paths with '~' representing HOME
 * @param: string of the name or path of a file
 * @return: string of absolute path of the file if found. NULL otherwise.
 */
char* getFilePath(char* fileName){
   char** paths = getPaths();
   char* filePath = NULL;
   int i = 0;
   filePath = malloc(PATH_MAX*sizeof(char));

   while( paths[i] != NULL){
       strcpy(filePath, paths[i]);
       strcat(filePath, "/");
       strcat(filePath, fileName);

       if(doesFileExist(filePath)){
	   return filePath;
	}
 
 	i++;
   }
   
   //File was not found in PATH
   strcpy(filePath, fileName);
   filePath = tildaToHome(filePath);
   filePath = realpath(filePath, NULL);

   if( filePath != NULL ){
       return filePath;
   }

	return NULL;
}

/* @return: An array of strings, which are the PATH environment values
 * The last element in the array is set to NULL
 */
char** getPaths(){
   char* temp = getenv("PATH");
   char* pathEnv = (char*)malloc(strlen(temp));
   strcpy(pathEnv, temp);

   int numOfPaths = 1;	//Set to account for	
   int i;
   for(i = (strlen(pathEnv)-1); i > 0; i--){
       if(':' == pathEnv[i]){
	   numOfPaths++;
       }
   }

   char** paths = (char**)calloc(numOfPaths+1, sizeof(char*));
   paths[numOfPaths] = NULL;
   numOfPaths--;

   for(i = (strlen(pathEnv)-1); i > 0; i--){
       if(':' == pathEnv[i]){
 	   paths[numOfPaths] = &pathEnv[i+1];
	   numOfPaths--;
	   pathEnv[i] = '\0';
	}
   }

   paths[0] = &pathEnv[0];
   return paths;
}
