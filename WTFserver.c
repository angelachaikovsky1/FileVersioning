#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include<pthread.h>
#include<signal.h>
//server
struct manifestLL{
    char mode;
    char * version;
    char * path;
    char * oldHash;
    char * liveHash;
    struct manifestLL * next;
};
void error(char *msg);
int create(char * buffer, int buffSize);
int creation(int fileDes);
int commition(int fileDes);
void * commandExtract(void* fd2);
int appendToEnd(char * fileName, char * entryToAppend);
int findCurrentVer(int fileDes);
int findHistory(int fileDes);
int sendCheckout(int fileDes);
int returnContent(char * fileName, char ** charContent);
int executeRollback(int fileDes);
int tarProject(char * projectName);
int writeToFileX(struct manifestLL * commitHeader, char * fileName, char * version);
int pushing(int fileDes);
int extractProject2(char * projectName);
int destruction(int fileDes);
int upgrading(int fileDes);
int updating(int fileDes);
int tarProject2(char * projectName, struct manifestLL * head);
struct manifestLL * getCommitLL(int sizeOfFile, char * fileContent);
int tarProject3(char * projectName, struct manifestLL * head);
int remove_directory(char *path);
struct manifestLL * getLL(int sizeOfFile, char * fileContent, char ** version);
//void * exitThread();

void sigHand(int signalnum){
    printf("\nEnded connection with clients\n");
    (void)signal(SIGINT,SIG_DFL);
    //close(sockfd);
    exit(0);
}
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char const *argv[])
{
    //char cwdPath[400];
    //getwd(cwdPath);
   // printf("server path: %s\n", cwdPath);
    (void) signal(SIGINT, sigHand);
    int sockfd, newsockfd, portno;

    struct sockaddr_in serv_addr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;
    //int n;
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        error("ERROR opening socket");
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        error("ERROR on binding");
    }
   listen(sockfd,50);
    pthread_t tid[60];
    int i = 0;

    while(1){
        addr_size=sizeof serverStorage;
        newsockfd = accept(sockfd, (struct sockaddr *) &serverStorage, &addr_size) ;
        if (newsockfd < 0){
            //error("ERROR on accept");
            break;
        }
      //  pthread_mutex_lock(&lock);
        if( pthread_create(&tid[i], NULL, commandExtract, &newsockfd) != 0 ){

           printf("Failed to create thread\n");

           }
      //   pthread_mutex_unlock(&lock);
        if( i >= 50)
        {
          i = 0;
          while(i < 50)
          {
            pthread_join(tid[i++],NULL);
          }
          i = 0;
        }

    }
   // int s = atexit(exitThread);
  return 0;

}
/*)void * exitThread(){
    printf("Request has been processed!\n");
    exit(0);
}*/

void * commandExtract(void * fd2){
   //pthread_mutex_lock(&lock);
   //pthread_mutex_unlock(&lock);
    int n;
    int * newsockfd = fd2;
    int bytesRead = 0;
    int i = 0;
    char * command = (char *)malloc(5*sizeof(char));
    bzero(command,5);
    while(1){
        //loop exists until certain conditions are met
        n = read(*newsockfd, (command + bytesRead), 1);
        bytesRead = bytesRead + n;
        i++;
        if(i == 4){
            //command has been read in
            command[i] = '\0';
            break;
        }
    }
    //printf("%s\n", command);
    //call functions based on what command is


    while(pthread_mutex_trylock(&lock) != 0){

    }

   // sleep(10);
    if(strcmp(command, "cte:") == 0){
       // pthread_mutex_lock(&lock);
        int returnVal = creation(*newsockfd);
       
           // pthread_mutex_unlock(&lock);// printf("Successful creation of project");
    }else if(strcmp(command, "cvn:")==0){
        //pthread_mutex_lock(&lock);
             int returnVal = findCurrentVer(*newsockfd);
       
       // pthread_mutex_unlock(&lock);
    }else if(strcmp(command, "hst:")==0){
        //pthread_mutex_lock(&lock);
             int returnVal = findHistory(*newsockfd);
       
       // pthread_mutex_unlock(&lock);
    }else if(strcmp(command, "ckt:")==0){
        //pthread_mutex_lock(&lock);
        int returnVal= sendCheckout(*newsockfd);
      
    //pthread_mutex_unlock(&lock);

    }else if(strcmp(command, "rlb:")==0){
       // pthread_mutex_lock(&lock);
        int returnVal= executeRollback(*newsockfd);
       

   // pthread_mutex_unlock(&lock);
    }else if(strcmp(command, "cmt:") == 0){
        int returnVal = commition(*newsockfd);
       
       // printf("Successful creation of project");
    }else if(strcmp(command, "dst:") == 0){
        int returnVal = destruction(*newsockfd);
        
    }else if(strcmp(command, "psh:") == 0){
        int returnVal = pushing(*newsockfd);
       
    }else if(strcmp(command, "upg:") == 0){
        int returnVal = upgrading(*newsockfd);
       

    }else if(strcmp(command, "upd:") == 0){
        updating(*newsockfd);
        

    }
    //close(newsockfd);
    //
   // pthread_mutex_unlock(&lock);
    free(command);
    pthread_mutex_unlock(&lock);
    //pthread_exit(NULL);
  return (void *)0;
   // return;
}

int creation(int fileDes){
    int n;
    int newsockfd = fileDes;

    int numToRead = 0;
    int i = 0;

    char projectNameSize[100];
    bzero(projectNameSize, 100);
    int totalBytes = 0;
    while(1){
        n = read(newsockfd,(projectNameSize + totalBytes),1);
        totalBytes = totalBytes + n;

        if (n < 0){
            error("ERROR reading from socket\n");
        }

        if(projectNameSize[i] == ':'){
            projectNameSize[i] = '\0';
            break;
        }
        i++;
    }

    //the number of bytes to read for the name plus null
    numToRead = atoi(projectNameSize) + 1;
    char * projectName = (char *)malloc(numToRead*sizeof(char));
    bzero(projectName, numToRead);
    n = read(newsockfd,projectName, (numToRead - 1));
    projectName[numToRead - 1] = '\0';



    char * projectPath = (char *)malloc((3+numToRead)*sizeof(char));
    strcpy(projectPath, "./");
    strcat(projectPath, projectName);
    projectPath[2 + numToRead] = '\0';

    //root already exists
    //printf("project name: %s\n", projectPath);
    DIR * dir2 = opendir(projectPath);
    if(dir2 != NULL){
        printf("project requested already exists\n");
        closedir(dir2);
         n = write(newsockfd, "fls:", 4);
        return -1;
    }

    int check = mkdir(projectPath, 0777);
    if(check < 0){
        printf("error in making the project \n");
         n = write(newsockfd, "fls:", 4);
        return -1;
    }

    char * manifest = (char *)malloc((numToRead + 9 + 7)*sizeof(char));
    strcpy(manifest, projectPath);
    strcat(manifest, "/");
    strcat(manifest, ".Manifest");
    manifest[(numToRead + 9 + 6)] = '\0';
    printf("manifest location: %s\n", manifest);
    int fd = open(manifest, O_CREAT | O_WRONLY , 00600);
    if(fd < 0){
        printf("There was an error making a .Manifest file\n");
        n = write(newsockfd, "fls:", 4);
        return -1;
    }
    write(fd, "1", 1);
    write(fd, "\n", 1);
    close(fd);

    char * completeManifest = (char *)malloc(9 * sizeof(char));
    strcpy(completeManifest, "cte:2:1\n");
    completeManifest[8] = '\0';
    n = write(newsockfd,completeManifest,9);
    if(n < 0){
        printf("there was an issue writing back to client\n");
         n = write(newsockfd, "fls:", 4);
        return -1;
    }

    return 0;
}
int findCurrentVer(int fileDes){
   // printf("n\n");
     int n;
    int newsockfd = fileDes;

    int numToRead = 0;
    int i = 0;

    char projectNameSize[500];
    bzero(projectNameSize, 500);
    int totalBytes = 0;
    while(1){
        n = read(newsockfd,(projectNameSize + totalBytes),1);
        totalBytes = totalBytes + n;

        if (n < 0){
            n = write(newsockfd, "fls:", 4);
            error("ERROR reading from socket");
            return -1;
        }

        if(projectNameSize[i] == ':'){
            projectNameSize[i] = '\0';
            break;
        }
        i++;
    }
    numToRead = atoi(projectNameSize) + 3;
    char * projectName = (char *)malloc(numToRead*sizeof(char));
    bzero(projectName, numToRead);
    strcpy(projectName, "./");
    n = read(newsockfd,projectName + 2, numToRead - 3);
    if(n < 0){
        printf("problems reading from socket\n");
        n = write(newsockfd, "fls:", 4);
        return -1;
    }

    DIR * dir = opendir(projectName);
    if(projectName== NULL){
        n = write(newsockfd, "fls:", 4);
        return -1;
    }
    closedir(dir);
    char * manifest = (char *)malloc((numToRead + 10)*sizeof(char));
    bzero(manifest, (numToRead+10));
    strcpy(manifest, projectName);
    strcat(manifest, "/.Manifest");

    char * fileContent = NULL;
    int sizeToSend = returnContent(manifest, &fileContent);
    if(sizeToSend == -1){
        printf("Something is wrong with the .Manifest file\n");
       n = write(newsockfd, "fls:", 4);
        return -1;
    }

    char * sizeOfLength = (char *)malloc(100*sizeof(char));
    bzero(sizeOfLength, 100);
    sprintf(sizeOfLength, "%d", sizeToSend);
    int sizeLen = strlen(sizeOfLength);

    int sendLen = 5 + sizeLen + sizeToSend + 1;
    char * sendString = (char *)malloc(sendLen*sizeof(char));
    strcpy(sendString, "cvn:");
    strcat(sendString, sizeOfLength);
    strcat(sendString, ":");
    strcat(sendString, fileContent);
    sendString[sendLen - 1] = '\0';
    free(sizeOfLength);
    free(manifest);
    free(fileContent);

    n = write(newsockfd, sendString, sendLen);
    if(n < 0){
        printf("There was an issue sending .Manifest to client\n");
        n = write(newsockfd, "fls:", 4);
        return -1;
    }
return 0;

}
int findHistory(int fileDes){
     int n;
    int newsockfd = fileDes;

    int numToRead = 0;
    int i = 0;

    char projectNameSize[500];
    bzero(projectNameSize, 500);
    int totalBytes = 0;
    while(1){
        n = read(newsockfd,(projectNameSize + totalBytes),1);
        totalBytes = totalBytes + n;

        if (n < 0){
            n = write(newsockfd, "fls:", 4);
            error("ERROR reading from socket");
            return -1;
        }

        if(projectNameSize[i] == ':'){
            projectNameSize[i] = '\0';
            break;
        }
        i++;
    }
    numToRead = atoi(projectNameSize) + 3;
    char * projectName = (char *)malloc(numToRead*sizeof(char));
    bzero(projectName, numToRead);
    strcpy(projectName, "./");
    n = read(newsockfd,projectName + 2, numToRead - 3);
    if(n < 0){
        printf("problems reading from socket\n");
       n = write(newsockfd, "fls:", 4);
        return -1;
    }

    DIR * dir = opendir(projectName);
    if(projectName== NULL){
        n = write(newsockfd, "fls:", 4);
        return -1;
    }
    closedir(dir);
    char * history = (char *)malloc((numToRead + 9)*sizeof(char));
    bzero(history, (numToRead+9));
    strcpy(history, projectName);
    strcat(history, "/.History");

    char * fileContent = NULL;
    int sizeToSend = returnContent(history, &fileContent);
    if(sizeToSend == -1){
        printf("Something is wrong with the .History file\n");
        n = write(newsockfd, "fls:", 4);
        return -1;
    }

    char * sizeOfLength = (char *)malloc(100*sizeof(char));
    bzero(sizeOfLength, 100);
    sprintf(sizeOfLength, "%d", sizeToSend);
    int sizeLen = strlen(sizeOfLength);

    int sendLen = 5 + sizeLen + sizeToSend + 1;
    char * sendString = (char *)malloc(sendLen*sizeof(char));
    strcpy(sendString, "hst:");
    strcat(sendString, sizeOfLength);
    strcat(sendString, ":");
    strcat(sendString, fileContent);
    sendString[sendLen - 1] = '\0';
    free(sizeOfLength);
    free(history);
    free(fileContent);

    n = write(newsockfd, sendString, sendLen);
    if(n < 0){
        printf("There was an issue sending .History to client\n");
        n = write(newsockfd, "fls:", 4);
        return -1;
    }
return 0;
}
int returnContent(char * fileName, char ** charContent){
    int fd = open(fileName, O_RDONLY);
    if(fd < 0){
        return -1;
    }
    int size = 10;
    int sizeToReturn = 0;
    char * holder = (char *)malloc(size *sizeof(char));

    int bytes = 0;
    int totalBytes = 0;
    while(1){
        bytes = read(fd, holder + totalBytes, size - totalBytes);
        if(bytes == -1){
            free(holder);
            return -1;
        }

        totalBytes = totalBytes + bytes;
        if(totalBytes == size){
            size = size * 2;
            char * temp = (char *)malloc(size * sizeof(char));
            memcpy(temp, holder, totalBytes);
            free(holder);
            holder = temp;
        }
         if(bytes == 0){

            break;
        }
    }
    if(totalBytes == 0){
        holder[0] = '\0';
    }else{
        holder[totalBytes] = '\0';
    }

    sizeToReturn = totalBytes;
    *charContent = holder;
    close(fd);
    return sizeToReturn;
}
int sendCheckout(int fileDes){
      int n;
    int newsockfd = fileDes;

    int numToRead = 0;
    int i = 0;

    char projectNameSize[500];
    bzero(projectNameSize, 500);
    int totalBytes = 0;
    while(1){
        n = read(newsockfd,(projectNameSize + totalBytes),1);
        totalBytes = totalBytes + n;

        if (n < 0){
            n = write(newsockfd, "fls:", 4);
            error("ERROR reading from socket");
            return -1;
        }

        if(projectNameSize[i] == ':'){
            projectNameSize[i] = '\0';
            break;
        }
        i++;
    }

    numToRead = atoi(projectNameSize) + 3;
    char * projectName = (char *)malloc(numToRead*sizeof(char));
    bzero(projectName, numToRead);
    strcpy(projectName, "./");
    n = read(newsockfd,projectName + 2, numToRead - 3);
    if(n < 0){
        printf("problems reading from socket\n");
        n = write(newsockfd, "fls:", 4);
        return -1;
    }
       // printf("%s\n", projectName);
    DIR * dir = opendir(projectName);
    // printf("%d\n", dir);
    if(dir == NULL){
        printf("Unfortunately, project does not exist on the server. Please try a valid project!\n");
        n = write(newsockfd, "fls:", 4);
        return -1;
    }
    char * project = (char *)malloc((strlen(projectName)-2)*sizeof(char));
    bzero(project, (strlen(projectName)-2));
    int j=0;
    for(j=0;j<strlen(projectName); j++){
        if(projectName[j]!= '.'&& projectName[j]!='/'){
            project[j-2]=projectName[j];
        }
    }
   tarProject(project);
    char * wholeProj = (char *)malloc((numToRead + 7)*sizeof(char));
    bzero(wholeProj, (numToRead+7));
    strcpy(wholeProj, projectName);
    strcat(wholeProj, ".tar.gz");

    char * fileContent = NULL;
    int sizeToSend = returnContent(wholeProj, &fileContent);
    if(sizeToSend == -1){
        printf("Something is wrong with the compressed tar file\n");
        n = write(newsockfd, "fls:", 4);
        return -1;
    }
      //printf("%s\n", wholeProj);
       /* int fd = open(wholeProj, O_CREAT | O_WRONLY , 00600);
        int bytes = write(fd, fileContent, sizeToSend);
        if(bytes<0){
            printf("There was an error writing to the file\n");
            close(fd);
            return -1;

        }

    return 0;*/
    char * sizeOfLength = (char *)malloc(100*sizeof(char));
    bzero(sizeOfLength, 100);
    sprintf(sizeOfLength, "%d", sizeToSend);
    int sizeLen = strlen(sizeOfLength);

    int sendLen = 5 + sizeLen+1;
    char * sendString = (char *)malloc(sendLen*sizeof(char));
    strcpy(sendString, "ckt:");
    strcat(sendString, sizeOfLength);
    strcat(sendString, ":");
    sendString[sendLen - 1] = '\0';
    free(sizeOfLength);

    free(fileContent);

    n = write(newsockfd, sendString, sendLen-1);
    n=write(newsockfd, fileContent, sizeToSend);

    if(n < 0){
        printf("There was an issue sending the checkout to client\n");
        n = write(newsockfd, "fls:", 4);
        return -1;
    }
    remove(wholeProj);
 free(wholeProj);
return 0;
}
int executeRollback(int fileDes){
 int n;
    int newsockfd = fileDes;

    int numToRead = 0;
    int i = 0;

    char projectNameSize[500];
    bzero(projectNameSize, 500);
    int totalBytes = 0;
    while(1){
        n = read(newsockfd,(projectNameSize + totalBytes),1);
        totalBytes = totalBytes + n;

        if (n < 0){
            error("ERROR reading from socket");
        }

        if(projectNameSize[i] == ':'){
            projectNameSize[i] = '\0';
            break;
        }
        i++;
    }

    numToRead = atoi(projectNameSize) + 3;
    char * projectName = (char *)malloc(numToRead*sizeof(char));
    bzero(projectName, numToRead);
    strcpy(projectName, "./");
    n = read(newsockfd,projectName + 2, numToRead - 3);
    if(n < 0){
        printf("problems reading from socket\n");
       n = write(newsockfd, "fls:", 4);
        return -1;
    }
       // printf("%s\n", projectName);
    DIR * dir = opendir(projectName);
    // printf("%d\n", dir);
    if(dir == NULL){
        printf("Unfortunately, the project or this version of the project does not exist on the server. Please try a valid project or project version!\n");
        n = write(newsockfd, "fls:", 4);
        return -1;
    }
    closedir(dir);
    int projectManLength = strlen(projectName)+10;
    char * projectManifest = (char *)malloc(projectManLength * sizeof(char));
    bzero(projectManifest, projectManLength);
    strcpy(projectManifest, projectName);
    strcat(projectManifest, "/.Manifest");
   char * fileContent = NULL;
    int manSize = returnContent(projectManifest, &fileContent);
    if(manSize == -1){
        printf("Something is wrong with the version's .Manifest file\n");
        return -1;
    }
    char * projVersion = (char *)malloc(20*sizeof(char));
    bzero(projVersion, 20);
    i=0;
    while(fileContent[i]!='\n'){
        projVersion[i]=fileContent[i];
        i++;
    }
    int realProjLen = strlen(projectName)-strlen(projVersion);
    char * project= (char *)malloc(realProjLen*sizeof(char));
    i=0;
    for(i=0; i<realProjLen;i++){
        project[i]=projectName[i];

    }
    int versionNum = atoi(projVersion);
    DIR * dir2;
    dir2 = opendir(project);
    closedir(dir2);
    remove_directory(project);
    while(dir2!=NULL){

         versionNum++;
        bzero(projVersion, 20);
        //itoa(versionNum, projVersion, 10);
        sprintf(projVersion, "%d", versionNum);
        char * newerProject = (char *)malloc((strlen(project)+strlen(projVersion))*sizeof(char));
        strcpy(newerProject, project);
        strcat(newerProject, projVersion);
        dir2 = opendir(newerProject);

        if(dir2!=NULL){
        closedir(dir2);
        remove_directory(newerProject);
        }
        bzero(newerProject, strlen(project)+strlen(projVersion));


    }

    rename(projectName, project);
    char * sizeOfLength = (char *)malloc(100*sizeof(char));
    bzero(sizeOfLength, 100);
    sprintf(sizeOfLength, "%d", 8);
    int sizeLen = strlen(sizeOfLength);

    int sendLen = 5 + sizeLen + 8 + 1;
    char * sendString = (char *)malloc(sendLen*sizeof(char));
    strcpy(sendString, "rlb:");
    strcat(sendString, sizeOfLength);
    strcat(sendString, ":");
    strcat(sendString, "Success!");
    sendString[sendLen - 1] = '\0';
    free(sizeOfLength);



    n = write(newsockfd, sendString, sendLen);
    if(n < 0){
        printf("There was an issue sending rollback feedback to client\n");
        n = write(newsockfd, "fls:", 4);
        return -1;
    }

return 0;


}
int updating(int fileDes){
    int n;
    int newsockfd = fileDes;
    char projectNameSize[500];
    bzero(projectNameSize, 500);
    int totalBytes = 0;
    int i = 0;
    while(1){
        n = read(newsockfd,(projectNameSize + totalBytes),1);
        totalBytes = totalBytes + n;

        if (n < 0){
            error("ERROR reading from socket");
        }

        if(projectNameSize[i] == ':'){
            projectNameSize[i] = '\0';
            break;
        }
        i++;
    }
    int numToRead = atoi(projectNameSize) + 3;
    char * projectName = (char *)malloc(numToRead*sizeof(char));
    bzero(projectName, numToRead);
    strcat(projectName, "./");

    n = read(newsockfd, projectName+2, numToRead - 3);
    if(n < 0){
        printf("error reading from socket\n");
        return -1;
    }


    DIR * dir = opendir(projectName);
    if(dir == NULL){
        n = write(newsockfd, "fls:", 4);
        if (n < 0){
            printf("Failed to write back to client\n");
            return -1;
        }
        return -1;
    }
    closedir(dir);
    n = write(newsockfd, "upd:", 4);
    if(n < 0){
        return -1;
    }


    char * manifest = (char *)malloc((numToRead + 10)*sizeof(char));
    bzero(manifest, (numToRead+10));
    strcpy(manifest, projectName);
    strcat(manifest, "/.Manifest");

    char * fileContent = NULL;
    int sizeToSend = returnContent(manifest, &fileContent);
    if(sizeToSend == -1){
        printf("Something is wrong with the .Manifest file\n");
        n = write(newsockfd, "fls:", 4);
        return -1;
    }

    char sizeOfLength[100];
    bzero(sizeOfLength, 100);
    sprintf(sizeOfLength, "%d", sizeToSend);
    int sizeLen = strlen(sizeOfLength);

    int sendLen = 5 + sizeLen + sizeToSend + 1;
    char * sendString = malloc(sendLen*sizeof(char));
    strcpy(sendString, "upd:");
    strcat(sendString, sizeOfLength);
    strcat(sendString, ":");
    strcat(sendString, fileContent);
    sendString[sendLen - 1] = '\0';
    if(fileContent != NULL){
         free(fileContent);
    }
    n = write(newsockfd, sendString, strlen(sendString));
    if(n < 0){
        printf("There was an issue sending .Manifest to client\n");
        return -1;
    }

    return 0;
}

int upgrading(int fileDes){
    int n;
    int newsockfd = fileDes;
    char projectNameSize[500];
    bzero(projectNameSize, 500);
    int totalBytes = 0;
    int i = 0;
    while(1){
        n = read(newsockfd,(projectNameSize + totalBytes),1);
        totalBytes = totalBytes + n;

        if (n < 0){
            error("ERROR reading from socket");
        }

        if(projectNameSize[i] == ':'){
            projectNameSize[i] = '\0';
            break;
        }
        i++;
    }
    int numToRead = atoi(projectNameSize) + 3;
    char * projectName = (char *)malloc(numToRead*sizeof(char));
    bzero(projectName, numToRead);
    strcat(projectName, "./");

    n = read(newsockfd, projectName+2, numToRead - 3);
    if(n < 0){
        printf("error reading from socket\n");
        return -1;
    }


    DIR * dir = opendir(projectName);
    if(dir == NULL){
        n = write(newsockfd, "fls:", 4);
        if (n < 0){
            printf("Failed to write back to client\n");
            return -1;
        }
        return -1;
    }
    n = write(newsockfd, "upg:", 4);
    if(n < 0){
        return -1;
    }
    closedir(dir);

    char com[5];
    com[4] = '\0';

    n = read(newsockfd, com, 4);
    if(n < 0){
        return -1;
    }
    char updateSize[500];
    bzero(updateSize, 500);

    i = 0;
    while(1){
        n = read(newsockfd, updateSize + i, 1);
        if(n < 0){
            return -1;
        }
        if(updateSize[i] == ':'){
            updateSize[i] = '\0';
            break;
        }
        i++;
    }
    int uSize = atoi(updateSize) + 1;
    char * uContent = malloc(uSize*sizeof(char));
    bzero(uContent, uSize);
    n = read(newsockfd, uContent, uSize-1);

    struct manifestLL * updateHead = getCommitLL(uSize - 1, uContent);
    tarProject3(projectName, updateHead);
    //append .tar.gz to proejctName
    strcat(projectName, ".tar.gz");

    char * tarContent = NULL;
    int tarSize = returnContent(projectName, &tarContent);
    char charTarSize[200];
    bzero(charTarSize, 200);
    sprintf(charTarSize, "%d", tarSize);
    int sendSize = 6+strlen(projectName) + strlen(charTarSize);
    char * sendOver = (char *)malloc(sendSize*sizeof(char));
    strcpy(sendOver, "upg:");
    strcat(sendOver, projectName);
    strcat(sendOver, ":");
    strcat(sendOver, charTarSize);
    strcat(sendOver, ":");
    //strcat(sendOver, tarContent);
    n = write(newsockfd, sendOver, sendSize);
    if(n == -1){
        return -1;
    }
    n = write(newsockfd, tarContent, tarSize);
    if(n == -1){
        return -1;
    }



    return 0;
}


int pushing(int fileDes){
    int n;
    int newsockfd = fileDes;
    char projectNameSize[500];
    bzero(projectNameSize, 500);
    int totalBytes = 0;
    int i = 0;
    while(1){
        n = read(newsockfd,(projectNameSize + totalBytes),1);
        totalBytes = totalBytes + n;

        if (n < 0){
            error("ERROR reading from socket");
        }

        if(projectNameSize[i] == ':'){
            projectNameSize[i] = '\0';
            break;
        }
        i++;
    }
    int numToRead = atoi(projectNameSize) + 3;
    char * projectName = (char *)malloc(numToRead*sizeof(char));
    bzero(projectName, numToRead);
    strcat(projectName, "./");

    n = read(newsockfd, projectName+2, numToRead - 3);
    if(n < 0){
        printf("error reading from socket\n");
        return -1;
    }


    DIR * dir = opendir(projectName);
    if(dir == NULL){
        n = write(newsockfd, "fls:", 4);
        if (n < 0){
            printf("Failed to write back to client\n");
            return -1;
        }
        return -1;
    }
    closedir(dir);
    n = write(newsockfd, "psh:", 4);
    if(n < 0){
        return -1;
    }
    //now check for the hashNum recieved
    char com[5];
    n = read(newsockfd, com, 4);
    if(n < 0){
        return -1;
    }
    com[4] = '\0';
    //read in hash size
    i = 0;
    char hashSize[200];
    bzero(hashSize, 200);
    while(1){
        n = read(newsockfd, hashSize + i, 1);
        if( n < 0){
            return -1;
        }
        if(hashSize[i] == ':'){
            hashSize[i] = '\0';
            break;
        }
        i++;
    }
    int hSize = atoi(hashSize);
    char hash[500];
    bzero(hash, 500);
    n = read(newsockfd, hash, hSize);
    if(n < 0){
        return -1;
    }
    char commitFile[1000];
    bzero(commitFile, 1000);
    strcpy(commitFile, projectName);
    strcat(commitFile, "/.c/");
    strcat(commitFile, hash);

    int fd = open(commitFile, O_RDONLY);
    if(fd < 0){
        printf("the commit file does not exist, pushing failed\n");
        n = write(newsockfd, "fls:", 4);
        if(n < 0){
            return -1;
        }
        return -1;
    }
    close(fd);

    char * commitContent = NULL;
    int sizeCommit = returnContent(commitFile, &commitContent);
    //make ll of commit content
    struct manifestLL * commitHead = getCommitLL(sizeCommit, commitContent);
    n = write(newsockfd, "psh:", 4);
    if(n < 0){
        return -1;
    }
    //get the tar
    //check for previous history if exists get it
    char history[500];
    bzero(history, 500);
    strcpy(history, projectName);
    strcat(history, "/.History");

    char * hContent  = NULL;
    hSize = 0;
    int t = open(history, O_RDWR);
    if(t != -1){
        //grab history content
        close(t);
        hSize = returnContent(history, &hContent);
    }
    //get full content now
    hSize = sizeCommit + hSize;
    char * temp = malloc((hSize)*sizeof(char));
    bzero(temp, hSize);
    if(t != -1){
        strcpy(temp, hContent);
        strcat(temp, commitContent);
    }else{
        strcpy(temp, commitContent);
    }
    if(hContent != NULL){
        free(hContent);
    }
    hContent = temp;
    //psh:tarredname:fileSize:fileContent
    n = read(newsockfd, com, 4);
    if(n < 0){
        return -1;
    }
    char tarName[500];
    bzero(tarName, 500);
    i = 0;
    while(1){
        n = read(newsockfd, tarName + i, 1);
        if(n < 0){
            return -1;
        }
        if(tarName[i] == ':'){
            tarName[i] = '\0';
            break;
        }
        i++;
    }
    char tarSize[200];
    bzero(tarSize, 200);
    i = 0;
    while(1){
        n = read(newsockfd, tarSize + i, 1);
        if(n < 0){
            return -1;
        }
        if(tarSize[i] == ':'){
            tarSize[i] = '\0';
            break;
        }
        i++;
    }
    int actualTarSize = atoi(tarSize);
    char * tarContent = (char *)malloc(actualTarSize*sizeof(char));
    n = read(newsockfd, tarContent, actualTarSize);
    if(n < 0){
        return -1;
    }

    fd = open(tarName, O_RDWR | O_CREAT, 00600);
    if(fd < 0){
        n = write(newsockfd, "fls:", 4);
        return -1;
    }

    n = write(fd, tarContent, actualTarSize);
    close(fd);
    bzero(commitFile, 1000);
    strcpy(commitFile, projectName);
    strcat(commitFile, "/.c");
    remove_directory(commitFile);
    bzero(commitFile, 1000);
    strcpy(commitFile, projectName);
    strcat(commitFile, "/.Manifest");
    fd = open(commitFile, O_RDONLY);
    if (fd < 0){
        n = write(newsockfd, "fls:", 4);
        if(n < 0){
            return -1;
        }
        return -1;
    }
    char maniVersion[200];
    bzero(maniVersion, 200);
    i = 0;
    while(1){
        n = read(fd, maniVersion + i, 1);
        if(maniVersion[i] == '\n'){
            maniVersion[i] = '\0';
            break;
        }
        i++;
    }
    close(fd);
    bzero(commitFile, 1000);
    strcpy(commitFile, projectName);
    //rename project to its old version
    strcat(commitFile, maniVersion);


    char manifestName[500];
    bzero(manifestName, 500);
    strcpy(manifestName, projectName);
    strcat(manifestName, "/.Manifest");

    char * manifestContent = NULL;
    int sizeManifest = returnContent(manifestName, &manifestContent);
    char * version = NULL;
    struct manifestLL * manifestHead = getLL(sizeManifest, manifestContent, &version);
    int v = atoi(version);
    v++;
    char s[200];
    bzero(s, 200);
    sprintf(s, "%d", v);

    rename(projectName, commitFile);



    extractProject2(tarName);
    remove(tarName);



    //unlink(manifestName);
    fd = open(manifestName, O_RDWR | O_CREAT, 00600);
    if(fd < 0){
        n = write(newsockfd, "fls:", 4);
        return -1;
    }
    n = write(fd, s, strlen(s));
    n = write(fd, "\n", 1);
    close(fd);
    //delete D from manifest
     struct manifestLL * cPtr = commitHead;
    struct manifestLL * mPtr = manifestHead;
    struct manifestLL * mLag = NULL;

    while(cPtr != NULL){
        mPtr = manifestHead;
        mLag = NULL;
        while(mPtr != NULL){
            if(strcmp(mPtr->path, cPtr->path) == 0 && cPtr->mode == 'D'){
                //remove the entry from manifest ll
                if(mLag == NULL){
                    mPtr = mPtr->next;
                    free(manifestHead->version);
                    free(manifestHead->path);
                    free(manifestHead->oldHash);
                    free(manifestHead);
                    manifestHead = mPtr;
                    mLag = NULL;
                }else{
                    mLag->next = mPtr->next;
                    free(mPtr->version);
                    free(mPtr->path);
                    free(mPtr->oldHash);
                    free(mPtr);
                    mPtr = mLag->next;
                }
            }else{
                mPtr = mPtr->next;
            }
        }
        cPtr = cPtr->next;
    }
    //now that all D is deleted

    cPtr = commitHead;
    //modified case where i just change the live hash and mode
    mPtr = manifestHead;
    mLag = NULL;
    while(cPtr != NULL){
        mPtr = manifestHead;
        while(mPtr != NULL){
            if(strcmp(cPtr->path, mPtr->path) == 0 && cPtr->mode == 'M'){

                mPtr->liveHash = malloc((strlen(cPtr->oldHash) + 1)*sizeof(char));
                strcpy(mPtr->liveHash, cPtr->oldHash);
                mPtr->liveHash[strlen(cPtr->oldHash)] = '\0';

                mPtr->mode = 'Z';

                free(mPtr->version);
                mPtr->version = malloc((strlen(cPtr->version) + 1)*sizeof(char));
                bzero(mPtr->version, strlen(cPtr->version) + 1);
                strcpy(mPtr->version, cPtr->version);

                break;
            }
            mLag = mPtr;
            mPtr = mPtr->next;
        }
        cPtr = cPtr->next;
    }


    cPtr = commitHead;
    mPtr = manifestHead;
    if(mPtr != NULL){
         while(mPtr->next != NULL){
            mPtr = mPtr->next;
        }
    }

    while(cPtr!= NULL){
        if(cPtr->mode == 'A'){
            //append to manifest
            if(manifestHead == NULL){
                manifestHead = malloc(sizeof(struct manifestLL));
                mPtr = manifestHead;
                mPtr->next = NULL;
            }else{
                mPtr->next = malloc(sizeof(struct manifestLL));
                mPtr = mPtr->next;
                mPtr->next = NULL;
            }
            mPtr->mode = 'Z';
            mPtr->version = malloc((strlen(cPtr->version) + 1)*sizeof(char));
            bzero(mPtr->version, strlen(cPtr->version) + 1);
            strcpy(mPtr->version, cPtr->version);

            mPtr->liveHash = malloc((strlen(cPtr->oldHash) + 1)*sizeof(char));
            strcpy(mPtr->liveHash, cPtr->oldHash);
            mPtr->liveHash[strlen(cPtr->oldHash)] = '\0';

            mPtr->path = malloc((strlen(cPtr->path) + 1)*sizeof(char));
            strcpy(mPtr->path, cPtr->path);
            mPtr->path[(strlen(cPtr->path))] = '\0';

        }
        cPtr = cPtr->next;
    }
    int try2 = open(history, O_RDWR | O_CREAT, 00600);
    if(try2 < 0){
        n = write(newsockfd, "fls:", 4);
        return -1;
    }
    totalBytes = 0;
    int bytes = 0;
    while(totalBytes != hSize){
        bytes = write(try2, hContent + totalBytes, hSize - totalBytes);
        if(bytes == -1){
            break;
        }
        totalBytes = totalBytes + bytes;
    }

    close(try2);
    writeToFileX(manifestHead, manifestName, s);
    char * mCont = NULL;
    int sendBack = returnContent(manifestName,&mCont);
    char toSend[200];
    bzero(toSend, 200);
    sprintf(toSend, "%d", sendBack);

    //push:size:cont
    int testing = 6 + sendBack + strlen(toSend);
    char * sending = (char *)malloc(testing*sizeof(char));
    bzero(sending, testing);
    strcpy(sending, "psh:");
    strcat(sending, toSend);
    strcat(sending, ":");
    strcat(sending, mCont);
    n = write(newsockfd, sending, testing);
    if(n < 0){
        return -1;
    }




    return 0;
}
struct manifestLL * getLL(int sizeOfFile, char * fileContent, char ** version){
    int i = 0;
    struct manifestLL * temp = NULL;
    struct manifestLL * head = temp;

    *version = (char *)malloc(20*sizeof(char));
    bzero(*version, 20);

    if(sizeOfFile <= 3){
        //maybe account for null terminator?
        *version[0] = '1';
        return head;
    }
    while(fileContent[i] != '\n'){
        *version[i] = fileContent[i];
        i++;
    }
    i = 2; //skip 1/n
    while(i != sizeOfFile){
        if(head == NULL){
            head = (struct manifestLL *)malloc(sizeof(struct manifestLL));
            head->next = NULL;
            head->liveHash = NULL;
            temp = head;
        }else{
            temp->next = (struct manifestLL *)malloc(sizeof(struct manifestLL));
            temp = temp->next;
            temp->next = NULL;
            temp->liveHash = NULL;
        }

        temp->mode = fileContent[i];
        i = i + 2;
        temp->version = (char *)malloc(20 * sizeof(char));
        bzero(temp->version, 20);
        int j = 0;
        while(fileContent[i] != ' '){
            temp->version[j] = fileContent[i];
            j++;
            i++;
        }
        i++;
        int length = 1;
        while (fileContent[i] != ' '){
            i++;
            length++;
        }
        i = i - (length - 1);
        temp->path = (char *)malloc(length*sizeof(char));
        bzero(temp->path, length);
        j = 0;
        while(fileContent[i] != ' '){
            temp->path[j] = fileContent[i];
            j++;
            i++;
        }
        i++;
        length = 1;
        while (fileContent[i] != '\n'){
            i++;
            length++;
        }
        i = i - (length - 1);
        temp->oldHash = (char *)malloc(length*sizeof(char));
        bzero(temp->oldHash, length);
        j = 0;
        while(fileContent[i] != '\n'){
            temp->oldHash[j] = fileContent[i];
            j++;
            i++;
        }
        i++;
    }
    return head;
}
int writeToFileX(struct manifestLL * commitHeader, char * fileName, char * version){
    int fd = open(fileName, O_RDWR | O_CREAT , 00600);
    if(fd < 0){
        printf("issue during commit\n");
        return -1;
    }
    int bytes = 0;
    write(fd, version, strlen(version));
    write(fd, "\n", 1);
    struct manifestLL * ptr = commitHeader;
    //make this blocking when tomorrow comes!
    while(ptr!=NULL){
        char oops[2];
        //mode
        oops[0] = ptr->mode;
        oops[1] = '\0';
        bytes = write(fd, oops, 1);
        if(bytes < 0){
            printf("issues\n");
            return -1;
        }
        write(fd, " ", 1);
        //file version
        bytes = write(fd, ptr->version, strlen(ptr->version));
        if(bytes < 0){
            printf("issues\n");
            return -1;
        }
        write(fd, " ", 1);
        bytes = write(fd, ptr->path, strlen(ptr->path));
        if(bytes < 0){
            printf("issues\n");
            return -1;
        }
        write(fd, " ", 1);
        if(ptr->liveHash != NULL){
             bytes = write(fd, ptr->liveHash, strlen(ptr->liveHash));
        }else{
             bytes = write(fd, ptr->oldHash, strlen(ptr->oldHash));
        }

        if(bytes < 0){
            printf("issues\n");
            return -1;
        }
        write(fd, "\n", 1);
        //printf("%s %s\n", oops, ptr->path);
        ptr = ptr->next;
    }
    close(fd);
    return 0;
}
int tarProject3(char * projectName, struct manifestLL * head){
    struct manifestLL * ptr = head;
    int len = 0;
    while(ptr != NULL){
        if(ptr->mode != 'D'){
            len = len + strlen(ptr->path) + 2;
        }
        ptr=ptr->next;
    }
    ptr = head;

    len = len + strlen(projectName) +13;

    int totalLength  = (2*strlen(projectName))+19 + len;
    char * string = (char *)malloc(totalLength*sizeof(char));
    bzero(string, totalLength);
    strcpy(string, "tar czvf ");
    strcat(string, projectName);
    strcat(string,".tar.gz ");
    while(ptr != NULL){
        if(ptr->mode != 'D'){
             strcat(string, ptr->path);
            strcat(string, " ");
        }
        ptr=ptr->next;
    }
    strcat(string, projectName);
    strcat(string, "/.Manifest");
    //strcat(string, projectName);
    system(string);
    return 0;
}
struct manifestLL * getCommitLL(int sizeOfFile, char * fileContent){
    int i = 0;
    struct manifestLL * temp = NULL;
    struct manifestLL * head = temp;

    while(i != sizeOfFile){
        if(head == NULL){
            head = (struct manifestLL *)malloc(sizeof(struct manifestLL));
            head->next = NULL;
            head->liveHash = NULL;
            temp = head;
        }else{
            temp->next = (struct manifestLL *)malloc(sizeof(struct manifestLL));
            temp = temp->next;
            temp->next = NULL;
            temp->liveHash = NULL;
        }

        temp->mode = fileContent[i];
        i = i + 2;
        temp->version = (char *)malloc(20 * sizeof(char));
        bzero(temp->version, 20);
        int j = 0;
        while(fileContent[i] != ' '){
            temp->version[j] = fileContent[i];
            j++;
            i++;
        }
        i++;
        int length = 1;
        while (fileContent[i] != ' '){
            i++;
            length++;
        }
        i = i - (length - 1);
        temp->path = (char *)malloc(length*sizeof(char));
        bzero(temp->path, length);
        j = 0;
        while(fileContent[i] != ' '){
            temp->path[j] = fileContent[i];
            j++;
            i++;
        }
        i++;
        length = 1;
        while (fileContent[i] != '\n'){
            i++;
            length++;
        }
        i = i - (length - 1);
        temp->oldHash = (char *)malloc(length*sizeof(char));
        bzero(temp->oldHash, length);
        j = 0;
        while(fileContent[i] != '\n'){
            temp->oldHash[j] = fileContent[i];
            j++;
            i++;
        }
        i++;
    }
    return head;
}

int createDir(char * pathName){
    //if doesnt exist create and return 1 for success
    DIR * dir = opendir(pathName);
    if(dir == NULL){
        int check;
        check= mkdir(pathName, 0700);
        if(check < 0){
            //printf("Root directory can't be made! \n");
            return -1;
        }
    }
    closedir(dir);
    return 1;
}

int destruction(int fileDes){
    //lock repository
    int n;
    int newsockfd = fileDes;
    char projectNameSize[500];
    bzero(projectNameSize, 500);
    int totalBytes = 0;
    int i = 0;
    while(1){
        n = read(newsockfd,(projectNameSize + totalBytes),1);
        totalBytes = totalBytes + n;

        if (n < 0){
            error("ERROR reading from socket");
        }

        if(projectNameSize[i] == ':'){
            projectNameSize[i] = '\0';
            break;
        }
        i++;
    }
    int numToRead = atoi(projectNameSize) + 3;
    char * projectName = (char *)malloc(numToRead*sizeof(char));
    bzero(projectName, numToRead);
    strcat(projectName, "./");

    n = read(newsockfd, projectName+2, numToRead - 3);
    if(n < 0){
        printf("error reading from socket\n");
        return -1;
    }

    DIR * dir = opendir(projectName);
    if(dir == NULL){
        n = write(newsockfd, "fls:", 4);
        if (n < 0){
            printf("Failed to write back to client\n");
            return -1;
        }
        return -1;
    }
    closedir(dir);

    //before remove check manifest for version

    char currentManifest[500];
    bzero(currentManifest, 500);
    char deleteFolder[500];
    bzero(deleteFolder, 500);
    strcpy(deleteFolder, projectName);
    strcpy(currentManifest, projectName);
    strcat(currentManifest, "/.Manifest");

    int manifestVersion = 0; // doesnt exist
    char manifestV[200];
    bzero(manifestV, 200);
    i = 0;
    int fd = open(currentManifest, O_RDONLY);
    if(fd != -1){
        while(1){
            manifestVersion = read(fd, manifestV + i, 1);
            if(manifestV[i] == '\n'){
                manifestV[i] = '\0';
                break;
            }
            i++;
        }
    }
    close(fd);
    manifestVersion = atoi(manifestV);
    //manifestVersion--;
    remove_directory(projectName);
    while(manifestVersion > 0){
        //remove all other previous versions
        bzero(deleteFolder, 500);
        strcpy(deleteFolder, projectName);
        strcat(deleteFolder, manifestV);
        remove_directory(deleteFolder);
        manifestVersion--;
        bzero(manifestV, 200);
        sprintf(manifestV, "%d", manifestVersion);
    }
    n = write(newsockfd, "dst:", 4);
    if(n < 0){
        printf("Failed to write back to client\n");
        return -1;
    }

    return 0;

    //expire PENDING COMMITS ??

}
int commition(int fileDes){
    int n;
    int newsockfd = fileDes;

    int numToRead = 0;
    int i = 0;

    char projectNameSize[500];
    bzero(projectNameSize, 500);
    int totalBytes = 0;
    while(1){
        n = read(newsockfd,(projectNameSize + totalBytes),1);
        totalBytes = totalBytes + n;

        if (n < 0){
            error("ERROR reading from socket");
        }

        if(projectNameSize[i] == ':'){
            projectNameSize[i] = '\0';
            break;
        }
        i++;
    }
    numToRead = atoi(projectNameSize) + 3;
    char * projectName = (char *)malloc(numToRead*sizeof(char));
    bzero(projectName, numToRead);
    strcpy(projectName, "./");
    n = read(newsockfd,projectName + 2, numToRead - 3);
    if(n < 0){
        printf("problems reading from socket\n");
        return -1;
    }

    DIR * dir = opendir(projectName);
    if(projectName== NULL){
        n = write(newsockfd, "fls:", 4);
        return -1;
    }
    closedir(dir);
    char * manifest = (char *)malloc((numToRead + 10)*sizeof(char));
    bzero(manifest, (numToRead+10));
    strcpy(manifest, projectName);
    strcat(manifest, "/.Manifest");

    char * fileContent = NULL;
    int sizeToSend = returnContent(manifest, &fileContent);
    if(sizeToSend == -1){
        printf("Something is wrong with the .Manifest file\n");
        return -1;
    }

    char * sizeOfLength = (char *)malloc(100*sizeof(char));
    bzero(sizeOfLength, 100);
    sprintf(sizeOfLength, "%d", sizeToSend);
    int sizeLen = strlen(sizeOfLength);

    int sendLen = 5 + sizeLen + sizeToSend + 1;
    char * sendString = (char *)malloc(sendLen*sizeof(char));
    strcpy(sendString, "cmt:");
    strcat(sendString, sizeOfLength);
    strcat(sendString, ":");
    strcat(sendString, fileContent);
    sendString[sendLen - 1] = '\0';
    free(sizeOfLength);
    free(manifest);
    free(fileContent);

    n = write(newsockfd, sendString, strlen(sendString));
    if(n < 0){
        printf("There was an issue sending .Manifest to client\n");
        return -1;
    }
    //expecting either failure or returning commit file
    char buffie[5];
    n = read(newsockfd, buffie, 4);
    buffie[4] = '\0';
    if(strcmp(buffie, "fls:") == 0){
        return -1;
    }
    //else it is cmt:
    i = 0;
    char hashCode[500];
    bzero(hashCode, 500);
    while(1){
        n = read(newsockfd, hashCode + i, 1);
        if(n < 0){
            printf("error reading from socket\n");
            return -1;
        }
        if(hashCode[i] == ':'){
            hashCode[i] = '\0';
            break;
        }
        i++;
    }
    char * commitProject = (char *)malloc((numToRead+4+strlen(hashCode))*sizeof(char));
    bzero(commitProject, numToRead + 4 + strlen(hashCode));
    strcpy(commitProject, projectName);
    strcat(commitProject, "/.c");

    if(strcmp(buffie, "cmO:") == 0){
        //user has not had past commits make new
        //then cmN:newhash:size:content
        DIR * dir2 = opendir(commitProject);
        if(dir2 == NULL){
            n = mkdir(commitProject, 0700);
            if(n == -1){
                n = write(newsockfd, "fls:", 4);
                if (n == -1){
                    return -1;
                }
            }
        }
        closedir(dir2);
        strcat(commitProject, "/");
        strcat(commitProject, hashCode);
        int fd = open(commitProject, O_WRONLY | O_CREAT, 00600);
        if(fd == -1){
            n = write(newsockfd, "fls:", 4);
            if (n == -1){
                return -1;
            }
        }
        char sizeFile[100];
        bzero(sizeFile, 100);
        i = 0;
        while(1){
            n = read(newsockfd, sizeFile + i, 1);
            if(sizeFile[i] == ':'){
                sizeFile[i] = '\0';
                break;
            }
            i++;
        }
        int fileSize = atoi(sizeFile);
        char * writeCommit = (char *)malloc((fileSize + 1)*sizeof(char));
        bzero(writeCommit, fileSize + 1);
        n = read(newsockfd, writeCommit, fileSize);
        n = write(fd, writeCommit, fileSize);
        //do more erorr checking and blocking writes for files here and in client
        close(fd);
        n = write(newsockfd, "cmt:", 4); //success
        return 0;
    }else{
        //if buffie is "cmO:" then look for previos hashcode stored
        //cmO:newhash:old:size:content, right now look for old
        strcat(commitProject, "/");
        char hashCode2[500];
        bzero(hashCode2, 500);
        i = 0;
        while(1){
            n = read(newsockfd, hashCode2 + i, 1);
            if(n < 0){
                printf("error reading from socket\n");
                return -1;
            }
            if(hashCode2[i] == ':'){
                hashCode2[i] = '\0';
                break;
            }
            i++;
        }

        char * newCommit = (char *)malloc((numToRead+4+strlen(hashCode2))*sizeof(char));
        bzero(newCommit, (numToRead+4+strlen(hashCode2)));
        strcpy(newCommit, commitProject);
        strcat(commitProject, hashCode);
        int fd = open(commitProject, O_RDWR);
        if(fd < 0){
            n = write(newsockfd, "fls:", 4);
            if(n == -1){
                printf("error on write to socket\n");
                return -1;
            }
            return -1;
        }
        close(fd);

        strcat(newCommit, hashCode2);
        unlink(commitProject);
        fd = open(newCommit, O_WRONLY | O_CREAT, 00600);
        if(fd == -1){
            n = write(newsockfd, "fls:", 4);
            if (n == -1){
                return -1;
            }
        }
        char sizeFile[100];
        bzero(sizeFile, 100);
        i = 0;
        while(1){
            n = read(newsockfd, sizeFile + i, 1);
            if(sizeFile[i] == ':'){
                sizeFile[i] = '\0';
                break;
            }
            i++;
        }
        int fileSize = atoi(sizeFile);
        char * writeCommit = (char *)malloc((fileSize + 1)*sizeof(char));
        bzero(writeCommit, fileSize + 1);
        n = read(newsockfd, writeCommit, fileSize);
        n = write(fd, writeCommit, fileSize);
        close(fd);
        n = write(newsockfd, "cmt:", 4);
    }
    //if you cant find this then commit
    return 0;
}
int tarProject2(char * projectName, struct manifestLL * head){
    struct manifestLL * ptr = head;
    int len = 0;
    while(ptr != NULL){
        if(ptr->mode != 'D'){
            len = len + strlen(ptr->path) + 2;
        }
        ptr=ptr->next;
    }
    ptr = head;

    int totalLength  = (2*strlen(projectName))+19 + len;
    char * string = (char *)malloc(totalLength*sizeof(char));
    bzero(string, totalLength);
    strcpy(string, "tar czvf ");
    strcat(string, projectName);
    strcat(string,".tar.gz ");
    while(ptr != NULL){
        if(ptr->mode != 'D'){
             strcat(string, ptr->path);
            strcat(string, " ");
        }
        ptr=ptr->next;
    }

    //strcat(string, projectName);
    system(string);
    return 0;
}

int extractProject2(char * projectName){
    int totalLength  = strlen(projectName)+16;

    char * string = (char *)malloc(totalLength*sizeof(char));
    bzero(string, totalLength);
    strcpy(string, "tar xvf ");
    strcat(string, projectName);
  //  printf("%s\n", string);
    system(string);
    return 0;
}
int remove_directory(char *path)
{
   DIR *d = opendir(path);
   size_t path_len = strlen(path);
   int r = -1;

   if (d)
   {
      struct dirent *p;

      r = 0;

      while (!r && (p=readdir(d)))
      {
          int r2 = -1;
          char *buf;
          size_t len;


          if (strcmp(p->d_name, ".") == 0 || strcmp(p->d_name, "..") == 0)
          {
             continue;
          }

          len = path_len + strlen(p->d_name) + 2;
          buf = malloc(len);

          if (buf)
          {
             struct stat statbuf;

             snprintf(buf, len, "%s/%s", path, p->d_name);

             if (!stat(buf, &statbuf))
             {
                if (S_ISDIR(statbuf.st_mode))
                {
                   r2 = remove_directory(buf);
                }
                else
                {
                   r2 = unlink(buf);
                }
             }

             free(buf);
          }

          r = r2;
      }

      closedir(d);
   }

   if (!r)
   {
      r = rmdir(path);
   }

   return r;
}
int tarProject(char * projectName){
    int totalLength  = (2*strlen(projectName))+19;
    char * string = (char *)malloc(totalLength*sizeof(char));
    strcpy(string, "tar czvf ");
    strcat(string, projectName);
    strcat(string,".tar.gz ./");
    strcat(string, projectName);
    system(string);
    return 0;
}
void error(char *msg)
{
    perror(msg);
    exit(1);
}
