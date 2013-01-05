#ifndef _CPU_H_
#define _CPU_H_
#include  "XC23xxBREGS.h"
void CpuUnlockProtecReg(void);
void CpuLockProtecReg(void);
void CpuFrequencyInit(void);
void CpuInitOsTick(void);

#endif /* _CPU_H_ */
