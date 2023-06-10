#include "types.h"
#include "stat.h"
#include "user.h"

#define N 10000 // global array size - change to see effect. Try 3000, 5000, 10000
int glob[N];
int main()
{
    glob[0] = 2; // initialize with any integer value
    printf(1, "global addr from user space: %x\n", glob);
    for (int i = 1; i < N; i++)
    {
        glob[i] = 2;
    }
    int pid = fork();

    if (pid < 0) // error
    {
        printf(2, "Fork failed\n");
    }
    else if (pid == 0) // child process
    {
        printf(1, "\nInitial child page table:\n");
        pgtPrint();
        printf(1, "\n");
        for (int i = 0; i < N / 2; i++) // for loop that modifies the starting few pages of child.
        {
            glob[i] = 1;
        }

        glob[3 * N / 4] = 1; // modify a page towards the end.
        printf(1, "\nFinal child page table:\n");
        pgtPrint();
        printf(1, "\n");
    }
    else // parent process
    {
        wait();
        printf(1, "Initial Parent page table:\n\n");
        pgtPrint();
    }

    exit();
}
