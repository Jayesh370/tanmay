#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#define MAX 20

void bubble_asc(int arr[], int n)
{ 
    int i, j, temp;
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < n - i - 1; j++)
        {
            if(arr[j] > arr[j + 1])
            {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void bubble_dsc(int arr[], int n)
{ 
    int i, j, temp;
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < n - i - 1; j++)
        {
            if(arr[j] < arr[j + 1])
            {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void print(int arr[], int n)
{
    int i;
    for(i = 0; i < n; i++)
    {
        printf("\t%d", arr[i]);
    }
    printf("\n\n");
}

int main()
{
    int i, n, arr[MAX], f, pid;
    printf("\nHOW MANY NOS DO YOU WANT IN ARRAY : ");
    scanf("%d", &n);
    printf("ENTER ARRAY ELEMENTS : ");
    for(i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);
    }

    pid = fork();

    if(pid == 0)
    {
        /* ORPHAN STATE */
        printf("\n\t\t********** ORPHAN STATE **************\n");
        sleep(5);
        printf("\n\tCHILD PROCESS PID : %d", getpid());
        printf("\n\tPARENT PROCESS PPID : %d", getppid());
        system("ps -el | grep init");

        printf("\n\nARRAY IN ASCENDING ORDER : ");
        bubble_asc(arr, n);
        print(arr, n);
    }
    else if(pid > 0)
    {
        /* ZOMBIE STATE */
        printf("\n\t\t********** ZOMBIE STATE **************\n");
        system("ps -el | grep Z");
        f = wait(0);

        printf("\n\tTERMINATED CHILD PID : %d", f);
        printf("\n\tPARENT PID (MAIN) : %d", getpid());
        printf("\n\tPARENT'S PARENT PPID (BASH) : %d", getppid());
        printf("\n\nARRAY IN DESCENDING ORDER : ");
        bubble_dsc(arr, n);
        print(arr, n);
    }
    else
        printf("\nERROR IN FORK!!");

    return 0;
}





#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>

void bubble_asc(int arr[], int n)
{
    int i, j, temp;
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < n - i - 1; j++)
        {
            if(arr[j] > arr[j + 1])
            {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    printf("\nArray in ascending order: ");
    for(i = 0; i < n; i++)
    {
        printf("\t%d", arr[i]);
    }
}

int main(int argc, char *argv[]) // Example: ./a.out ./prog2 5 2 7 9 4
{
    int i, j = 2, l, k;
    int n = argc - 2;
    int a[n];
    char *args[n + 1];

    args[0] = argv[1];
    for(i = 1; i <= n; i++, j++)
    {
        args[i] = argv[j];
    }
    args[i] = (char*)0;

    for(l = 0, i = 1; i <= n; i++, l++)
    {
        a[l] = atoi(args[i]);
    }

    bubble_asc(a, n);

    for(l = 0, i = 1; i <= n; i++, l++)
    {
        sprintf(args[i], "%d", a[l]);
    }

    pid_t pid;
    pid = fork();

    if(pid == 0)
    {
        printf("\nI am Child Process, my pid is %d\n", getpid());
        printf("I am Parent Process, my pid is %d\n", getppid());
        execve(argv[1], args, NULL);
    }
    else
    {
        k = wait(0);
        printf("\nI am Parent Process, my pid is %d\n", getpid());
        printf("I am Parent's Parent Process, my pid is %d\n\n", getppid());
    }
    return 0;
}





#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>

void bubble_dsc(int arr[], int n)
{
    int i, j, temp;
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < n - i - 1; j++)
        {
            if(arr[j] < arr[j + 1])
            {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
    printf("\nArray in descending order:");
    for(i = 0; i < n; i++)
    {
        printf("\t%d", arr[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    int i, j = 1, l;
    int n = argc - 1;
    int a[n];
    char *args[n + 1];

    printf("\nPid of the 2nd program is: %d\n", getpid());

    for(i = 0; i <= n; i++, j++)
    {
        args[i] = argv[j];
    }
    args[i] = (char*)0;

    for(l = 0, i = 0; i < n; i++, l++)
    {
        a[l] = atoi(args[i]);
    }

    bubble_dsc(a, n);
    return 0;
}
