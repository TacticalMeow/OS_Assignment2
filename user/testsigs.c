#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
int terminate=0;

void change_mask_test()
{
    printf("Change signal mask :\n");
    int oldmask1=sigprocmask(0x8000);
    int oldmask2=sigprocmask(0x8001);
    if(oldmask1==0 && oldmask2==0x8000)
    {
        printf("PASS \n");
    }
    else
    {
        printf("FAILED \n");
    }
    printf("Testing blocking signals:\n");
    int c_pid=fork();
    if(c_pid==0)
    {
        sleep(3);
        printf("PASS\n");
        exit(0);
    }
    else
    {
        kill(c_pid,15);
    }
    sigprocmask(0);
}


void illegal_signal_overwrite_test()
{
    printf("Overwriting the SIGKILL signal test:\n");
    if(sigaction(9,0,0)==-1)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAILED\n");
    }
}
void sigaction_sc_test()
{
    printf("Overwrite for SIGIGN:\n");
    struct sigaction* ignoreact = malloc(sizeof(struct sigaction));
    struct sigaction* oldact = malloc(sizeof(struct sigaction));
    ignoreact->sa_handler=(void*)1; //SIGIGN
    ignoreact->sigmask=0x8000;
    int result = sigaction(21,ignoreact,oldact);
    if(result==0)
    {
        if( oldact->sa_handler==(void*)-1 && oldact->sigmask==0 )
            printf("PASS\n");

    }
    else
    {
         printf("FAILED\n");
    }
    printf("Using SIGIGN:\n");
    int c_pid=fork();
    if(c_pid==0)
    {
        sleep(3);
        printf("PASS \n");
        exit(0);
    }
    else
    {
        kill(c_pid,21);
    }
    free(ignoreact);
    free(oldact);
}


void signal_handle_custom()
{
    terminate=1;
    printf("PASS\n");
}

void custom_handle_test()
{
    printf("Testing custom signal handle:\n");
    struct sigaction* custom_act = malloc(sizeof(struct sigaction));
    struct sigaction* oldact2 = malloc(sizeof(struct sigaction));
    custom_act->sa_handler=signal_handle_custom; //SIGIGN
    custom_act->sigmask=0;
    int result = sigaction(22,custom_act,oldact2);
    if(result==-1)
    {
        printf("FAILED \n");
    }
    int c_pid=fork();
    if(c_pid==0)
    {
        while(terminate==0)
            printf("HELLO");
        exit(0);
    }
    else
    {
        kill(c_pid,22);
    }
    free(custom_act);
    free(oldact2);
}

void sigstop_test()
{
    printf("Testing sigstop:\n");

}

int main(int argc,char* argv[])
{
    change_mask_test();
    sleep(5);
    illegal_signal_overwrite_test();
    sigaction_sc_test();
    sleep(5);
    custom_handle_test();
    sleep(5);
    exit(0);
}
