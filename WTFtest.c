#include <stdio.h>
#include <stdlib.h>

int main()
{
    system("mkdir server && mkdir client1 && mkdir client2");
    system("cp WTFserver server/ && cp WTF client1/ && cp WTF client2/");

    printf("Welcome to the GIT-like tester\n");
    sleep(1);
    printf("There will be helper statements during execution for readabilty\n");
    sleep(1);
    printf("2 clients will be running in order to test effectiveness\n");
    sleep(1);
    printf("Begin:\n");
    printf("starting server:\n");
    chdir("./server");
    system("./WTFserver 9521 &");
     sleep(3);
    printf("Configuring client1\n");
    //system("cd ..");
    //system("cd WTFclient");
    chdir("../client1");
    system("./WTF configure $HOSTNAME 9521");

    printf("First client: Creating a Project\n\n");
    system("./WTF create Project");
    sleep(1);


    printf("Configuring client2\n");
    chdir("../client2");

    system("./WTF configure $HOSTNAME 9521");

    printf("Second client: Now running checkout\n\n");
    system("./WTF checkout Project");


    chdir("../client1");

    printf("First client: now artificially add files for the test\n\n");

    system("echo \"this is a test also\" > ./Project/file1" );
    system("echo \"this is a test another test\" > ./Project/file2");
    system("echo \"this is duck duck goose a test\" > ./Project/file3");
    system("echo \"this is a cat dog with potato test\" > ./Project/file4");

    system("./WTF add Project file1");
    system("./WTF add Project file2");
    system("./WTF add Project file3");
    system("./WTF add Project file4");

    printf("First client: Will now artificially remove a file for the test\n\n");
    system("unlink ./Project/file4");
    system("./WTF remove Project file4");

    printf("First client: Now committing the project\n\n");
    system("./WTF commit Project");
    printf("First client: Now pushing the project\n\n");
    system("./WTF push Project");

    printf("Second client: Running history\n\n");
    chdir("../client2");
    system("./WTF history Project");

    printf("Second client: Now displaying the current version\n\n");
    system("./WTF currentversion Project");


    printf("Second client: Now updating the project\n\n");
    system("./WTF update Project");

    printf("Second client: Now upgrading the project\n\n");
    system("./WTF upgrade Project");

    printf("Second client: Running rollback to first version\n\n");
    system("./WTF rollback Project 1");

    printf("Second client: finally, running destroy to delete on server side\n\n");
    system("./WTF destroy Project");
    chdir("..");

    sleep(1);
    system("pkill WTFserver");
    system("rm -r server");
    system("rm -r client1");
    system("rm -r client2");
    //system("pkill WTFserver");

    //system("^C");
    return 0;
}
