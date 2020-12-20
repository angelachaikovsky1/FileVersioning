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
#include <netdb.h>
#include <openssl/sha.h>
//client
void error (char * msg);
int configureClient(char* ip, char* port);
int connectToServer(char * command, char * information, int size);
int create(char * name);
int openConfigure(char ** port, char ** ip);
int addFile(char * projectName, char * fileName);
int appendToEndAdd(char * fileName, char * entryToAppend, char * path);
int appendToEnd(char * fileName, char * entryToAppend);
int returnContent(char * fileName, char ** charContent);
int removeFile(char * projectName, char * fileName);
int currentVersion(char * projectName);
char * compHash(char * fileContent);
int tarProject(char * projectName);
int extractProject(char * projectName);
int history(char * projectName);
int checkout(char * projectName);
int rollback(char * projectName, char * version);
struct manifestLL * getLL(int sizeOfFile, char * fileContent, char ** version);
int remove_directory(char *path);
int writeToFile(struct manifestLL * commitHeader, char * fileName);
int destroy(char * name);
int push(char * name);
int upgrade(char * name);
int commit(char * name);
int update(char * name);
int extractProject2(char * projectName);
void setLiveHash(struct manifestLL * head);
int modified(struct manifestLL ** commitHead, struct manifestLL * clientHead, struct manifestLL * serverHead);
int added(struct manifestLL ** commitHead, struct manifestLL * clientHead, struct manifestLL * serverHead);
int deleted(struct manifestLL ** commitHead, struct manifestLL * clientHead, struct manifestLL * serverHead);
int partialOrFailure(struct manifestLL ** conflictHead, struct manifestLL ** updateHead, struct manifestLL * clientHead, struct manifestLL * serverHead);
struct manifestLL * getCommitLL(int sizeOfFile, char * fileContent);
int tarProject2(char * projectName, struct manifestLL * head);
int isFileEmpty(char * fileName);
struct manifestLL{
    char mode;
    char * version;
    char * path;
    char * oldHash;
    char * liveHash;
    struct manifestLL * next;
};

int main(int argc, char *argv[]){
    if(argc<3){
        printf("Not a sufficient amount of arguments!\n");
        return 0;
    }

    if(strcmp(argv[1], "configure")==0){
         if(argc!=4){
            printf("Not a sufficient amount of arguments!\n");
          return 0;
        }else{
            int check =0;
            check= configureClient(argv[2],argv[3]);
            if(check==-1){
                printf("There was an error in creating the .configure file.\n");
                return 0;
            }
        }
    }else if(strcmp(argv[1], "checkout")==0){
         if(argc!=3){
            printf("Not an expected amount of arguments!\n");
          return 0;
        }else{
            int check =0;
            check= checkout(argv[2]);
            if(check==-1){
               // printf("There was an error in adding the file to the Manifest.\n");
                return 0;
            }
        }

    }else if(strcmp(argv[1], "update")==0){
         if(argc!= 3){
            printf("You did not write an expected number of arguments\n");
            return 0;
        }else{
            int check = update(argv[2]);
            if(check == -1){
                //printf("there was an error updating the project\n");
                return 0;
            }
            }
    }else if(strcmp(argv[1], "upgrade")==0){
         if(argc!= 3){
            printf("You did not write an expected number of arguments\n");
            return 0;
        }else{
            int check = upgrade(argv[2]);
            if(check == -1){
               // printf("there was an error updating the project\n");
                return 0;
            }
        }
    }else if(strcmp(argv[1], "commit")==0){
         if(argc!= 3){
            printf("You did not write an expected number of arguments\n");
            return 0;
        }else{
            int check = commit(argv[2]);
            if(check == -1){
                //printf("there was an error committing the project\n");
                return 0;
            }
        }
    }else if(strcmp(argv[1], "push")==0){
            if(argc!= 3){
            printf("You did not write an expected number of arguments\n");
            return 0;
        }else{
            int check = push(argv[2]);
            if(check == -1){
               // printf("there was an error pushing the project\n");
                return 0;
            }
        }
    }else if(strcmp(argv[1], "create")==0){
         if(argc!=3){
            printf("Not an expected amount of arguments!\n");
          return 0;
        }else{
            int check =0;
            check= create(argv[2]);
            if(check==-1){
               // printf("There was an error in creating the project.\n");
                return 0;
            }
        }
    }else if(strcmp(argv[1], "destroy")==0){
        if(argc!= 3){
            printf("Not an expected amount of arguments!\n");
            return 0;
        }else{
            int check = 0;
            check = destroy(argv[2]);
            if(check == -1){
                //printf("there was an error destroying this project");
                return 0;
            }
        }
    }else if(strcmp(argv[1], "add")==0){
         if(argc!=4){
            printf("Not an expected amount of arguments!\n");
          return 0;
        }else{
            int check =0;
            check= addFile(argv[2], argv[3]);
            if(check==-1){
               // printf("There was an error in adding the file to the Manifest.\n");
                return 0;
            }
        }
    }else if(strcmp(argv[1], "remove")==0){
          if(argc!=4){
            printf("Not an expected amount of arguments!\n");
          return 0;
        }else{
            int check =0;
            check= removeFile(argv[2], argv[3]);
            if(check==-1){
               // printf("There was an error in adding the file to the Manifest.\n");
                return 0;
            }
        }
    }else if(strcmp(argv[1], "currentversion")==0){
             if(argc!=3){
                printf("Not an expected amount of arguments!\n");
                return 0;
            }else{
                int check =0;
                check= currentVersion(argv[2]);
                if(check==-1){
               // printf("There was an error in adding the file to the Manifest.\n");
                return 0;
            }
        }
    }else if(strcmp(argv[1], "history")==0){
          if(argc!=3){
            printf("Not an expected amount of arguments!\n");
          return 0;
        }else{
            int check =0;
            check= history(argv[2]);
            if(check==-1){
                printf("There was an error in getting the history of the project.\n");
                return 0;
            }
        }

    }else if(strcmp(argv[1], "rollback")==0){
        if(argc!=4){
            printf("Not a sufficient amount of arguments!\n");
          return 0;
        }else{
            int check =0;
            check= rollback(argv[2],argv[3]);
            if(check==-1){
                printf("There was an error in rolling back.\n");
                return 0;
            }
        }
    }else{
        printf("Unknown action. Please enter a valid command\n");
        return 0;
    }

    /*int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        error("ERROR opening socket");
    }
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr_list[0],(char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){
        error("ERROR connecting");
    }
    printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0){
         error("ERROR writing to socket");
      }
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0){
         error("ERROR reading from socket");
    }
    printf("%s\n",buffer);*/
    return 0;
}
//char * itoa(int value, char * base, int dec);

int create(char * name){
    int nameLength = strlen(name);
    nameLength++; //for null terminator
    char * sizeOfLength = (char *)malloc(nameLength*sizeof(char));
    sizeOfLength++; //for null terminator
    sprintf(sizeOfLength, "%d", nameLength);
    //itoa(nameLength, sizeOfLength, 10);
    sizeOfLength[nameLength] = '\0';
    int newSize = strlen(sizeOfLength);
    int totalLength = 5+nameLength+newSize;
    char * info = (char *) malloc(totalLength * sizeof(char));

    strcpy(info, "cte:");
    strcat(info, sizeOfLength);
    strcat(info,":");
    strcat(info, name);
    int status = connectToServer("create", info, totalLength);
    if(status == -1){
        //free(sizeOfLength);
        //free(info);
        return -1;
    }
    //free(sizeOfLength);
    //free(info);
    return 0;
}
int isFileEmpty(char * fileName){
    int fd = open(fileName, O_RDONLY);
    if(fd == -1){
        return 1;
    }
    char holder[1];
    int bytes = read(fd, holder, 1);
    if(bytes != 0){
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}
int push(char * name){
    char commitName[500];
    bzero(commitName, 500);
    strcpy(commitName, "./");
    strcat(commitName, name);
    strcat(commitName, "/.Commit");
    int fd = open(commitName, O_RDONLY);
    if(fd < 0){
        printf("must commit first\n");
        return -1;
    }
    close(fd);

    int nameLength = strlen(name);
    nameLength++; //for null terminator
    char * sizeOfLength = (char *)malloc(nameLength*sizeof(char));
    bzero(sizeOfLength, nameLength);
    sizeOfLength++; //for null terminator
    sprintf(sizeOfLength, "%d", nameLength);
    //itoa(nameLength, sizeOfLength, 10);
    sizeOfLength[nameLength - 1] = '\0';
    int newSize = strlen(sizeOfLength);
    int totalLength = 5+nameLength+newSize;
    char * info = (char *) malloc(totalLength * sizeof(char));

    strcpy(info, "psh:");
    strcat(info, sizeOfLength);
    strcat(info,":");
    strcat(info, name);
    int status = connectToServer("push", info, totalLength);
    if(status == -1){
        return -1;
    }
 //   free(sizeOfLength);
  //  free(info);

    return 0;
}

int upgrade(char * name){
    int nameLength = strlen(name);
    nameLength++; //for null terminator
    char * sizeOfLength = (char *)malloc(nameLength*sizeof(char));
    bzero(sizeOfLength, nameLength);
    sizeOfLength++; //for null terminator
    sprintf(sizeOfLength, "%d", nameLength);
    //itoa(nameLength, sizeOfLength, 10);
    sizeOfLength[nameLength - 1] = '\0';
    int newSize = strlen(sizeOfLength);
    int totalLength = 5+nameLength+newSize;
    char * info = (char *) malloc(totalLength * sizeof(char));

    strcpy(info, "upg:");
    strcat(info, sizeOfLength);
    strcat(info,":");
    strcat(info, name);
    int status = connectToServer("upgrade", info, totalLength);
    if(status == -1){
        return -1;
    }
    //free(sizeOfLength);
    //free(info);

    return 0;
}

int update(char * name){
    int nameLength = strlen(name);
    nameLength++; //for null terminator
    char * sizeOfLength = (char *)malloc(nameLength*sizeof(char));
    bzero(sizeOfLength, nameLength);
    sizeOfLength++; //for null terminator
    sprintf(sizeOfLength, "%d", nameLength);
    //itoa(nameLength, sizeOfLength, 10);
    sizeOfLength[nameLength - 1] = '\0';
    int newSize = strlen(sizeOfLength);
    int totalLength = 5+nameLength+newSize;
    char * info = (char *) malloc(totalLength * sizeof(char));

    strcpy(info, "upd:");
    strcat(info, sizeOfLength);
    strcat(info,":");
    strcat(info, name);
    int status = connectToServer("update", info, totalLength);
    if(status == -1){
        return -1;
    }
    //free(sizeOfLength);
    //free(info);

    return 0;
}

int destroy(char * name){
    //name is project name
    int nameLength = strlen(name);
    nameLength++; //for null terminator
    char * sizeOfLength = (char *)malloc(nameLength*sizeof(char));
    bzero(sizeOfLength, nameLength);
    sizeOfLength++; //for null terminator
    sprintf(sizeOfLength, "%d", nameLength);
    //itoa(nameLength, sizeOfLength, 10);
    sizeOfLength[nameLength - 1] = '\0';
    int newSize = strlen(sizeOfLength);
    int totalLength = 5+nameLength+newSize;
    char * info = (char *) malloc(totalLength * sizeof(char));

    strcpy(info, "dst:");
    strcat(info, sizeOfLength);
    strcat(info,":");
    strcat(info, name);
    int status = connectToServer("destroy", info, totalLength);
    if(status == -1){
        return -1;
    }
    //free(sizeOfLength);
    //free(info);
    return 0;
}

int commit(char * name){
    // isFileEmpty on .Update should be
    //name is essentially project name
    char * conflictFile;
    char * updateFile;

    int cLength = strlen(name) + 10 + 3; //.conflict
    int uLength = strlen(name) + 8 + 3; //.update

    conflictFile =(char *)malloc(cLength * sizeof(char));
    updateFile =(char *)malloc(uLength * sizeof(char));
    strcpy(conflictFile, "./");
    strcpy(updateFile, "./");
    strcat(conflictFile, name);
    strcat(updateFile, name);
    strcat(conflictFile, "/");
    strcat(updateFile, "/");
    strcat(conflictFile, ".Conflict");
    strcat(updateFile, ".Update");

    conflictFile[cLength - 1] = '\0';
    updateFile[uLength - 1] = '\0';

    //check if conflict and update exist
    // 1 for file does not exist
    // 0 for file exists but is empty
    // -1 for file exists and is nOT empty
    //check only if update is empty
    if(isFileEmpty(conflictFile) != 1){
        printf("this project has a .Conflict file, cannot commit\n");
        return -1;
    }
    if(isFileEmpty(updateFile) == -1){
        printf("this project has an.Update file  with pending updates, cannot commit\n");
        return -1;
    }
    int nameLength = strlen(name);
    nameLength++; //for null terminator
    char * sizeOfLength = (char *)malloc(nameLength*sizeof(char));
    bzero(sizeOfLength, nameLength);
    sizeOfLength++; //for null terminator
    sprintf(sizeOfLength, "%d", nameLength);
    //itoa(nameLength, sizeOfLength, 10);
    sizeOfLength[nameLength -1] = '\0';
    int newSize = strlen(sizeOfLength);
    int totalLength = 5+nameLength+newSize;
    char * info = (char *) malloc(totalLength * sizeof(char));

    strcpy(info, "cmt:");
    strcat(info, sizeOfLength);
    strcat(info,":");
    strcat(info, name);
    int status = connectToServer("commit", info, totalLength);
    if(status == -1){
        return -1;
    }
    return 0;
}
int appendToEndAdd(char * manifest, char * entryToAppend, char * path){
    char * content = NULL;
    int manifestSize = returnContent(manifest, &content);
    if(manifestSize!=0){
    struct manifestLL * head=NULL;
    char* version =NULL;
   // printf("%s\n", manifestContent);
    head =  getLL(manifestSize,content, &version);
   // printf("%s\n", version);
    struct manifestLL * ptr =head;
    int count =0;
    while(ptr!=NULL){
        //printf("%c\n", ptr->mode);
        if(strcmp(ptr->path, path)==0){
            if(ptr->mode=='R'){
                ptr->mode='A';
                count++;
            }else if(ptr->mode=='A'){
                printf("The file has already been added to the .Manifest\n");
                return -1;
            }

        }
        // printf("%c\n", ptr->mode);
     ptr = ptr->next;
     }
     if(count!=0){
    //return 0;
    remove(manifest);
    int fd = open(manifest, O_CREAT | O_WRONLY , 00600);
    struct manifestLL * temp = head;
    int firstLineBytes = 0;
    firstLineBytes = write(fd, version, strlen(version));
    firstLineBytes = firstLineBytes + write(fd, "\n", 1);
    while (temp!=NULL){

       int entryLength = 1+strlen(temp->version)+strlen(temp->path)+strlen(temp->oldHash)+4;
       char * entry = (char *)malloc(entryLength* sizeof(char));
       bzero(entry, entryLength);
        entry[0]=temp->mode;
        strcat(entry, " ");
        strcat(entry, temp->version);
        strcat(entry, " ");
        strcat(entry, temp->path);
        strcat(entry, " ");
        strcat(entry, temp->oldHash);
        strcat(entry, "\n");

        int length = strlen(entry);
        int bytes=0;
        int totalbytes=0;
        while(totalbytes!=length){
            bytes = write(fd, entry+totalbytes, length-totalbytes);
        if(bytes<0){
            printf("There was an error writing to the .Manifest file\n");
            close(fd);
            return -1;

        }
        totalbytes=totalbytes+bytes;
    }
        temp=temp->next;
    }
    free(version);
    close(fd);
    return 0;
    }
}
   /* int contentLength = strlen(content);
    int pathLength = strlen(path);
    int i=0;
    for(i=0; i<contentLength; i++){
        int count =0;
        if(content[i]==path[0]){
            int j=0;
            for(j=0; j<pathLength; j++){
                if(content[i+j]==path[j]){
                    count++;
                }else{
                    break;
                }

            }
        }
        if(count==pathLength){
            printf("The file already exists in the .Manifest so it could not be added\n");
            return -1;
        }
    }*/
    int fd2 = open(manifest, O_RDWR);

    int loc = lseek(fd2, 0, SEEK_END);
   // printf("%d\n", loc);
    if(loc<0){
             printf("There was an error writing to the .Manifest file\n");
            close(fd2);
            return -1;
        }
    int length = strlen(entryToAppend);
    int bytes=0;
    int totalbytes=0;
    while(totalbytes!=length){
        bytes = write(fd2, entryToAppend+totalbytes, length-totalbytes);
        if(bytes<0){
            printf("There was an error writing to the .Manifest file\n");
            close(fd2);
            return -1;

        }
        totalbytes=totalbytes+bytes;
    }
    free(content);

    return 0;
}
int addFile(char * projectName, char * fileName){
    int pathLength = strlen(projectName)+strlen(fileName)+5;
    char * path = (char *)malloc(pathLength*sizeof(char));
    strcpy(path, ".");
    strcat(path, "/");
    strcat(path, projectName);
     DIR * dir = opendir(path);
    if(dir == NULL){
        printf("Given project does not exist\n");
        return -1;
    }
    closedir(dir);
    //path[strlen(projectName)+2]='\0';
    if(fileName[0]!='/'){
    strcat(path, "/");
    }
    strcat(path, fileName);
    //path[pathLength]='\0';
    //printf("%s\n", path);
    int fd = open(path, O_RDONLY);

    if(fd<0){
        printf("The file does not exist within the project. Please enter a valid file!\n");
        return -1;
        close(fd);
    }
    close(fd);

    char version[]  = "1";
    char * content = NULL;
    returnContent(path, &content);
    char * hashcode =(char*)malloc(256*sizeof(char));
    strcpy(hashcode,(char *)compHash(content));

   // printf("%02x\n", hashcode);
    int totalLength = strlen(path)+strlen(version)+strlen(hashcode)+5;
    char * info  = (char *)malloc(totalLength*sizeof(char));
    strcpy(info, "A");
    strcat(info, " ");
    strcat(info, version);
    strcat(info, " ");
    strcat(info, path);
    strcat(info, " ");
    strcat(info, hashcode);
    strcat(info, "\n");
    //printf("%s\n", info);
    int manLength = 12+strlen(projectName);
    char * manPath = (char *)malloc(manLength *sizeof(char));
    strcpy(manPath, "./");
    strcat(manPath, projectName);
    strcat(manPath, "/");
    strcat(manPath, ".Manifest");
    appendToEndAdd(manPath, info, path);
   /* if(val<0){
        printf("There was an issue with the .Manifest file\n");
        }
    */
    free(path);
    free(hashcode);
    free(info);
    return 0;

}
int removeFile(char * projectName, char * fileName){
     int pathLength = strlen(projectName)+strlen(fileName)+5;
    char * path = (char *)malloc(pathLength*sizeof(char));
    strcpy(path, ".");
    strcat(path, "/");
    strcat(path, projectName);
     DIR * dir = opendir(path);
    if(dir == NULL){
        printf("Given project does not exist\n");
        return -1;
    }
    closedir(dir);

    if(fileName[0]!='/'){
        strcat(path, "/");
    }
    strcat(path, fileName);

    char * manifestContent = NULL;
    int manLength = 12+strlen(projectName);
    char * manPath = (char *)malloc(manLength *sizeof(char));
    strcpy(manPath, "./");
    strcat(manPath, projectName);
    strcat(manPath, "/");
    strcat(manPath, ".Manifest");

    int fp = open(manPath, O_RDWR);
     if(fp<0){
        printf("There was no .Manifest file found\n");
        return -1;
    }
    close(fp);

    int fileSize = returnContent(manPath, &manifestContent);

    struct manifestLL * head=NULL;
    char* version =NULL;
   // printf("%s\n", manifestContent);
    head =  getLL(fileSize,manifestContent, &version);
   // printf("%s\n", version);
    struct manifestLL * ptr =head;
    int count =0;
    while(ptr!=NULL){
        //printf("%c\n", ptr->mode);
        if(strcmp(ptr->path, path)==0){
            if(ptr->mode!='R'){
                ptr->mode='R';
                count++;
            }else{
                printf("The file has already been removed from the .Manifest\n");
                return -1;
            }

        }
        // printf("%c\n", ptr->mode);
     ptr = ptr->next;
     }
     if(count==0){
        printf("The file was not previously added to the .Manifest file so it could not be removed. \n");
        return -1;
     }
    //return 0;
    remove(manPath);
    int fd = open(manPath, O_CREAT | O_WRONLY , 00600);
    struct manifestLL * temp = head;
    int firstLineBytes = 0;
    firstLineBytes = write(fd, version, strlen(version));
    firstLineBytes = firstLineBytes + write(fd, "\n", 1);
    while (temp!=NULL){

       int entryLength = 1+strlen(temp->version)+strlen(temp->path)+strlen(temp->oldHash)+4;
       char * entry = (char *)malloc(entryLength* sizeof(char));
        entry[0]=temp->mode;
        strcat(entry, " ");
        strcat(entry, temp->version);
        strcat(entry, " ");
        strcat(entry, temp->path);
        strcat(entry, " ");
        strcat(entry, temp->oldHash);
        strcat(entry, "\n");

        int length = strlen(entry);
        int bytes=0;
        int totalbytes=0;
        while(totalbytes!=entryLength){
            bytes = write(fd, entry+totalbytes, length-totalbytes);
        if(bytes<0){
            printf("There was an error writing to the .Manifest file\n");
            close(fd);
            return -1;

        }
        totalbytes=totalbytes+bytes;
    }
        temp=temp->next;
    }

return 0;
}
int currentVersion(char * projectName){
     int pathLength = strlen(projectName)+2;
    char * path = (char *)malloc(pathLength*sizeof(char));
    strcpy(path, ".");
    strcat(path, "/");
    strcat(path, projectName);

     int nameLength = strlen(projectName);
    nameLength++;
    char * sizeOfLength = (char *)malloc(nameLength*sizeof(char));
    sizeOfLength++;
    sprintf(sizeOfLength, "%d", nameLength);
    sizeOfLength[nameLength] = '\0';
    int newSize = strlen(sizeOfLength);
    int totalLength = 5+nameLength+newSize;
    char * info = (char *) malloc(totalLength * sizeof(char));

    strcpy(info, "cvn:");
    strcat(info, sizeOfLength);
    strcat(info,":");
    strcat(info, projectName);


    int status = connectToServer("currentversion", info, totalLength);
    if(status == -1){
        return -1;
    }
    return 0;
}
int history(char * projectName){
    int nameLength = strlen(projectName);
    nameLength++;
    char * sizeOfLength = (char *)malloc(nameLength*sizeof(char));
    sizeOfLength++;
    sprintf(sizeOfLength, "%d", nameLength);
    sizeOfLength[nameLength] = '\0';
    int newSize = strlen(sizeOfLength);
    int totalLength = 5+nameLength+newSize;
    char * info = (char *) malloc(totalLength * sizeof(char));

    strcpy(info, "hst:");
    strcat(info, sizeOfLength);
    strcat(info,":");
    strcat(info, projectName);
    //printf("%s\n", info);
    int status = connectToServer("history", info, totalLength);

    if(status == -1){
        return -1;
    }
    return 0;
}
int rollback(char * projectName, char * version){
    int pathLength = strlen(projectName)+strlen(version);
    char * path = (char *)malloc(pathLength*sizeof(char));
    strcpy(path, projectName);
    strcat(path, version);
     int nameLength = strlen(path);
    nameLength++;
    char * sizeOfLength = (char *)malloc(nameLength*sizeof(char));
    sizeOfLength++;
    sprintf(sizeOfLength, "%d", nameLength);
    sizeOfLength[nameLength] = '\0';
    int newSize = strlen(sizeOfLength);
    int totalLength = 5+nameLength+newSize;
    char * info = (char *) malloc(totalLength * sizeof(char));

    strcpy(info, "rlb:");
    strcat(info, sizeOfLength);
    strcat(info,":");
    strcat(info, path);


    int status = connectToServer("rollback", info, totalLength);
    if(status == -1){
        return -1;
    }
    return 0;

}
int checkout(char * projectName){
    int pathLength = strlen(projectName)+2;
    char * path = (char *)malloc(pathLength*sizeof(char));
    strcpy(path, ".");
    strcat(path, "/");
    strcat(path, projectName);
     DIR * dir = opendir(path);
    if(dir != NULL){
        printf("Given project already exists on the client side\n");
        return -1;
    }
    closedir(dir);
    int nameLength = strlen(projectName);
    nameLength++;
    char * sizeOfLength = (char *)malloc(nameLength*sizeof(char));
    sizeOfLength++;
    sprintf(sizeOfLength, "%d", nameLength);
    sizeOfLength[nameLength] = '\0';
    int newSize = strlen(sizeOfLength);
    int totalLength = 5+nameLength+newSize;
    char * info = (char *) malloc(totalLength * sizeof(char));

    strcpy(info, "ckt:");
    strcat(info, sizeOfLength);
    strcat(info,":");
    strcat(info, projectName);
    //printf("%s\n", info);
    int status = connectToServer("checkout", info, totalLength);

    if(status == -1){
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
    // printf("c\n");
    while(fileContent[i] != '\n'){
        *version[i] = fileContent[i];
        i++;
    }
    // printf("c\n");
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


char * compHash(char * fileContent){
 // printf("cool\n");
    int DataLen = strlen(fileContent);

    unsigned char digest[SHA256_DIGEST_LENGTH];
    int i;
     //printf("cool\n");
    SHA256(fileContent,DataLen, digest);
    unsigned char * hash = malloc(SHA256_DIGEST_LENGTH*2);
    // printf("cool\n");
     for(i=0; i<SHA256_DIGEST_LENGTH; i++){
        sprintf((char*)(hash+(i*2)), "%02x", digest[i]);
        }
    return (char *)hash;
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
int configureClient(char* ip, char* port){

    if(atoi(port)==0){
        return -1;
    }
    int length = strlen(ip)+strlen(port)+1;
    char * combined = (char *)malloc(length*sizeof(char));
    strcpy(combined, ip);
    strcat(combined, "\n");
    strcat(combined, port);
    int bytes=0;
    int totalbytes=0;
    int fd = open(".configure", O_CREAT | O_WRONLY , 00600);
    if(fd<0){
            printf("There was an error creating the .configure file\n");
            free(combined);
            return -1;
    }
    while(totalbytes!=length){
        bytes = write(fd, combined+totalbytes, length-totalbytes);
        if(bytes<0){
            printf("There was an error writing to the .configure file\n");
            free(combined);
            close(fd);
            return -1;

        }
        totalbytes=totalbytes+bytes;
    }
    free(combined);
    close(fd);
    return 0;
}

int connectToServer(char * command, char * information, int size){
    char * port1 = NULL;
    char * ip1 = NULL;
    int  test = openConfigure(&port1, &ip1);
    if(test == -1){
        return -1;
    }
    int sockfd, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    portno = atoi(port1);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        error("ERROR opening socket");
    }
    server = gethostbyname(ip1);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        return -1;
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr_list[0],(char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){
        error("ERROR connecting");
    }
    //bzero(buffer,256);
    if(strcmp(command, "create") == 0){
        n = write(sockfd,information,size);
        if (n < 0){
            error("ERROR writing to socket");
        }
        //in case of create it will either return 9 bytes or some error message OF SIZE 9 BYTES i have not made yet
        //int bytesRead = 0;
        char buffer[9];
        bzero(buffer,9);

        n = read(sockfd,buffer,9);
        if (n < 0){
             error("ERROR reading from socket");
        }
        if(strcmp(buffer, "cte:2:1\n") == 0){
            //create directory place .Manifest inside
            char extractName[100];
            extractName[0] = '.';
            extractName[1] = '/';
            int x = 4;
            int t = 2;
            while(information[x] != ':'){
                x++;
            }
            x++;
            while(x != size - 1){
                extractName[t] = information[x];
                x++;
                t++;
            }
            extractName[t] = '\0';
            int check = mkdir(extractName, 0777);
            if(check < 0){
                printf("error in making the project \n");
                return -1;
            }
            strcat(extractName, "/.Manifest");
            int fd = open(extractName, O_CREAT | O_WRONLY , 00600);
            if(fd < 0){
                printf("There was an error making a .Manifest file\n");
                return -1;
            }
            write(fd, "1", 1);
            write(fd, "\n", 1);
            close(fd);
        }else{
            //I have not make the error case yet but
            printf("there was an error on the server side making this project. The requested project may already exist.\n");
            return -1;
        }




    }else if(strcmp(command, "currentversion") == 0){
        n = write(sockfd,information,size);
        if (n < 0){
            error("ERROR writing to socket");
        }
        char buffer[5];
        buffer[4] = '\0';
        n = read(sockfd, buffer, 4);
        if(n < 0){
            printf("there was an error reading from socket\n");
            return -1;
        }
        //printf("%s\n", buffer);
        if(strcmp(buffer, "cvn:") != 0){
            printf("getting the currentversion failed\n");
            return -1;
        }
        //manifest version comparisons and then do the whole side by side
        //commence commit
        int totalBytes = 0;
        char manifestSize[200];
        bzero(manifestSize, 200);
        while(1){
            n = read(sockfd, manifestSize + totalBytes, 1);
            if(n < 0){
                printf("failed to read from socket when getting the current version\n");
                return -1;
            }
            if(manifestSize[totalBytes] == ':'){
                manifestSize[totalBytes] = '\0';
                break;
            }
            totalBytes = totalBytes + n;
        }
        int sManifestSize = atoi(manifestSize);
        char * sManifest = (char *)malloc((sManifestSize + 1)*sizeof(char));
        bzero(sManifest, (sManifestSize + 1));
        n = read(sockfd, sManifest, sManifestSize);
        if(n < 0){
            printf("error reading from socket when getting the currentversion\n");
            free(sManifest);
            return -1;
        }
        char * sVersion = NULL;
        struct manifestLL * sHead = getLL(sManifestSize, sManifest, &sVersion);
        struct manifestLL* ptr = sHead;
        while(ptr!=NULL){
            int i=0;
            int numSlashes=0;
            int fileNameLen =0;
            int pathLen = strlen(ptr->path);
            for(i=0; i<pathLen; i++){
                fileNameLen++;
                if(ptr->path[i]=='/'){
                    numSlashes++;
                    fileNameLen=0;
                }
            }
            char * fileName = (char *)malloc(fileNameLen * sizeof(char));
            int j=0;
            int k=0;
            for(j=0; j<pathLen; j++){
                if(numSlashes==0){
                    fileName[k]=ptr->path[j];
                    k++;
                }
                if(ptr->path[j]=='/'){
                    numSlashes--;
                }
            }
            printf("Filename:%s\tVersion:%s\n", fileName, ptr->version);
            ptr=ptr->next;
    }
    }else if(strcmp(command, "history") == 0){

         n = write(sockfd,information,size);
        if (n < 0){
            error("ERROR writing to socket");
        }
        char buffer[5];
        buffer[4] = '\0';
        n = read(sockfd, buffer, 4);
        if(n < 0){
            printf("there was an error reading from socket\n");
            return -1;
        }
        //printf("%s\n", buffer);
        //printf("n\n");
        if(strcmp(buffer, "hst:") != 0){
            printf("getting the history failed\n");
            return -1;
        }
        int totalBytes = 0;
        char historySize[200];
        bzero(historySize, 200);
        while(1){
            n = read(sockfd, historySize + totalBytes, 1);
            if(n < 0){
                printf("failed to read from socket when getting the history\n");
                return -1;
            }
            if(historySize[totalBytes] == ':'){
                historySize[totalBytes] = '\0';
                break;
            }
            totalBytes = totalBytes + n;
        }
         //printf("n\n");

        int sHistorySize = atoi(historySize);
        char * sHistory = (char *)malloc((sHistorySize + 1)*sizeof(char));
        bzero(sHistory, (sHistorySize + 1));
        n = read(sockfd, sHistory, sHistorySize);
        if(n < 0){
            printf("error reading from socket when getting the history\n");
            free(sHistory);
            return -1;
        }

       // char * sVersion = NULL;
        struct manifestLL * sHead = getCommitLL(sHistorySize, sHistory);
        struct manifestLL* ptr = sHead;
        while(ptr!=NULL){
            printf("%c %s %s\n", ptr->mode, ptr->path, ptr->version);
            ptr=ptr->next;

        }




    }else if(strcmp(command, "checkout") == 0){
         n = write(sockfd,information,size);
        if (n < 0){
            error("ERROR writing to socket");
        }
        char buffer[5];
        buffer[4] = '\0';
        n = read(sockfd, buffer, 4);
        if(n < 0){
            printf("there was an error reading from socket\n");
            return -1;
        }
        //printf("%s\n", buffer);
        if(strcmp(buffer, "ckt:") != 0){
            printf("checkout failed\n");
            return -1;
        }
        int totalBytes = 0;
        char tarSize[200];
        bzero(tarSize, 200);
        while(1){
            n = read(sockfd, tarSize + totalBytes, 1);
            if(n < 0){
                printf("failed to read from socket when getting the checkout\n");
                return -1;
            }
            if(tarSize[totalBytes] == ':'){
                tarSize[totalBytes] = '\0';
                break;
            }
            totalBytes = totalBytes + n;
        }
        int sTarSize = atoi(tarSize);
        char * sTar = (char *)malloc((sTarSize + 1)*sizeof(char));
        bzero(sTar, (sTarSize + 1));
        n = read(sockfd, sTar, sTarSize);
        if(n < 0){
            printf("error reading from socket when getting the checkout\n");
            free(sTar);
            return -1;
        }
            // printf("%s\n", information);

            int j =0;
            int count =0;
            int projectLen=0;
            for(j=0; j<strlen(information); j++){
            if(count==2){
                projectLen++;
            }
            if(information[j]==':'){
                count++;
            }
        }
            //printf("%d\n", count);
            count=0;
            char * projName = (char *)malloc(projectLen*sizeof(char));
            j=0;
            int k=0;
            for(j=0; j<strlen(information); j++){
           // printf("%c\n", information[j]);
            if(count==2){
                //printf("n\n");
                //printf("%s\n", information[j]);
                projName[k]=information[j];
                k++;
            }
            if(information[j]==':'){
                count++;
            }
            }
        projName[projectLen]='\0';
            // printf("%s\n", projName);
        int tarLen = strlen(projName)+9;
        char * tarPath = (char *)malloc(tarLen*sizeof(char));
        strcpy(tarPath, "./");
        strcat(tarPath, projName);
        strcat(tarPath, ".tar.gz");
        //printf("%s\n", tarPath);

    int fd = open(tarPath, O_CREAT | O_WRONLY , 00600);
    if(fd<0){
            printf("There was an error creating the checked out project\n");
            free(tarPath);
            return -1;
    }

    int length = sTarSize;
    int bytes2=0;
    int totalbytes2=0;
    while(totalbytes2!=length){
        bytes2 = write(fd, sTar+totalbytes2, length-totalbytes2);
        if(bytes2<0){
            printf("There was an error checking out the project\n");
            close(fd);
            return -1;

        }
        totalbytes2=totalbytes2+bytes2;
    }
    close(fd);
    extractProject(projName);
    remove(tarPath);
    int histLen = strlen(projName)+11;
    char * histPath = (char *)malloc(histLen*sizeof(char));
    strcpy(histPath, "./");
    strcat(histPath, projName);
    strcat(histPath, "/.History");
    int fdh = open(histPath, O_RDONLY);
    close(fdh);
    if(fdh!=-1){
        remove(histPath);
    }
    int comLen = strlen(projName)+5;
    char * comPath = (char *)malloc(comLen*sizeof(char));
    strcpy(comPath, "./");
    strcat(comPath, projName);
    strcat(comPath, "/.c");
    DIR * dir = opendir(comPath);
     closedir(dir);
    if(dir != NULL){
        remove_directory(comPath);
    }


    }else if(strcmp(command, "rollback") == 0){
        n = write(sockfd,information,size);
        if (n < 0){
            error("ERROR writing to socket");
        }
        char buffer[5];
        buffer[4] = '\0';
        n = read(sockfd, buffer, 4);
        if(n < 0){
            printf("there was an error reading from socket\n");
            return -1;
        }
        //printf("%s\n", buffer);
        //printf("n\n");
        if(strcmp(buffer, "rlb:") != 0){
            printf("completing the rollback failed\n");
            return -1;
        }

     int totalBytes = 0;
        char size[5];
        bzero(size, 5);
        while(1){
            n = read(sockfd, size + totalBytes, 1);
            if(n < 0){
                printf("failed to read from socket when getting rollback feedback\n");
                return -1;
            }
            if(size[totalBytes] == ':'){
                size[totalBytes] = '\0';
                break;
            }
            totalBytes = totalBytes + n;
        }
        int feedbackSize = atoi(size);
        char * sFeedBack = (char *)malloc((feedbackSize + 1)*sizeof(char));
        bzero(sFeedBack, (feedbackSize + 1));
        n = read(sockfd, sFeedBack, feedbackSize);
        if(n < 0){
            printf("error reading from socket when getting the currentversion\n");
            free(sFeedBack);
            return -1;
        }
        printf("%s\n", sFeedBack);
        }else if(strcmp(command, "commit") == 0){
        //do commit server crapperoonies
        n = write(sockfd,information,size);
        if (n < 0){
            error("ERROR writing to socket");
        }
        char buffer[5];
        buffer[4] = '\0';
        n = read(sockfd, buffer, 4);
        if(n < 0){
            printf("there was an error reading from socket\n");
            return -1;
        }
        if(strcmp(buffer, "cmt:") != 0){
            printf("committing failed\n");
            return -1;
        }
        //manifest version comparisons and then do the whole side by side
        //commence commit
        int totalBytes = 0;
        char manifestSize[200];
        bzero(manifestSize, 200);
        while(1){
            n = read(sockfd, manifestSize + totalBytes, 1);
            if(n < 0){
                printf("failed to read from socket during commit\n");
                return -1;
            }
            if(manifestSize[totalBytes] == ':'){
                manifestSize[totalBytes] = '\0';
                break;
            }
            totalBytes = totalBytes + n;
        }
        int sManifestSize = atoi(manifestSize);
        char * sManifest = (char *)malloc((sManifestSize + 1)*sizeof(char));
        bzero(sManifest, (sManifestSize + 1));
        n = read(sockfd, sManifest, sManifestSize);
        if(n < 0){
            printf("error reading from socket in commit\n");
            free(sManifest);
            return -1;
        }
        //copy function from server side
        //get client to make long array of its own manifest
        //send both of these manifests into ll maker
        //start commit comparison
        char * sVersion = NULL;
        struct manifestLL * sHead = getLL(sManifestSize, sManifest, &sVersion);

        char clientProject[500];
        bzero(clientProject, 500);
        strcpy(clientProject, "./");
        int i = 4;
        int j = 2;
        while(information[i]!=':'){
            i++;
        }
        i++;
        while(information[i]!='\0'){
            clientProject[j] = information[i];
            j++;
            i++;
        }
        char clientProject2[500];
        bzero(clientProject2, 500);
        strcpy(clientProject2, clientProject);
        strcat(clientProject2, "/.Commit");
        strcat(clientProject, "/.Manifest");
        char * cVersion = NULL;
        char * cManifest = NULL;
        int cManifestSize = returnContent(clientProject, &cManifest);
        struct manifestLL * cHead = getLL(cManifestSize, cManifest, &cVersion);

        setLiveHash(cHead);

        int c = 1; //commit didnt exist before
        int fd = open(clientProject2, O_RDWR);
        if(fd < 0){
            //.Commit does not exist yet
            fd = open(clientProject2, O_RDWR | O_CREAT, 00600);
            if(fd < 0){
                printf("there was an issue making the commit file\n");
                return -1;
            }
        }else{
            //.Commit already exists
            c = 0; //for this flag
        }
        close(fd);
        //struct manifestLL * getLL(int sizeOfFile, char * fileContent, char * version)
        struct manifestLL * commitHead = NULL;
        int one = modified(&commitHead, cHead, sHead);
        if(one == -1){
            printf("Commit failed\n");
            n = write(sockfd, "fls:", 4);
            if(n == -1){
                printf("issues\n");

                return -1;
            }
            if(c == 1){
                close(fd);

            }
            unlink(clientProject2);
            return -1;
        }
        one =added(&commitHead, cHead, sHead);
        if(one == -1){
            printf("Commit failed\n");
            if(c == 1){
                close(fd);

            }
            unlink(clientProject2);
            return -1;
        }
        one = deleted(&commitHead, cHead, sHead);
        if(one == -1){
            printf("Commit failed\n");
            if(c == 1){
                close(fd);

            }
            unlink(clientProject2);
            return -1;
        }
        //if success make sure to store hashcode of old commit and then delete commit and replace with new commit
        char * content;
       int oldSize;
        char * oldClientHash;
        if(c == 0){
            content = NULL;
            oldSize = returnContent(clientProject2, &content);
            oldClientHash= compHash(content);
            unlink(clientProject2);
        }

        fd = open(clientProject2, O_RDWR | O_CREAT, 00600);
        if(fd < 0){
            printf("there was an issue making the commit file\n");
            return -1;
        }
        writeToFile(commitHead, clientProject2);
        //loop for mode and path
        struct manifestLL * ptr = commitHead;
        while(ptr != NULL){
            char mod[2];
            bzero(mod, 2);
            mod[0] = ptr->mode;
            printf("%s %s\n", mod, ptr->path);
            ptr = ptr->next;
        }
        char * newCont = NULL;
        int newSize =returnContent(clientProject2, &newCont);
        char * newClientHash = compHash(newCont);
        char holding[100];
        bzero(holding, 100);
        sprintf(holding, "%d", newSize);
        if(c == 1){
            int sendLength = 7 + strlen(newClientHash) + newSize + strlen(holding);
            char * toSend = (char *)malloc(sendLength * sizeof(char));
            bzero(toSend, sendLength);
            strcpy(toSend, "cmO:");
            strcat(toSend, newClientHash);
            strcat(toSend, ":");
            strcat(toSend, holding);
            strcat(toSend, ":");
            strcat(toSend, newCont);
            n = write(sockfd, toSend, sendLength);
            if ( n < 0){
                printf("issues writing to server\n");
                unlink(clientProject2);
                return -1;
            }
        }else{
            //c == 0 old exists
            int sendLength = 7 + strlen(newClientHash) + newSize + strlen(holding) + strlen(oldClientHash);
            char * toSend = (char *)malloc(sendLength * sizeof(char));
            bzero(toSend, sendLength);
            strcpy(toSend, "cmN:");
            strcat(toSend, oldClientHash);
            strcat(toSend, ":");
            strcat(toSend, newClientHash);
            strcat(toSend, ":");
            strcat(toSend, holding);
            strcat(toSend, ":");
            strcat(toSend, newCont);
            n = write(sockfd, toSend, sendLength);
            if ( n < 0){
                printf("issues writing to server\n");
                printf("Failed to commit\n");
            //old commit and delete new
                unlink(clientProject2);
                return -1;
            }
        }
        //keep the old client content in case committing fails, in that case just write back old file
        char comm[5];
        bzero(comm, 5);

        n = read(sockfd, comm, 4);
         // printf("%s\n", comm);
        if(n == -1){
            printf("issues\n");
            printf("Failed to commit\n");
            //old commit and delete new
           unlink(clientProject2);

            return -1;
        }
        if(strcmp(comm, "fls:") == 0){
             printf("Failed to commit\n");
            //old commit and delete new
            unlink(clientProject2);
        }else{
            printf("successfully commited\n");
            return 0;

        }

    }else if(strcmp(command, "destroy") == 0){
        n = write(sockfd,information,size);
        if (n < 0){
            error("ERROR writing to socket\n");
        }
        char buffer[5];
        buffer[4] = '\0';
        n = read(sockfd, buffer, 4);
        if(n < 0){
            printf("there was an error reading from socket\n");
            return -1;
        }
        if(strcmp(buffer, "dst:") != 0){
            printf("destroying the project failed\n");
            return -1;
        }
        printf("Successfully deleted the project\n");
    }else if (strcmp(command, "update") == 0){
        n = write(sockfd, information, size);
        if (n < 0){
            error("Error writing to socket\n");
        }
        char buffer[5];
        buffer[4] = '\0';
        n = read(sockfd, buffer, 4);
        if(n < 0){
            printf("there was an error reading from socket\n");
            return -1;
        }
        if(strcmp(buffer, "upd:") != 0){
            printf("updating the project failed\n");
            return -1;
        }
         n = read(sockfd, buffer, 4);
        if(n < 0){
            printf("there was an error reading from socket\n");
            return -1;
        }
        if(strcmp(buffer, "upd:") != 0){
            printf("updating the project failed\n");
            return -1;
        }

        int totalBytes = 0;
        char manifestSize[200];
        bzero(manifestSize, 200);
        while(1){
            n = read(sockfd, manifestSize + totalBytes, 1);
            if(n < 0){
                printf("failed to read from socket during update\n");
                return -1;
            }
            if(manifestSize[totalBytes] == ':'){
                manifestSize[totalBytes] = '\0';
                break;
            }
            totalBytes = totalBytes + n;
        }
        int sManifestSize = atoi(manifestSize);
        char * sManifest = (char *)malloc((sManifestSize + 1)*sizeof(char));
        bzero(sManifest, (sManifestSize + 1));
        n = read(sockfd, sManifest, sManifestSize);
        if(n < 0){
            printf("error reading from socket in update\n");
            free(sManifest);
            return -1;
        }
        //copy function from server side
        //get client to make long array of its own manifest
        //send both of these manifests into ll maker
        //start commit comparison
        char * sVersion = NULL;
        struct manifestLL * sHead = getLL(sManifestSize, sManifest, &sVersion);

        char clientProject[500];
        bzero(clientProject, 500);
        strcpy(clientProject, "./");
        int i = 4;
        int j = 2;
        while(information[i]!=':'){
            i++;
        }
        i++;
        while(information[i]!='\0'){
            clientProject[j] = information[i];
            j++;
            i++;
        }
        char clientProject2[500];
        char clientProject3[500];
        bzero(clientProject3, 500);
        bzero(clientProject2, 500);
        strcpy(clientProject3, clientProject);
        strcpy(clientProject2, clientProject);
        strcat(clientProject2, "/.Update");
        strcat(clientProject3, "/.Conflict");
        strcat(clientProject, "/.Manifest");
        char * cVersion = NULL;
        char * cManifest = NULL;
        int cManifestSize = returnContent(clientProject, &cManifest);
        struct manifestLL * cHead = getLL(cManifestSize, cManifest, &cVersion);

        //delete previous .Update and .Conflict if exist

        int test1 = isFileEmpty(clientProject2);
        int test2 = isFileEmpty(clientProject3);
        if(test1 != 1){
            unlink(clientProject2);
        }
        if(test2 != 1){
            unlink(clientProject3);
        }

        //full successCheck
        if(strcmp(cVersion, sVersion) == 0){
            int fd = open(clientProject2, O_RDWR | O_CREAT, 00600);
            if(fd == -1){
                printf("issues making .Update file\n");
                return -1;
            }
            printf("Up To Date\n");

        }else{
            //3 partial success and 1 failure case
            struct manifestLL * conflictHead = NULL;
            struct manifestLL * updateHead = NULL;
           // struct manifestLL * clientHead = NULL;
           // struct manifestLL * serverHead = NULL;
            setLiveHash(cHead);

            partialOrFailure(&conflictHead, &updateHead, cHead, sHead);

            if(updateHead != NULL){
                 writeToFile(updateHead, clientProject2);
                 struct manifestLL * ptr = updateHead;
                 while(ptr != NULL){
                    char mod[2];
                    bzero(mod, 2);
                    mod[0] = ptr->mode;
                    printf("%s %s\n", mod, ptr->path);
                    ptr = ptr->next;
                 }
            }

            if(conflictHead != NULL){
                writeToFile(conflictHead, clientProject3);
                struct manifestLL * ptr = conflictHead;
                 while(ptr != NULL){
                    char mod[2];
                    bzero(mod, 2);
                    mod[0] = ptr->mode;
                    printf("%s %s\n", mod, ptr->path);
                    ptr = ptr->next;
                 }
            }


            //int x = 0;
        }

    }else if (strcmp(command, "upgrade") == 0){
        //check if conflict and update exist
        // 1 for file does not exist
        // 0 for file exists but is empty
        // -1 for file exists and is nOT empty
        //check only if update is empty
        char clientProject[500];
        bzero(clientProject, 500);
        strcpy(clientProject, "./");
        int i = 4;
        int j = 2;
        while(information[i]!=':'){
            i++;
        }
        i++;
        while(information[i]!='\0'){
            clientProject[j] = information[i];
            j++;
            i++;
        }
        char clientProject2[500];
        bzero(clientProject2, 500);
        strcpy(clientProject2, clientProject);
        char updateFile[1000];
        bzero(updateFile, 1000);
        strcpy(updateFile, clientProject);
        strcat(updateFile, "/.Update");
        strcat(clientProject2, "/.Conflict");
        strcat(clientProject, "/.Manifest");
        if(isFileEmpty(clientProject2) != 1){
            //might be bug make sure update doesnt create random conflict file
            printf("Please make sure to resolve all conflicts before upgrading\n");
            return -1;
        }

        if(isFileEmpty(updateFile) == 1){
            printf("please update this project first\n");
            return -1;
        }else if(isFileEmpty(updateFile) == 0){
            printf("Project up to date\n");
            unlink(updateFile);
            return 0;
        }

        n = write(sockfd, information, size);
        if (n < 0){
            error("Error writing to socket\n");
        }

        char buff[5];
        buff[4] = '\0';
        n = read(sockfd, buff, 4);
        if (n < 0){
            error("Error writing to socket\n");
        }
        if(strcmp(buff, "fls:") == 0){
            printf("failed to upgrade project");
            return -1;
        }
        //otherwise continue with upgrade


        char * updateContent = NULL;
        int sizeUpdate = returnContent(updateFile, &updateContent);
        char s[200];
        bzero(s, 200);
        sprintf(s, "%d", sizeUpdate);
        unlink(updateFile);
    //make ll of commit content
        struct manifestLL * updateHead = getCommitLL(sizeUpdate, updateContent);

        struct manifestLL * ptr = updateHead;

        while(ptr != NULL){
            if(ptr->mode == 'D'){
                unlink(ptr->path);
            }
            ptr = ptr->next;
        }
        //send update file to server
        int totalSize = 5 + strlen(s) + sizeUpdate;
        char * sendOver = malloc(totalSize*sizeof(char));
        strcpy(sendOver, "upg:");
        strcat(sendOver, s);
        strcat(sendOver, ":");
        strcat(sendOver, updateContent);
        n = write(sockfd, sendOver, totalSize);
        if(n < 0){
            return -1;
        }
        //read back the tar file then uptar

        char com[5];
        com[4] = '\0';
        n = read(sockfd, com, 4);
        if(n < 0){
            return -1;
        }
        char tarName[500];
        bzero(tarName, 500);
        i = 0;
        while(1){
            n = read(sockfd, tarName + i, 1);
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
            n = read(sockfd, tarSize + i, 1);
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
        n = read(sockfd, tarContent, actualTarSize);
        if(n < 0){
            return -1;
        }

        int fd = open(tarName, O_RDWR | O_CREAT, 00600);
        if(fd < 0){
            n = write(sockfd, "fls:", 4);
            return -1;
        }

        n = write(fd, tarContent, actualTarSize);
        close(fd);
        unlink(clientProject);

        extractProject2(tarName);
        remove(tarName);

        n = 0;
        //n = read...  recieved tarred version of project then delete update file
    }else if(strcmp(command, "push") == 0){
        char actualProject[500];
        bzero(actualProject, 500);
        char actualProject2[500];
        bzero(actualProject2, 500);
        char clientProject[500];
        bzero(clientProject, 500);
        char mani[500];
        bzero(mani, 500);
        strcpy(clientProject, "./");
        int i = 4;
        int j = 2;
        while(information[i]!=':'){
            i++;
        }
        i++;
        while(information[i]!='\0'){
            clientProject[j] = information[i];
            j++;
            i++;
        }
        strcpy(mani, clientProject);
        strcat(mani, "/.Manifest");
        strcpy(actualProject, clientProject);
        strcpy(actualProject2, clientProject);
        strcat(clientProject, "/.Commit");
         n = write(sockfd, information, size);
        if (n < 0){
            unlink(clientProject);
            error("Error writing to socket\n");
        }
        char buffer[5];
        buffer[4] = '\0';
        n = read(sockfd, buffer, 4);
        if(n < 0){
            unlink(clientProject);
            printf("there was an error reading from socket\n");
            return -1;
        }
        if(strcmp(buffer, "psh:") != 0){
            unlink(clientProject);
            printf("pushing the project failed\n");
            return -1;
        }
        //if get to this part project exists

        //upon getting psh: back, send tarred project over after deleting .commit
        char * fileContents = NULL;
        returnContent(clientProject, &fileContents);
        char * commitHash = compHash(fileContents);
       // struct manifestLL * commitHead = getCommitLL(fileSize, fileContents);

        char * m = NULL;
        int mSize = returnContent(mani, &m);
        char * ve = NULL;
        struct manifestLL * manifestHead = getLL(mSize, m, &ve);


        char hashNumSize[200];
        bzero(hashNumSize, 200);
        sprintf(hashNumSize, "%d", (int)strlen(commitHash));
        int sendSize = 5 + strlen(commitHash) + strlen(hashNumSize);
        char * checkForCommit = (char *)malloc(sendSize*sizeof(char));
        strcpy(checkForCommit, "psh:");
        strcat(checkForCommit, hashNumSize);
        strcat(checkForCommit, ":");
        strcat(checkForCommit, commitHash);

        n = write(sockfd, checkForCommit, sendSize);
        if(n < 0){
            printf("error to send over\n");
            return -1;
        }
        //now if success recieve psh:
        n = read(sockfd, buffer, 4);
        if(n < 0){
            unlink(clientProject);
            printf("there was an error reading from socket\n");
            return -1;
        }
        if(strcmp(buffer, "psh:") != 0){
            //unlink(clientProject);
            printf("pushing the project failed\n");
            return -1;
        }
        remove(clientProject);
        //on psh: snend over tarred version of project
        tarProject2(actualProject, manifestHead);
        strcat(actualProject, ".tar.gz");
        //psh:tarredname:fileSize:fileContent
        char * tarContent = NULL;
        int tarSize = returnContent(actualProject, &tarContent);
        char charTarSize[200];
        bzero(charTarSize, 200);
        sprintf(charTarSize, "%d", tarSize);
        sendSize = 6+strlen(actualProject) + strlen(charTarSize);
        char * sendOver = (char *)malloc(sendSize*sizeof(char));
        strcpy(sendOver, "psh:");
        strcat(sendOver, actualProject);
        strcat(sendOver, ":");
        strcat(sendOver, charTarSize);
        strcat(sendOver, ":");
        //strcat(sendOver, tarContent);
        n = write(sockfd, sendOver, sendSize);
        if(n == -1){
            return -1;
        }
        n = write(sockfd, tarContent, tarSize);
        if(n == -1){
            return -1;
        }
        remove(actualProject);
        char buffing[5];
        bzero(buffing, 5);
        n = read(sockfd, buffing, 4);
        if(n < 0){
            return -1;
        }
        if(strcmp(buffing, "fls:") == 0){
            return -1;
        }else{
            //succes
            i = 0;
            char hold[200];
            bzero(hold, 200);
            while(1){
                n = read(sockfd, hold + i, 1);
                if(n < 0){
                    return -1;
                }
                if(hold[i] == ':'){
                    hold[i] = '\0';
                    break;
                }
                i++;
            }
            int actualSize = atoi(hold);
            char * manifestContent = (char *)malloc(actualSize*sizeof(char));
            n = read(sockfd, manifestContent, actualSize);
            if(n < 0){
                return -1;
            }
            strcat(actualProject2, "/.Manifest");
            unlink(actualProject2);
            int fd2 = open(actualProject2, O_RDWR|O_CREAT, 00600);
            if(fd2< 0){
                return -1;
            }
            n = write(fd2, manifestContent, actualSize);
            close(fd2);
            printf("successful push\n");

        }
        //get new manifest replace old one delete tar file



        return 0;

    }

    return 0;
    //printf("%s\n",buffer);
}
int appendToEnd(char * fileName, char * entryToAppend){

    int fd = open(fileName, O_RDWR);

    int loc = lseek(fd, 0, SEEK_END);
   // printf("%d\n", loc);
    if(loc<0){
             printf("There was an error writing to the file\n");
            close(fd);
            return -1;
        }
    int length = strlen(entryToAppend);
    int bytes=0;
    int totalbytes=0;
    while(totalbytes!=length){
        bytes = write(fd, entryToAppend+totalbytes, length-totalbytes);
        if(bytes<0){
            printf("There was an error writing to the file\n");
            close(fd);
            return -1;

        }
        totalbytes=totalbytes+bytes;
    }
    return 0;

}
int openConfigure(char ** port, char ** ip){
    int fd = open(".configure", O_RDONLY);
    if(fd < 0){
        printf("Your .configure file is not set up yet\n");
        return -1;
    }

    char data[500];
    int bytes = 0;
    int totalbytes = 0;

    while(totalbytes != 500){
        bytes = read(fd, (data + totalbytes),500-totalbytes);
        if(bytes == 0){
            data[totalbytes] = '\0';
            break;
        }
        totalbytes+=bytes;
    }

    int i = 0;
    char * portNumber = (char *)malloc(250*sizeof(char));
    char * ipAddress = (char *)malloc(250*sizeof(char));

    while(data[i] != '\n'){
        ipAddress[i] = data[i];
        i++;
    }
    ipAddress[i] = '\0';
    i=i+1;
    int j = 0;
    while(data[i]!='\0'){
        portNumber[j]=data[i];
        i++;
        j++;
    }
    portNumber[j] = '\0';
    *ip = ipAddress;
    *port = portNumber;

    close(fd);
    return 0;
}

void setLiveHash(struct manifestLL * head){
    struct manifestLL * ptr = head;

    while(ptr != NULL){
        if(ptr->mode == 'R'){
            ptr->liveHash = malloc((strlen(ptr->oldHash) + 1)*sizeof(char));
            strcpy(ptr->liveHash, ptr->oldHash);
            ptr->liveHash[(strlen(ptr->oldHash))] = '\0';
            ptr = ptr->next;
        }else{
            char * content = NULL;
            returnContent(ptr->path, &content);
            ptr->liveHash = compHash(content);
            ptr = ptr->next;
        }
    }
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

          /* Skip the names "." and ".." as we don't want to recurse on them. */
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
int partialOrFailure(struct manifestLL ** conflictHead, struct manifestLL ** updateHead, struct manifestLL * clientHead, struct manifestLL * serverHead){
    struct manifestLL * temp = *updateHead;
    if(temp != NULL){
        while(temp->next != NULL){
            temp = temp->next;
        }
    }
    struct manifestLL * temp2 = *conflictHead;
    if(temp2 != NULL){
        while(temp2->next != NULL){
            temp2 = temp2->next;
        }
    }
    struct manifestLL * ptr = temp;
    struct manifestLL * ptr2 = temp2;
    struct manifestLL * cPtr = clientHead;
    struct manifestLL * sPtr = serverHead;
    while(cPtr != NULL){
        sPtr = serverHead;
        while(sPtr != NULL){
            if(strcmp(cPtr->path, sPtr->path) == 0 && cPtr->mode == 'R'){
                if((*updateHead )== NULL){
                    (*updateHead ) = malloc(sizeof(struct manifestLL));
                    (*updateHead )->mode = 'A';
                    (*updateHead )->version = (char *)malloc(20*sizeof(char));
                    bzero((*updateHead )->version, 20);
                    int v = atoi(sPtr->version);
                    v++;
                    sprintf((*updateHead )->version, "%d", v);
                    (*updateHead )->liveHash = (char *)malloc((strlen(sPtr->oldHash) + 1)*sizeof(char));
                    bzero((*updateHead )->liveHash, strlen(sPtr->oldHash)+ 1);
                    strcpy((*updateHead )->liveHash, sPtr->oldHash);
                    (*updateHead )->path = (char *)malloc((strlen(sPtr->path) + 1)*sizeof(char));
                    bzero((*updateHead )->path, strlen(sPtr->path) + 1);
                    strcpy((*updateHead )->path, sPtr->path);
                    (*updateHead )->next = NULL;
                    (*updateHead )->oldHash = NULL;
                    ptr = (*updateHead );
                }else{
                    ptr->next = malloc(sizeof(struct manifestLL));
                    ptr = ptr->next;
                    ptr->mode = 'A';
                    ptr->version = (char *)malloc(20*sizeof(char));
                    bzero(ptr->version, 20);
                    int v = atoi(sPtr->version);
                    v++;
                    sprintf(ptr->version, "%d", v);
                    ptr->liveHash = (char *)malloc((strlen(sPtr->oldHash) + 1)*sizeof(char));
                    bzero(ptr->liveHash, (strlen(sPtr->oldHash) + 1));
                    strcpy(ptr->liveHash, sPtr->oldHash);
                    ptr->path = (char *)malloc((strlen(sPtr->path) + 1)*sizeof(char));
                    bzero(ptr->path, strlen(sPtr->path));
                    strcpy(ptr->path, sPtr->path);
                    ptr->next = NULL;
                    ptr->oldHash = NULL;
                }
            }else if(strcmp(cPtr->path, sPtr->path) == 0){
                //found = 0;
                if(strcmp(cPtr->oldHash, sPtr->oldHash) != 0 && strcmp(cPtr->liveHash, cPtr->oldHash) == 0){
                    //case #1 in partial success
                    if((*updateHead )== NULL){
                        (*updateHead ) = malloc(sizeof(struct manifestLL));
                        (*updateHead )->mode = 'M';
                        (*updateHead )->version = (char *)malloc(20*sizeof(char));
                        bzero((*updateHead )->version, 20);
                        int v = atoi(cPtr->version);
                        v++;
                        sprintf((*updateHead )->version, "%d", v);
                        (*updateHead )->liveHash = (char *)malloc((strlen(sPtr->oldHash) + 1)*sizeof(char));
                        bzero((*updateHead )->liveHash, strlen(sPtr->oldHash)+ 1);
                        strcpy((*updateHead )->liveHash, sPtr->oldHash);
                        (*updateHead )->path = (char *)malloc((strlen(cPtr->path) + 1)*sizeof(char));
                        bzero((*updateHead )->path, strlen(cPtr->path) + 1);
                        strcpy((*updateHead )->path, cPtr->path);
                        (*updateHead )->next = NULL;
                        (*updateHead )->oldHash = NULL;
                        ptr = (*updateHead );
                    }else{
                        ptr->next = malloc(sizeof(struct manifestLL));
                        ptr = ptr->next;
                        ptr->mode = 'M';
                        ptr->version = (char *)malloc(20*sizeof(char));
                        bzero(ptr->version, 20);
                        int v = atoi(cPtr->version);
                        v++;
                        sprintf(ptr->version, "%d", v);
                        ptr->liveHash = (char *)malloc((strlen(sPtr->oldHash) + 1)*sizeof(char));
                        bzero(ptr->liveHash, (strlen(sPtr->oldHash) + 1));
                        strcpy(ptr->liveHash, sPtr->oldHash);
                        ptr->path = (char *)malloc((strlen(cPtr->path) + 1)*sizeof(char));
                        bzero(ptr->path, strlen(cPtr->path));
                        strcpy(ptr->path, cPtr->path);
                        ptr->next = NULL;
                        ptr->oldHash = NULL;
                    }
                }
                if(strcmp(cPtr->oldHash, sPtr->oldHash) != 0 && strcmp(cPtr->liveHash, cPtr->oldHash) != 0){
                    //failure case, make node into conflict
                    if((*conflictHead )== NULL){
                        (*conflictHead ) = malloc(sizeof(struct manifestLL));
                        (*conflictHead )->mode = 'C';
                        (*conflictHead )->version = (char *)malloc(20*sizeof(char));
                        bzero((*conflictHead )->version, 20);
                        int v = atoi(cPtr->version);
                        v++;
                        sprintf((*conflictHead )->version, "%d", v);
                        (*conflictHead )->liveHash = (char *)malloc((strlen(cPtr->liveHash) + 1)*sizeof(char));
                        bzero((*conflictHead )->liveHash, strlen(cPtr->liveHash)+ 1);
                        strcpy((*conflictHead )->liveHash, cPtr->liveHash);
                        (*conflictHead )->path = (char *)malloc((strlen(cPtr->path) + 1)*sizeof(char));
                        bzero((*conflictHead )->path, strlen(cPtr->path) + 1);
                        strcpy((*conflictHead )->path, cPtr->path);
                        (*conflictHead )->next = NULL;
                        (*conflictHead )->oldHash = NULL;
                        ptr2 = (*conflictHead );
                    }else{
                        ptr2->next = malloc(sizeof(struct manifestLL));
                        ptr2 = ptr2->next;
                        ptr2->mode = 'C';
                        ptr2->version = (char *)malloc(20*sizeof(char));
                        bzero(ptr2->version, 20);
                        int v = atoi(cPtr->version);
                        v++;
                        sprintf(ptr2->version, "%d", v);
                        ptr2->liveHash = (char *)malloc((strlen(cPtr->liveHash) + 1)*sizeof(char));
                        bzero(ptr2->liveHash, (strlen(cPtr->liveHash) + 1));
                        strcpy(ptr2->liveHash, cPtr->liveHash);
                        ptr2->path = (char *)malloc((strlen(cPtr->path) + 1)*sizeof(char));
                        bzero(ptr2->path, strlen(cPtr->path));
                        strcpy(ptr2->path, cPtr->path);
                        ptr2->next = NULL;
                        ptr2->oldHash = NULL;
                    }
                }
                    //the below is from other function
            }
            sPtr = sPtr->next;
        }
        cPtr = cPtr->next;
    }

    int found = -1;
    cPtr = clientHead;
    sPtr = serverHead;
     while(sPtr != NULL){
        found = -1;
        cPtr = clientHead;
        while(cPtr != NULL){
            if(strcmp(cPtr->path, sPtr->path) == 0){
                found = 0;
                break;
            }
            cPtr = cPtr->next;
        }
        if( found == -1){
            //not found in client but has in server
            //case #1 in partial success
            if((*updateHead )== NULL){
                (*updateHead ) = malloc(sizeof(struct manifestLL));
                (*updateHead )->mode = 'A';
                (*updateHead )->version = (char *)malloc(20*sizeof(char));
                bzero((*updateHead )->version, 20);
                int v = atoi(sPtr->version);
                v++;
                sprintf((*updateHead )->version, "%d", v);
                (*updateHead )->liveHash = (char *)malloc((strlen(sPtr->oldHash) + 1)*sizeof(char));
                bzero((*updateHead )->liveHash, strlen(sPtr->oldHash)+ 1);
                strcpy((*updateHead )->liveHash, sPtr->oldHash);
                (*updateHead )->path = (char *)malloc((strlen(sPtr->path) + 1)*sizeof(char));
                bzero((*updateHead )->path, strlen(sPtr->path) + 1);
                strcpy((*updateHead )->path, sPtr->path);
                (*updateHead )->next = NULL;
                (*updateHead )->oldHash = NULL;
                ptr = (*updateHead );
            }else{
                ptr->next = malloc(sizeof(struct manifestLL));
                ptr = ptr->next;
                ptr->mode = 'A';
                ptr->version = (char *)malloc(20*sizeof(char));
                bzero(ptr->version, 20);
                int v = atoi(sPtr->version);
                v++;
                sprintf(ptr->version, "%d", v);
                ptr->liveHash = (char *)malloc((strlen(sPtr->oldHash) + 1)*sizeof(char));
                bzero(ptr->liveHash, (strlen(sPtr->oldHash) + 1));
                strcpy(ptr->liveHash, sPtr->oldHash);
                ptr->path = (char *)malloc((strlen(sPtr->path) + 1)*sizeof(char));
                bzero(ptr->path, strlen(sPtr->path));
                strcpy(ptr->path, sPtr->path);
                ptr->next = NULL;
                ptr->oldHash = NULL;
            }
        }
        sPtr = sPtr->next;
    }

    found = -1;
    cPtr = clientHead;
    sPtr = serverHead;
     while(cPtr != NULL){
        sPtr = serverHead;
        found = -1;
        while(sPtr != NULL){
            if(strcmp(cPtr->path, sPtr->path) == 0){
                found = 0;
                break;
            }
            sPtr = sPtr->next;
        }
        if( found == -1){
            //not found in client but has in server
            //case #1 in partial success
            if((*updateHead )== NULL){
                (*updateHead ) = malloc(sizeof(struct manifestLL));
                (*updateHead )->mode = 'D';
                (*updateHead )->version = (char *)malloc(20*sizeof(char));
                bzero((*updateHead )->version, 20);
                int v = atoi(cPtr->version);
                v++;
                sprintf((*updateHead )->version, "%d", v);
                (*updateHead )->liveHash = (char *)malloc((strlen(cPtr->liveHash) + 1)*sizeof(char));
                bzero((*updateHead )->liveHash, strlen(cPtr->liveHash)+ 1);
                strcpy((*updateHead )->liveHash, cPtr->liveHash);
                (*updateHead )->path = (char *)malloc((strlen(cPtr->path) + 1)*sizeof(char));
                bzero((*updateHead )->path, strlen(cPtr->path) + 1);
                strcpy((*updateHead )->path, cPtr->path);
                (*updateHead )->next = NULL;
                (*updateHead )->oldHash = NULL;
                ptr = (*updateHead );
            }else{
                ptr->next = malloc(sizeof(struct manifestLL));
                ptr = ptr->next;
                ptr->mode = 'D';
                ptr->version = (char *)malloc(20*sizeof(char));
                bzero(ptr->version, 20);
                int v = atoi(cPtr->version);
                v++;
                sprintf(ptr->version, "%d", v);
                ptr->liveHash = (char *)malloc((strlen(cPtr->liveHash) + 1)*sizeof(char));
                bzero(ptr->liveHash, (strlen(cPtr->liveHash) + 1));
                strcpy(ptr->liveHash, cPtr->liveHash);
                ptr->path = (char *)malloc((strlen(cPtr->path) + 1)*sizeof(char));
                bzero(ptr->path, strlen(cPtr->path));
                strcpy(ptr->path, cPtr->path);
                ptr->next = NULL;
                ptr->oldHash = NULL;
            }
        }
        cPtr = cPtr->next;
    }
    return 0;
}
int writeToFile(struct manifestLL * commitHeader, char * fileName){
    int fd = open(fileName, O_RDWR | O_CREAT , 00600);
    if(fd < 0){
        printf("issue during commit\n");
        return -1;
    }
    int bytes = 0;
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
        bytes = write(fd, ptr->liveHash, strlen(ptr->liveHash));
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
int modified(struct manifestLL ** commitHead, struct manifestLL * clientHead, struct manifestLL * serverHead){
   // int found = -1;
    struct manifestLL * temp = *commitHead;
    if(temp != NULL){
        while(temp->next != NULL){
            temp = temp->next;
        }
    }

    struct manifestLL * ptr = temp;
    struct manifestLL * cPtr = clientHead;
    struct manifestLL * sPtr = serverHead;
    while(cPtr != NULL){
        sPtr = serverHead;
        while(sPtr != NULL){
            if(strcmp(cPtr->path, sPtr->path) == 0 && cPtr->mode != 'R'){
                //found = 0;
                if(atoi(cPtr->version)<atoi(sPtr->version)&& strcmp(cPtr->oldHash, sPtr->oldHash) != 0){
                    return -1;
                }
                if(strcmp(cPtr->liveHash, cPtr->oldHash) != 0){
                    //counts as modified
                    if((*commitHead )== NULL){
                        (*commitHead ) = malloc(sizeof(struct manifestLL));
                        (*commitHead )->mode = 'M';
                        (*commitHead )->version = (char *)malloc(20*sizeof(char));
                        bzero((*commitHead )->version, 20);
                        int v = atoi(cPtr->version);
                        v++;
                        sprintf((*commitHead )->version, "%d", v);
                        (*commitHead )->liveHash = (char *)malloc((strlen(cPtr->liveHash) + 1)*sizeof(char));
                        bzero((*commitHead)->liveHash, strlen(cPtr->liveHash)+ 1);
                        strcpy((*commitHead )->liveHash, cPtr->liveHash);
                        (*commitHead )->path = (char *)malloc((strlen(cPtr->path) + 1)*sizeof(char));
                        bzero((*commitHead)->path, strlen(cPtr->path) + 1);
                        strcpy((*commitHead )->path, cPtr->path);
                        (*commitHead )->next = NULL;
                        (*commitHead )->oldHash = NULL;
                        ptr = (*commitHead );
                    }else{
                        ptr->next = malloc(sizeof(struct manifestLL));
                        ptr = ptr->next;
                        ptr->mode = 'M';
                        ptr->version = (char *)malloc(20*sizeof(char));
                        bzero(ptr->version, 20);
                        int v = atoi(cPtr->version);
                        v++;
                        sprintf(ptr->version, "%d", v);
                        ptr->liveHash = (char *)malloc((strlen(cPtr->liveHash) + 1)*sizeof(char));
                        bzero(ptr->liveHash, strlen(cPtr->liveHash) + 1);
                        strcpy(ptr->liveHash, cPtr->liveHash);
                        ptr->path = (char *)malloc((strlen(cPtr->path) + 1)*sizeof(char));
                        bzero(ptr->path, strlen(cPtr->path));
                        strcpy(ptr->path, cPtr->path);
                        ptr->next = NULL;
                        ptr->oldHash = NULL;
                    }
                }
                break;
            }
            sPtr = sPtr->next;
        }
        cPtr = cPtr->next;
    }

    return 0;
}
int added(struct manifestLL ** commitHead, struct manifestLL * clientHead, struct manifestLL * serverHead){
    int found = -1;
    struct manifestLL * temp = *commitHead;

     if(temp != NULL){
        while(temp->next != NULL){
            temp = temp->next;
        }
    }
    struct manifestLL * ptr = temp;
    struct manifestLL * cPtr = clientHead;
    struct manifestLL * sPtr = serverHead;
    while(cPtr != NULL){
        found = -1;
        sPtr = serverHead;
        while(sPtr != NULL){
            if(strcmp(cPtr->path, sPtr->path) == 0){
                found = 0;
                break;
            }
            sPtr = sPtr->next;
        }
        if(found == -1 && cPtr->mode != 'R'){
            //not found in server
             if(*commitHead == NULL){
                   (*commitHead ) = malloc(sizeof(struct manifestLL));
                    (*commitHead )->mode = 'A';
                    (*commitHead )->version = (char *)malloc(20*sizeof(char));
                    bzero((*commitHead )->version, 20);
                    int v = atoi(cPtr->version);
                    v++;
                    sprintf((*commitHead )->version, "%d", v);
                    (*commitHead )->liveHash = (char *)malloc((strlen(cPtr->liveHash) + 1)*sizeof(char));
                    bzero((*commitHead)->liveHash, strlen(cPtr->liveHash)+ 1);
                    strcpy((*commitHead )->liveHash, cPtr->liveHash);
                    (*commitHead )->path = (char *)malloc((strlen(cPtr->path) + 1)*sizeof(char));
                    bzero((*commitHead)->path, strlen(cPtr->path) + 1);
                    strcpy((*commitHead )->path, cPtr->path);
                    (*commitHead )->next = NULL;
                    (*commitHead )->oldHash = NULL;
                    ptr = (*commitHead );
                }else{
                    ptr->next = malloc(sizeof(struct manifestLL));
                    ptr = ptr->next;
                    ptr->mode = 'A';
                    ptr->version = (char *)malloc(20*sizeof(char));
                    bzero(ptr->version, 20);
                    int v = atoi(cPtr->version);
                    v++;
                    sprintf(ptr->version, "%d", v);
                    ptr->liveHash = (char *)malloc((strlen(cPtr->liveHash) + 1)*sizeof(char));
                    bzero(ptr->liveHash, strlen(cPtr->liveHash) + 1);
                    strcpy(ptr->liveHash, cPtr->liveHash);
                    ptr->path = (char *)malloc((strlen(cPtr->path) + 1)*sizeof(char));
                    bzero(ptr->path, strlen(cPtr->path));
                    strcpy(ptr->path, cPtr->path);
                    ptr->next = NULL;
                    ptr->oldHash = NULL;
                }
        }
        cPtr = cPtr->next;
    }

    return 0;
}
int deleted(struct manifestLL ** commitHead, struct manifestLL * clientHead, struct manifestLL * serverHead){
      int found = -1;
    struct manifestLL * temp = *commitHead;

     if(temp != NULL){
        while(temp->next != NULL){
            temp = temp->next;
        }
    }
    struct manifestLL * ptr = temp;
    struct manifestLL * cPtr = clientHead;
    struct manifestLL * sPtr = serverHead;
    while(sPtr != NULL){
        found = -1;
        cPtr = clientHead;
        while(cPtr != NULL){
            if(strcmp(cPtr->path, sPtr->path) == 0 && cPtr->mode == 'R'){
                if((*commitHead) == NULL){
                    (*commitHead ) = malloc(sizeof(struct manifestLL));
                    (*commitHead )->mode = 'D';
                    (*commitHead )->version = (char *)malloc(20*sizeof(char));
                    bzero((*commitHead )->version, 20);
                    int v = atoi(cPtr->version);
                    v++;
                    sprintf((*commitHead )->version, "%d", v);
                    (*commitHead )->liveHash = (char *)malloc((strlen(cPtr->liveHash) + 1)*sizeof(char));
                    bzero((*commitHead)->liveHash, strlen(cPtr->liveHash)+ 1);
                    strcpy((*commitHead )->liveHash, cPtr->liveHash);
                    (*commitHead )->path = (char *)malloc((strlen(cPtr->path) + 1)*sizeof(char));
                    bzero((*commitHead)->path, strlen(cPtr->path) + 1);
                    strcpy((*commitHead )->path, cPtr->path);
                    (*commitHead )->next = NULL;
                    (*commitHead )->oldHash = NULL;
                    ptr = (*commitHead );
                }else{
                    ptr->next = malloc(sizeof(struct manifestLL));
                    ptr = ptr->next;
                    ptr->mode = 'D';
                    ptr->version = (char *)malloc(20*sizeof(char));
                    bzero(ptr->version, 20);
                    int v = atoi(cPtr->version);
                    v++;
                    sprintf(ptr->version, "%d", v);
                    ptr->liveHash = (char *)malloc((strlen(cPtr->liveHash) + 1)*sizeof(char));
                    bzero(ptr->liveHash, strlen(cPtr->liveHash) + 1);
                    strcpy(ptr->liveHash, cPtr->liveHash);
                    ptr->path = (char *)malloc((strlen(cPtr->path) + 1)*sizeof(char));
                    bzero(ptr->path, strlen(cPtr->path));
                    strcpy(ptr->path, cPtr->path);
                    ptr->next = NULL;
                    ptr->oldHash = NULL;
                }
                found = 0;
                break;
            }else if(strcmp(cPtr->path, sPtr->path) == 0 ){
                found = 0;
                break;
            }
            cPtr = cPtr->next;
        }
        if(found == -1){
            //not found in server
            if((*commitHead) == NULL){
                (*commitHead ) = malloc(sizeof(struct manifestLL));
                (*commitHead )->mode = 'D';
                (*commitHead )->version = (char *)malloc(20*sizeof(char));
                bzero((*commitHead )->version, 20);
                int v = atoi(sPtr->version);
                v++;
                sprintf((*commitHead )->version, "%d", v);
                (*commitHead )->liveHash = (char *)malloc((strlen(sPtr->oldHash) + 1)*sizeof(char));
                bzero((*commitHead)->liveHash, strlen(sPtr->oldHash)+ 1);
                strcpy((*commitHead )->liveHash, sPtr->oldHash);
                (*commitHead )->path = (char *)malloc((strlen(sPtr->path) + 1)*sizeof(char));
                bzero((*commitHead)->path, strlen(sPtr->path) + 1);
                strcpy((*commitHead )->path, sPtr->path);
                (*commitHead )->next = NULL;
                (*commitHead )->oldHash = NULL;
                ptr = (*commitHead );
            }else{
                ptr->next = malloc(sizeof(struct manifestLL));
                ptr = ptr->next;
                ptr->mode = 'D';
                ptr->version = (char *)malloc(20*sizeof(char));
                bzero(ptr->version, 20);
                int v = atoi(sPtr->version);
                v++;
                sprintf(ptr->version, "%d", v);
                ptr->liveHash = (char *)malloc((strlen(sPtr->oldHash) + 1)*sizeof(char));
                bzero(ptr->liveHash, strlen(sPtr->oldHash) + 1);
                strcpy(ptr->liveHash, sPtr->oldHash);
                ptr->path = (char *)malloc((strlen(sPtr->path) + 1)*sizeof(char));
                bzero(ptr->path, strlen(sPtr->path));
                strcpy(ptr->path, sPtr->path);
                ptr->next = NULL;
                ptr->oldHash = NULL;
            }
        }
        sPtr = sPtr->next;
    }

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
        if(ptr->mode != 'D' && ptr->mode != 'R'){
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
int extractProject(char * projectName){
    int totalLength  = strlen(projectName)+16;
    char * string = (char *)malloc(totalLength*sizeof(char));
    strcpy(string, "tar xvf ");
    strcat(string, projectName);
    strcat(string,".tar.gz");
   // printf("%s\n", string);
    system(string);
    return 0;
}

void error(char *msg)
{
    perror(msg);
    exit(1);
}
