#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int sys_fork(void)
{
  return fork();
}

int sys_exit(void)
{
  exit();
  return 0; // not reached
}

int sys_wait(void)
{
  return wait();
}

int sys_kill(void)
{
  int pid;

  if (argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int sys_getpid(void)
{
  return myproc()->pid;
}

int sys_sbrk(void)
{
  int addr;
  int n;

  if (argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}

int sys_sleep(void)
{
  int n;
  uint ticks0;

  if (argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n)
  {
    if (myproc()->killed)
    {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int sys_pgtPrint(void)
{
  int count = 0;
  pde_t *pde = myproc()->pgdir;
  pte_t *pte;
  for (int i = 0; i < NPDENTRIES; i++)
  {
    if ((*pde & PTE_P) && (*pde & PTE_U))
    {
      pte = (pte_t *)P2V(PTE_ADDR(*pde));

      for (int j = 0; j < NPTENTRIES; j++)
      {
        if ((*pte & PTE_P) && (*pte & PTE_U))
        {
          pte_t physical_address = (pte_t)PTE_ADDR((*pte));
          pte_t virtual_address = PGADDR(i, j, 0);
          uint flags = PTE_FLAGS(*pte);
          int wbit = (flags & PTE_W) / 2;
          cprintf("pgdir entry number:%d, Pgt entry num: %d, Virtual addr: %d, Physical addr: %p, W-bit: %d\n", i, j, virtual_address, physical_address, wbit);
        }
        pte++;
        count++;
      }
    }
    pde++;
  }
  return 0;
}
