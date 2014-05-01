#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#define MAX_INT_HANDLERS 32

struct ihnd {
	void 	(*handler)(void *);
	void	*arg;
};

U32 interrupt_disable(void);
void interrupt_enable(U32 cpu_sr);
extern struct ihnd int_handlers[MAX_INT_HANDLERS];
int hw_interrupt_install(unsigned long vect, void (* handler)(void *), void *arg);
int hw_interrupt_mask(unsigned long vect);
int hw_interrupt_unmask(unsigned long vect);
void usr_handle();

#endif
