//Anel Mengdigali
//Part A:

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>

/*Generally, 3 children with specific execution tasks are created by fork() system call and executed by execvp() type of exit() system call. The parent process waits its children to terminated first, this is done through waitpid() for each children. Other details about how pid of entered command and commands execution can be found along the code.*/

int main( int argc, char *argv[] ) {

    //checking number of arguments in the command line for having command/process so to avoid errors:
    if ( argc != 2 ) {
        printf( "No program name!\n" );
    }

    //allocating size for future popen stream values
    char buf[256];

    //getting an argument from the user (Linux command / process):
    char cmd[] = "pidof ";
    strcat( cmd, argv[1] );
    
    //using popen function to call pidof for of the entered command, and saving result in FILE type:
    FILE *fcmd = popen( cmd, "r" );

    //if empty:
    if ( fcmd == NULL ) {
        printf( "No such pidof!\n" );
        return 1;
    }

    //getting int value pid out of read command:
    fgets( buf, 256, fcmd );
    pid_t pid = strtoul( buf, NULL, 10 );

    //closing file:
    pclose( fcmd );
   
    //process with the pid is running if it is not negative value:
    if ( pid != -1 ) {
 
        //displaying pid as process does exist:
        printf( "Process exists in the process table with pids: %s\n", buf );
	
        //child#1: whereis command is used to execute and display where this command exists in the disk: 
        int p1 = fork(); //fork#1 to create child#1
        //if returned int is 0 --> child process:
        if ( p1 == 0 ) {

            char *arg[3] = { "whereis", argv[1], NULL }; //preparing command execution

            //execvp calls exit() by itself after execution is done --> child#1 terminates, and program returns to parent: 
            execvp( "whereis", arg ); //child#1 does its execution of the command
                     
        }
        
        //child#2: gnome-terminal -- man command is used to execute and display manual page of this command in a new terminal:
        int p2 = fork(); //fork#2 to create child#2
        //if returned int is 0 --> child process:
        if ( p2 == 0 ) {

            char *arg[5] = { "gnome-terminal", "--", "man", argv[1], NULL }; //preparing command execution
           
            //execvp calls exit() by itself after execution is done --> child#2 terminates, and program returns to parent:
            execvp( "gnome-terminal", arg ); //child#2 does its execution of the command 
                 
	}
        
	//child#3: firefox command is used to open a firefox tab displaying the google search page of the word entered using http://www.google.com/search?q:
        int p3 = fork(); //fork#3 to create child#3
        //if returned int is 0 --> child process:
        if ( p3 == 0 ) {
             
             //preparing command execution:
             char page[50] = "http://www.google.com/search?q=";
             strcat( page, argv[1] );
             char *arg[5] = { "firefox", "-new-tab", "-url", page, NULL };
                      
             //execvp calls exit() by itself after execution is done -->  child#3 terminates, and program returns to parent:
             execvp( "firefox", arg ); //child#3 does its execution of the command

        }
        	
        //Parent process: waitpid is used for parent waiting children to terminate before termination of the program:
        else {
        
             //3 int values for examinig status of the 3 children:
             int status1, status2, status3;
             
             pid_t cpid1 = waitpid( p1, &status1, 0) ; //parent wait for child#1 to terminate

             if ( status1 == 0 ) {
                  printf( "The child#1 is done without errors\n" );
             }
                          
             pid_t cpid2 = waitpid(p2, &status2, 0); //parent wait for child#2 to terminate

             if ( status2 == 0 ) {
                  printf( "The child#2 is done without errors\n" );
             }

             pid_t cpid3 = waitpid(p3, &status3, 0); //parent wait for child#3 to terminate

             if ( status3 == 0 ) {
                  printf( "The child#3 is done without errors\n" );
             }

        }
        	
   }

   //process with the pid is not running:
   else {
        //displaying an error message as process does not exist:
        printf("Process does not exist!\n");
   }

   //parent terminates:
   return 0;
    
}



