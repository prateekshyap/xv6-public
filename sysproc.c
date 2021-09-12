#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "stddef.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
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
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_getyear(void)
{
  return 2020;
}

int
sys_draw(void)
{
  const char pic[] = "           __________ \n         .'----------`.\n         | .--------. |\n         | |########| |       __________\n         | |########| |      /__________\\\n.--------| `--------' |------|    --=-- |-------------.\n|        `----,-.-----'      |o ======  |             |\n|       ______|_|_______     |__________|             |\n|      /  kkkkkkkkkkkk  \\                             |\n|     /  kkkkkkkkkkkkkk  \\                            |\n|     ^^^^^^^^^^^^^^^^^^^^                            |\n+-----------------------------------------------------+\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\0";
  int bufferSize;
  char * buffer;

  int picSize = sizeof(pic);

  int bufferSizeFetchError = argint(1,&bufferSize) < 0 ? 1 : 0;
  int bufferFetchError = argptr(0,&buffer,bufferSize) < 0 ? 1 : -1;

  if (bufferSize < picSize || bufferSizeFetchError > 0 || bufferFetchError > 0)
    return -1;

  memmove(buffer,pic,(uint)picSize);

  return picSize;
}