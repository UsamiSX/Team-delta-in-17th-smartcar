#include "headfile.h"
#pragma section all "cpu2_dsram"
//���������#pragma section all restore���֮���ȫ�ֱ���������CPU2��RAM��

void core2_main(void)
{
	disableInterrupts();
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    //�û��ڴ˴����ø��ֳ�ʼ��������





	//�ȴ����к��ĳ�ʼ�����
	IfxCpu_emitEvent(&g_cpuSyncEvent);
	IfxCpu_waitEvent(&g_cpuSyncEvent, 0xFFFF);
    enableInterrupts();
    while (TRUE)
    {
		//�û��ڴ˴���д�������

    }
}



#pragma section all restore
