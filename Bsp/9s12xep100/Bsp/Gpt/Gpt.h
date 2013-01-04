#ifndef _GPT_H_
#define _GPT_H_
/* This Gpt is For MC9S12XEP100 */
/* Will Use PIT Module */
#define GPT_TIMER1   0
#define GPT_TIMER2   1
#define GPT_TIMER3   2
#define GPT_TIMER4   3
#define GPT_TIMER5   4
#define GPT_TIMER6   5
#define GPT_TIMER7   6
#define GPT_TIMER8   7
void Gpt_Init(void);
void Gpt_StartTimer(unsigned char xTimerID,unsigned int xPeriod);
void Gpt_StopTimer(unsigned char xTimerID);

#endif /* _GPT_H_ */
