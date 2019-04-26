#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include "zxperf.h"

int main(int argc, char **argv)
{
	int dfd;
	struct ZXPERF chx002perf;
	int timecount = 0;
	unsigned long long msrdata = 0;
	if(argc == 1)
	{
		printf("rdmsr: zxperf -rdmsr msr\neg:read 0x198, zxperf -rdmsr 198\n\n");
		printf("wrmsr: zxperf -wrmsr msr high low\neg:write 0x198 0x3bb53bb5 0x0, zxperf -wrmsr 198 3bb53bb5 0\n\n");
		printf("perf:  zxperf -perf event\n");
		return 0;
	}
	if(!strcmp(argv[1],"-rdmsr"))
	{
		printf("now read msr:\n");
		sscanf(argv[2],"%x", &chx002perf.msr);
		dfd = open("/dev/zxperf", O_RDWR);
		if(dfd < 0)
		{
			printf("zxperf driver load fail \n");
			return -1;
		}
		ioctl(dfd,ZXPERF_RDMSR,&chx002perf);
		printf("msr %x : high %x  low %x\n", chx002perf.msr, chx002perf.msr_data_high, chx002perf.msr_data_low);
		close(dfd);
	}
	if(!strcmp(argv[1], "-wrmsr"))
	{
		sscanf(argv[2],"%x", &chx002perf.msr);
		sscanf(argv[3],"%x", &chx002perf.msr_data_high);
		sscanf(argv[4],"%x", &chx002perf.msr_data_low);
		dfd = open("/dev/zxperf", O_RDWR);
		if(dfd < 0)
		{
			printf("zxperf driver load fail \n");
			return -1;
		}
		ioctl(dfd,ZXPERF_WRMSR,&chx002perf);
		printf("msr %x : high %x  low %x\n", chx002perf.msr, chx002perf.msr_data_high,  chx002perf.msr_data_low);
		close(dfd);
	}
	if(!strcmp(argv[1], "-perf"))
	{
		dfd = open("/dev/zxperf", O_RDWR);
		if(dfd < 0)
		{
			printf("zxperf driver load fail \n");
			return -1;
		}
		memset(&chx002perf,0,sizeof(struct ZXPERF));
		chx002perf.core_msr_perf_global_ctrl = IA32_MSR_PERF_GLOBAL_CTRL;
		ioctl(dfd,ZXPERF_INIT,&chx002perf);
		chx002perf.core_msr_perfevtsl0 = IA32_MSR_PERFEVTSEL0;
		chx002perf.core_msr_perfevtsl0_data_low = 0x430300;
		chx002perf.core_msr_perfevtsl0_data_high = 0x0;
		chx002perf.core_msr_pmc0 = IA32_MSR_PERF_PMC0;
		ioctl(dfd,ZXPERF_EVENT_SET,&chx002perf);
		ioctl(dfd,ZXPERF_COUNTER_READ,&chx002perf);
		printf("core_msr_pmc0_counter is %x \n", chx002perf.core_msr_pmc0_data_low);
		while(timecount < 10)
		{
			timecount++;
			sleep(1);
			ioctl(dfd,ZXPERF_COUNTER_READ,&chx002perf);
			printf("core_msr_pmc0_counter is %x \n", chx002perf.core_msr_pmc0_data_low);
		}
		close(dfd);
	}
	
	return 0;
}
