#include "../kernel/include/var_define.h"
#include "include/hw_include.h"
/* Disable or1200 interrupt */
U32 interrupt_disable(void)
{
	U32 cpu_sr;
 
	cpu_sr = mfspr(SPR_SR);
	mtspr(SPR_SR, cpu_sr & ~(SPR_SR_IEE | SPR_SR_TEE));
	return cpu_sr;
}

/* Enable or1200 interrupt */ 
void interrupt_enable(U32 cpu_sr)
{
	mtspr(SPR_SR, cpu_sr); /* | (SPR_SR_IEE | SPR_SR_TEE) */
}

