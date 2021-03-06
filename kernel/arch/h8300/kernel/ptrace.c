

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/smp.h>
#include <linux/errno.h>
#include <linux/ptrace.h>
#include <linux/user.h>
#include <linux/signal.h>

#include <asm/uaccess.h>
#include <asm/page.h>
#include <asm/pgtable.h>
#include <asm/system.h>
#include <asm/processor.h>
#include <asm/signal.h>

/* cpu depend functions */
extern long h8300_get_reg(struct task_struct *task, int regno);
extern int  h8300_put_reg(struct task_struct *task, int regno, unsigned long data);


void user_disable_single_step(struct task_struct *child)
{
}


void ptrace_disable(struct task_struct *child)
{
	user_disable_single_step(child);
}

long arch_ptrace(struct task_struct *child, long request, long addr, long data)
{
	int ret;

	switch (request) {
	/* read the word at location addr in the USER area. */
		case PTRACE_PEEKUSR: {
			unsigned long tmp = 0;
			
			if ((addr & 3) || addr < 0 || addr >= sizeof(struct user)) {
				ret = -EIO;
				break ;
			}
			
		        ret = 0;  /* Default return condition */
			addr = addr >> 2; /* temporary hack. */

			if (addr < H8300_REGS_NO)
				tmp = h8300_get_reg(child, addr);
			else {
				switch(addr) {
				case 49:
					tmp = child->mm->start_code;
					break ;
				case 50:
					tmp = child->mm->start_data;
					break ;
				case 51:
					tmp = child->mm->end_code;
					break ;
				case 52:
					tmp = child->mm->end_data;
					break ;
				default:
					ret = -EIO;
				}
			}
			if (!ret)
				ret = put_user(tmp,(unsigned long *) data);
			break ;
		}

      /* when I and D space are separate, this will have to be fixed. */
		case PTRACE_POKEUSR: /* write the word at location addr in the USER area */
			if ((addr & 3) || addr < 0 || addr >= sizeof(struct user)) {
				ret = -EIO;
				break ;
			}
			addr = addr >> 2; /* temporary hack. */
			    
			if (addr == PT_ORIG_ER0) {
				ret = -EIO;
				break ;
			}
			if (addr < H8300_REGS_NO) {
				ret = h8300_put_reg(child, addr, data);
				break ;
			}
			ret = -EIO;
			break ;

		case PTRACE_GETREGS: { /* Get all gp regs from the child. */
		  	int i;
			unsigned long tmp;
			for (i = 0; i < H8300_REGS_NO; i++) {
			    tmp = h8300_get_reg(child, i);
			    if (put_user(tmp, (unsigned long *) data)) {
				ret = -EFAULT;
				break;
			    }
			    data += sizeof(long);
			}
			ret = 0;
			break;
		}

		case PTRACE_SETREGS: { /* Set all gp regs in the child. */
			int i;
			unsigned long tmp;
			for (i = 0; i < H8300_REGS_NO; i++) {
			    if (get_user(tmp, (unsigned long *) data)) {
				ret = -EFAULT;
				break;
			    }
			    h8300_put_reg(child, i, tmp);
			    data += sizeof(long);
			}
			ret = 0;
			break;
		}

		default:
			ret = ptrace_request(child, request, addr, data);
			break;
	}
	return ret;
}

asmlinkage void do_syscall_trace(void)
{
	if (!test_thread_flag(TIF_SYSCALL_TRACE))
		return;
	if (!(current->ptrace & PT_PTRACED))
		return;
	ptrace_notify(SIGTRAP | ((current->ptrace & PT_TRACESYSGOOD)
				 ? 0x80 : 0));
	/*
	 * this isn't the same as continuing with a signal, but it will do
	 * for normal use.  strace only continues with a signal if the
	 * stopping signal is not SIGTRAP.  -brl
	 */
	if (current->exit_code) {
		send_sig(current->exit_code, current, 1);
		current->exit_code = 0;
	}
}
