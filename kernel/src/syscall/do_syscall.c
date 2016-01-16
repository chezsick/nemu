#include "irq.h"

#include <sys/syscall.h>

void add_irq_handle(int, void (*)(void));
void mm_brk(uint32_t);
int fs_write(int, void *, int);

static void sys_brk(TrapFrame *tf) {
#ifdef IA32_PAGE
	mm_brk(tf->ebx);
#endif
	tf->eax = 0;
}
char ch;
void serial_printc(char);
int sys_write(int fd, void *buf, int len) {
	if ((fd==1)||(fd==2))
 	{
		//asm volatile (".byte 0xd6" : : "a"(2), "c"(buf), "d"(len));
		assert(0);
		int i;
		for (i=0; i<len; i++) {
			ch = ((char*)buf)[i];
			assert(!(ch == 'H'));
			serial_printc(((char*)buf)[i]);
			//Log("sys_write:%c", ((char*)buf)[i]);
			//set_bp();
		}
	}
	//ide_write(buf, file_table[fd-3].disk_offset+file_state[fd].offset, len);
	//file_state[fd].offset += len;
	return len;
}

void do_syscall(TrapFrame *tf) {
	switch(tf->eax) {
 		/* The ``add_irq_handle'' system call is artificial. We use it to 
		 * let user program register its interrupt handlers. But this is 
		 * very dangerous in a real operating system. Therefore such a 
		 * system call never exists in GNU/Linux.
		 */
		case 0: 
			cli();
			add_irq_handle(tf->ebx, (void*)tf->ecx);
			sti();
			break;

		case SYS_brk: sys_brk(tf); break;

		/* TODO: Add more system calls. */
		case SYS_write:
			//tf->eax = fs_write(tf->ebx,(void*)tf->ecx, tf->edx);
			//assert(0);
			tf->eax = sys_write(tf->ebx, (void*)tf->ecx, tf->edx);
			break;
		/*case SYS_read:
			tf->eax = fs_read(tf->ebx, (void*)tf->ecx, tf->edx);
			break;
		case SYS_open:
			tf->eax = fs_open((char*)tf->ebx, tf->ecx);
			break;
		case SYS_close:
			tf->eax = fs_close(tf->ebx);
		        break;
		case SYS_lseek:
			tf->eax = fs_lseek(tf->ebx, tf->ecx, tf->edx);
			break;
		*/
		default: panic("Unhandled system call: id = %d", tf->eax);
	}
}

