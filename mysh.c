/* do stuff */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

char **path = NULL;

void errMsg(){

	char error_message[30] = "An error has occured\n";
	write(STDERR_FILENO, error_message, strlen(error_message));

}

void change(char *myarg[]){

	size_t d;
	for(d = 0; myarg[d]; d++){

		d = d;

	}

	if(d == 1){

		errMsg();

	} else if (d > 2) {

		errMsg();

	} else {

		int retValue = 2;

		retValue = chdir(myarg[1]);

		if(retValue == -1){

			errMsg();

		}

	}

}

void changeP(char *myarg[]){


	free(path);

	int size = 10;
	int counter = 0;

	path = malloc(sizeof(char*)*size);

	if(path == NULL){

		errMsg();
		free(path);
		exit(1);
	}

	size_t len;
	for(len = 0; myarg[len]; len++){
		len = len;
	}

	if(len == 1){

		path[0] = "NoDirectory";

	} else {

	int cSize = 0;
	for(size_t p = 1; myarg[p]; p++){

			counter++;

			if(counter == size){

				size = size + 1;
				path = realloc(path, sizeof(char*)*size);

					if(path == NULL){
						errMsg();
						free(path);
						exit(1);

					}
			}

			cSize = strlen(myarg[p]);

			path[p-1] = malloc(cSize + 2);
			strcpy(path[p-1], myarg[p]);
			strcat(path[p-1], "/");

		}

	}

}

void quit(char *myarg[]){
	size_t q;
	for(q = 0; myarg[q]; q++){

		q = q;

	}

	if(q == 1){

		exit(0);

	} else {

		errMsg();

	}

}

int checkBuilt(char *myarg[]){

	if(strcmp(myarg[0],"cd") == 0){

		change(myarg);
		return 0;

	} else if(strcmp(myarg[0], "path") == 0){

		changeP(myarg);
		return 0;

	} else if (strcmp(myarg[0],"exit") == 0){

		quit(myarg);
		return 0;

	} else {

		return 1;

	}

}

void tokenExe(char testS[]){

	char *str1, *str2, *token, *subtoken;
        char *saveptr1, *saveptr2;
	char **myarg;
	int size = 10;

	str1 = testS;
	int i = 0;
	int j;
	for (j = 1; ; j++, str1 = NULL){

		myarg = malloc(sizeof(char*)*size);

		if(myarg == NULL){

			errMsg();
			free(myarg);
			exit(1);

		}

		token = strtok_r(str1, "&", &saveptr1);

		if (token == NULL){

			break;

		}

		for (str2 = token; ; str2 = NULL){

			subtoken = strtok_r(str2, " \n\t\f\r\v", &saveptr2);

			if (subtoken == NULL){

				break;

			}

			myarg[i] = subtoken;

			i++;

			if(i == size){

				size = size + 1;

				myarg = realloc(myarg, sizeof(char*)*size);

				if(myarg == NULL){

					errMsg();
					free(myarg);
					exit(1);

				}

			}

               }

               if(checkBuilt(myarg)){

			int rc = fork();

			if(rc < 0){
				errMsg();
			}

			if(rc==0){

        			int check = -1;
        			size_t k;
        			char *launchPath;
        			int sizePath = 0;
        			int sizeArg = strlen(myarg[0]);

        			for(k = 0; path[k]; k++){

        				sizePath = strlen(path[k]);

        				launchPath = malloc(sizeArg + sizePath + 2);

        				strcat(launchPath, path[k]);
        				strcat(launchPath, myarg[0]);

        				check = access(launchPath, X_OK);

        				if(check == 0){

        					break;

        				} else {

        				free(launchPath);

        				}

        			}

				if(check == -1){

               				errMsg();
               				exit(1);

               			}


				for(size_t l = 0; myarg[l]; l++){

					if(strncmp(myarg[l],">",1)==0){

						if(strlen(myarg[l]) != 1){

							errMsg();
							exit(0);

						}

						if((l+2)==i){

							int newFd = open(myarg[l+1],O_WRONLY | O_CREAT | O_TRUNC, 0644);

							dup2(newFd,1);
							dup2(newFd,2);

        						char **redir = malloc(sizeof(char*)*i);
        						for(size_t m = 0; m < l; m++){

        							redir[m] = myarg[m];

        						}

        						execv(launchPath,redir);
        						exit(1);

						} else {

							errMsg();
							exit(0);

						}

					}

				}

        			execv(launchPath,myarg);

        			errMsg();

        		} else {
				//Uncomment for guaranteed correct order.
        			//wait(NULL);

        		}

		}


		free(myarg);
		i = 0;

	}

	int o = 0;
	//pid_t pid; if you need to check child IDs
	int status = 0;

	while(o < j){

		//pid = wait(&status); if you need to check child IDs
		wait(&status);
		o++;

	}

}




int main(int argc, char *argv[]){


	if(argc > 2){
		errMsg();
		exit(1);
	}

	if(argc < 2){

		char **init = malloc(sizeof(char*)*2);

		if(init == NULL){

			errMsg();
			free(init);
			exit(1);

		}

		init[0]= malloc(sizeof(char)*5);
		strcpy(init[0], "path");

		if(init == NULL){

			errMsg();
			free(init);
			exit(1);

		}

		init[1]= malloc(sizeof(char)*5);
		strcpy(init[1], "/bin");

		if(init == NULL){

			errMsg();
			free(init);
			exit(1);

		}

		changeP(init);




		char *line = NULL;
		size_t len = 0;
		ssize_t read;

		printf("%s", "mysh> ");

		while((read = getline(&line, &len, stdin)) != -1){


			if(feof(stdin)){
				exit(0);
			} else {
				tokenExe(line);
			}

			printf("%s", "mysh> ");

		}

	} else {

		char **init = malloc(sizeof(char*)*2);

		if(init == NULL){

			errMsg();
			free(init);
			exit(1);

		}

		init[0]= malloc(sizeof(char)*5);
		strcpy(init[0], "path");

		if(init == NULL){

			errMsg();
			free(init);
			exit(1);

		}

		init[1]= malloc(sizeof(char)*5);
		strcpy(init[1], "/bin");

		if(init == NULL){

			errMsg();
			free(init);
			exit(1);

		}

		changeP(init);



		FILE *fp;
		char *line = NULL;
		size_t len = 0;
		ssize_t read;


		fp = fopen(argv[1], "r");
		if(fp == NULL){
			errMsg();
			exit(1);
		}

		while((read = getline(&line, &len, fp)) != -1){

			if(feof(fp)){
				exit(0);
			} else {
				tokenExe(line);

			}

		}

	}

	return 0;
}
