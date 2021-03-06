#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <errno.h>
#include "zxperf.h"
#include "zxperfevent.h"

#define __USE_GNU
#include <sched.h>

struct MEASUREEVENT
{
	char eventname[256];
	unsigned int configlow;
	unsigned int confighigh;
	char pmcname[256];
	unsigned int whichcore;
	unsigned int start_low;
	unsigned int start_high;
	unsigned int overflowcount;
	unsigned int end_low;
	unsigned int end_high;
};
unsigned int processcount = 0;
pid_t *childpids;
int dfd;
struct ZXPERFTAG funarg;
unsigned char perf_abort_flag = 0;
struct  ZXPERFEVENT *pzxperfevents;
struct MEASUREEVENT *pmeasureevents;
pid_t mainpid;
void split_config(char* config, unsigned int length, struct MEASUREEVENT *pmeasureevent)
{
	int i = 0;
	int j = 0;
	int index[5];
	char low[64];
	char high[64];
	j = 0;
	for(i = 0; i < length; i++)
	{
		if(config[i] == ',')
		{
			index[j] = i;
			j++;
			//printf("i is %d\n", i);
		}
	}
	for(i = index[0]+1, j = 0; i < index[1]; i++, j++)
	{
		pmeasureevent->eventname[j] = config[i];
	}
	pmeasureevent->eventname[j] = '\0';
	//printf("eventname %s \n",pmeasureevent->eventname);
	for(i = index[1]+1,j = 0; i < index[2]; i++, j++)
	{
		high[j] = config[i];
	}
	high[j] = '\0';
	for(i = index[2]+1,j = 0; i < index[3]; i++, j++)
	{
		low[j] = config[i];
	}
	low[j] = '\0';
	for(i = index[3]+1,j = 0; i < index[4]; i++, j++)
	{
		pmeasureevent->pmcname[j] = config[i];
	}
	pmeasureevent->pmcname[j] = '\0';
	//printf("pmcname %s \n",pmeasureevent->pmcname);
	sscanf(high,"%x", &pmeasureevent->confighigh);
	sscanf(low,"%x", &pmeasureevent->configlow);
}
//flag=0 pmc config; flag = 1 event config; from config to get msr and msr value
void pmc_counter_write_read_config(struct ZXPERFTAG *pzxperftag, struct MEASUREEVENT pmeasureevent, unsigned char flag)
{
	
	if(!strcmp(pmeasureevent.pmcname, "COREPMC0")) 
	{	
		if(flag == 0)
		{
			pzxperftag->zxperf.msr = IA32_MSR_PERF_PMC0;
			pzxperftag->zxperf.msr_data_high = 0x0;
			pzxperftag->zxperf.msr_data_low = 0x0;
		}
		else if(flag == 1) 
		{
			pzxperftag->zxperf.msr = IA32_MSR_PERFEVTSEL0;
			pzxperftag->zxperf.msr_data_high = pmeasureevent.confighigh;
			pzxperftag->zxperf.msr_data_low = pmeasureevent.configlow;
		}	
	}
	else if(!strcmp(pmeasureevent.pmcname, "COREPMC1"))	
	{
		if(flag == 0)
		{
			pzxperftag->zxperf.msr = IA32_MSR_PERF_PMC1;
			pzxperftag->zxperf.msr_data_high = 0x0;
			pzxperftag->zxperf.msr_data_low = 0x0;
		}
		else if(flag == 1)
		{
			pzxperftag->zxperf.msr = IA32_MSR_PERFEVTSEL1;
			pzxperftag->zxperf.msr_data_high = pmeasureevent.confighigh;
			pzxperftag->zxperf.msr_data_low = pmeasureevent.configlow;
		}

	}
	else if(!strcmp(pmeasureevent.pmcname, "COREPMC2"))
	{
		if(flag == 0)
		{
			pzxperftag->zxperf.msr = IA32_MSR_PERF_PMC2;
			pzxperftag->zxperf.msr_data_high = 0x0;
			pzxperftag->zxperf.msr_data_low = 0x0;
		}
		else if(flag == 1)
		{
			pzxperftag->zxperf.msr = IA32_MSR_PERFEVTSEL2;
			pzxperftag->zxperf.msr_data_high = pmeasureevent.confighigh;
			pzxperftag->zxperf.msr_data_low = pmeasureevent.configlow;
		}
	}
	else if(!strcmp(pmeasureevent.pmcname, "COREPMC3"))
	{
		if(flag == 0)
		{
			pzxperftag->zxperf.msr = IA32_MSR_PERF_PMC3;
			pzxperftag->zxperf.msr_data_high = 0x0;
			pzxperftag->zxperf.msr_data_low = 0x0;
		}
		else if(flag == 1)
		{
			pzxperftag->zxperf.msr = IA32_MSR_PERFEVTSEL3;
			pzxperftag->zxperf.msr_data_high = pmeasureevent.confighigh;
			pzxperftag->zxperf.msr_data_low = pmeasureevent.configlow;
		}
	}
	else if(!strcmp(pmeasureevent.pmcname, "UNCOREPMC0"))
	{
		if(flag == 0)
		{
			pzxperftag->zxperf.msr = IA32_MSR_UNCORE_PMC0;
			pzxperftag->zxperf.msr_data_high = 0x0;
			pzxperftag->zxperf.msr_data_low = 0x0;
		}
		else if(flag == 1)
		{
			pzxperftag->zxperf.msr = IA32_MSR_UNCORE_PERFEVTSEL0;
			pzxperftag->zxperf.msr_data_high = pmeasureevent.confighigh;
			pzxperftag->zxperf.msr_data_low = pmeasureevent.configlow;
		}
	}
	else if(!strcmp(pmeasureevent.pmcname, "UNCOREPMC1"))
	{
		if(flag == 0)
		{
			pzxperftag->zxperf.msr = IA32_MSR_UNCORE_PMC1;
			pzxperftag->zxperf.msr_data_high = 0x0;
			pzxperftag->zxperf.msr_data_low = 0x0;
		}
		else if(flag == 1)
		{
			pzxperftag->zxperf.msr = IA32_MSR_UNCORE_PERFEVTSEL1;
			pzxperftag->zxperf.msr_data_high = pmeasureevent.confighigh;
			pzxperftag->zxperf.msr_data_low = pmeasureevent.configlow;
		}
	}
	else if(!strcmp(pmeasureevent.pmcname, "UNCOREPMC2"))
	{
		if(flag == 0)
		{
			pzxperftag->zxperf.msr = IA32_MSR_UNCORE_PMC2;
			pzxperftag->zxperf.msr_data_high = 0x0;
			pzxperftag->zxperf.msr_data_low = 0x0;
		}
		else if(flag == 1)
		{
			pzxperftag->zxperf.msr = IA32_MSR_UNCORE_PERFEVTSEL2;
			pzxperftag->zxperf.msr_data_high = pmeasureevent.confighigh;
			pzxperftag->zxperf.msr_data_low = pmeasureevent.configlow;
		}
	}
	else if(!strcmp(pmeasureevent.pmcname, "UNCOREPMC3"))
	{
		if(flag == 0)
		{
			pzxperftag->zxperf.msr = IA32_MSR_UNCORE_PMC3;
			pzxperftag->zxperf.msr_data_high = 0x0;
			pzxperftag->zxperf.msr_data_low = 0x0;
		}
		else if(flag == 1)
		{
			pzxperftag->zxperf.msr = IA32_MSR_UNCORE_PERFEVTSEL3;
			pzxperftag->zxperf.msr_data_high = pmeasureevent.confighigh;
			pzxperftag->zxperf.msr_data_low = pmeasureevent.configlow;
		}
	}
	else if(!strcmp(pmeasureevent.pmcname, "UNCOREPMC4"))
	{
		if(flag == 0)
		{
			pzxperftag->zxperf.msr = IA32_MSR_UNCORE_PMC4;
			pzxperftag->zxperf.msr_data_high = 0x0;
			pzxperftag->zxperf.msr_data_low = 0x0;
		}
		else if(flag == 1)
		{
			pzxperftag->zxperf.msr = IA32_MSR_UNCORE_PERFEVTSEL4;
			pzxperftag->zxperf.msr_data_high = pmeasureevent.confighigh;
			pzxperftag->zxperf.msr_data_low = pmeasureevent.configlow;
		}
	}
	else if(!strcmp(pmeasureevent.pmcname, "UNCOREPMC5"))
	{
		if(flag == 0)
		{
			pzxperftag->zxperf.msr = IA32_MSR_UNCORE_PMC5;
			pzxperftag->zxperf.msr_data_high = 0x0;
			pzxperftag->zxperf.msr_data_low = 0x0;
		}
		else if(flag == 1)
		{
			pzxperftag->zxperf.msr = IA32_MSR_UNCORE_PERFEVTSEL5;
			pzxperftag->zxperf.msr_data_high = pmeasureevent.confighigh;
			pzxperftag->zxperf.msr_data_low = pmeasureevent.configlow;
		}
	}
	else if(!strcmp(pmeasureevent.pmcname, "UNCOREPMC6"))
	{
		if(flag == 0)
		{
			pzxperftag->zxperf.msr = IA32_MSR_UNCORE_PMC6;
			pzxperftag->zxperf.msr_data_high = 0x0;
			pzxperftag->zxperf.msr_data_low = 0x0;
		}
		else if(flag == 1)
		{
			pzxperftag->zxperf.msr = IA32_MSR_UNCORE_PERFEVTSEL6;
			pzxperftag->zxperf.msr_data_high = pmeasureevent.confighigh;
			pzxperftag->zxperf.msr_data_low = pmeasureevent.configlow;
		}
	}
	else if(!strcmp(pmeasureevent.pmcname, "UNCOREPMC7"))
	{
		if(flag == 0)
		{
			pzxperftag->zxperf.msr = IA32_MSR_UNCORE_PMC7;
			pzxperftag->zxperf.msr_data_high = 0x0;
			pzxperftag->zxperf.msr_data_low = 0x0;
		}
		else if(flag == 1)
		{
			pzxperftag->zxperf.msr = IA32_MSR_UNCORE_PERFEVTSEL7;
			pzxperftag->zxperf.msr_data_high = pmeasureevent.confighigh;
			pzxperftag->zxperf.msr_data_low = pmeasureevent.configlow;
		}
	}
	//printf("config_addr %x\t", pzxperftag->zxperf.msr);
	//printf("config_high %x, config_low %x \n",pzxperftag->zxperf.msr_data_high, pzxperftag->zxperf.msr_data_low);

}

void zxperf_init()
{
	int j = 0;
	struct ZXPERFTAG zxperfinit;
	if(funarg.coreeventnum > 0)
	{
		//disable
		zxperfinit.zxperf.msr = IA32_MSR_PERF_GLOBAL_CTRL;
		zxperfinit.zxperf.msr_data_high = 0x0;
		zxperfinit.zxperf.msr_data_low = 0x0;
		ioctl(dfd,ZXPERF_WRITE,&zxperfinit);
	}
	if(funarg.uncoreeventnum > 0)
	{
		//disable
		zxperfinit.zxperf.msr = IA32_MSR_UNCORE_PERF_GLOBAL_CTRL;
		zxperfinit.zxperf.msr_data_high = 0x0;
		zxperfinit.zxperf.msr_data_low = 0x0;
		ioctl(dfd,ZXPERF_WRITE,&zxperfinit);
	}
	//pmc counter clear to 0
	for(j = 0; j < (funarg.coreeventnum + funarg.uncoreeventnum); j++)
	{
		pmc_counter_write_read_config(&zxperfinit,pmeasureevents[j],0);
		ioctl(dfd,ZXPERF_WRITE,&zxperfinit);
	}
	
	if(funarg.coreeventnum > 0)
	{
		//enable
		zxperfinit.zxperf.msr = IA32_MSR_PERF_GLOBAL_CTRL;
		zxperfinit.zxperf.msr_data_high = 0xffffffff;
		zxperfinit.zxperf.msr_data_low = 0xffffffff;
		ioctl(dfd,ZXPERF_WRITE,&zxperfinit);
	}
	if(funarg.uncoreeventnum > 0)
	{
		//enable
		zxperfinit.zxperf.msr = IA32_MSR_UNCORE_PERF_GLOBAL_CTRL;
		zxperfinit.zxperf.msr_data_high = 0xffffffff;
		zxperfinit.zxperf.msr_data_low = 0xffffffff;
		ioctl(dfd,ZXPERF_WRITE,&zxperfinit);
	}
	//event config
	for(j = 0; j < (funarg.coreeventnum + funarg.uncoreeventnum); j++)
	{	
		pmc_counter_write_read_config(&zxperfinit,pmeasureevents[j],1);
		//printf("config_addr %x\t", zxperfinit.zxperf.msr);
		//printf("config_high %x, config_low %x \n",zxperfinit.zxperf.msr_data_high, zxperfinit.zxperf.msr_data_low);
		ioctl(dfd,ZXPERF_WRITE,&zxperfinit);
	}
}
void overflow_process()
{
	int j = 0;
	struct ZXPERFTAG overflow;
	for(j = 0; j < (funarg.coreeventnum + funarg.uncoreeventnum); j++)
	{
		if(!strcmp(pmeasureevents[j].pmcname, "COREPMC0"))
		{	
			//overflow status
			overflow.zxperf.msr = IA32_MSR_PERF_GLOBAL_STATUS;
			ioctl(dfd,ZXPERF_READ,&overflow);
			if(overflow.zxperf.msr_data_low >> 0 & 1)
			{
				pmeasureevents[j].overflowcount ++;
				//overflow control
				overflow.zxperf.msr = IA32_MSR_PERF_GLOBAL_OVF_CTRL;
				ioctl(dfd,ZXPERF_RDMSR,&overflow);
				setbit(overflow.zxperf.msr_data_low,0);
				ioctl(dfd,ZXPERF_WRMSR,&overflow);
				//PMC clear 0
				overflow.zxperf.msr = IA32_MSR_PERF_PMC0;
				overflow.zxperf.msr_data_high = 0x0;
				overflow.zxperf.msr_data_low = 0x0;
				ioctl(dfd,ZXPERF_WRMSR,&overflow);
			}	
		}
		else if(!strcmp(pmeasureevents[j].pmcname, "COREPMC1"))
		{	
			//overflow status
			overflow.zxperf.msr = IA32_MSR_PERF_GLOBAL_STATUS;
			ioctl(dfd,ZXPERF_READ,&overflow);
			if(overflow.zxperf.msr_data_low >> 1 & 1)
			{
				pmeasureevents[j].overflowcount ++;
				//overflow control
				overflow.zxperf.msr = IA32_MSR_PERF_GLOBAL_OVF_CTRL;
				ioctl(dfd,ZXPERF_RDMSR,&overflow);
				setbit(overflow.zxperf.msr_data_low,1);
				ioctl(dfd,ZXPERF_WRMSR,&overflow);
				//PMC clear 0
				overflow.zxperf.msr = IA32_MSR_PERF_PMC1;
				overflow.zxperf.msr_data_high = 0x0;
				overflow.zxperf.msr_data_low = 0x0;
				ioctl(dfd,ZXPERF_WRMSR,&overflow);
			}	
		}
		else if(!strcmp(pmeasureevents[j].pmcname, "COREPMC2"))
		{	
			//overflow status
			overflow.zxperf.msr = IA32_MSR_PERF_GLOBAL_STATUS;
			ioctl(dfd,ZXPERF_READ,&overflow);
			if(overflow.zxperf.msr_data_low >> 2 & 1)
			{
				pmeasureevents[j].overflowcount ++;
				//overflow control
				overflow.zxperf.msr = IA32_MSR_PERF_GLOBAL_OVF_CTRL;
				ioctl(dfd,ZXPERF_RDMSR,&overflow);
				setbit(overflow.zxperf.msr_data_low,2);
				ioctl(dfd,ZXPERF_WRMSR,&overflow);
				//PMC clear 0
				overflow.zxperf.msr = IA32_MSR_PERF_PMC2;
				overflow.zxperf.msr_data_high = 0x0;
				overflow.zxperf.msr_data_low = 0x0;
				ioctl(dfd,ZXPERF_WRMSR,&overflow);
			}	
		}
		else if(!strcmp(pmeasureevents[j].pmcname, "COREPMC3"))
		{	
			//overflow status
			overflow.zxperf.msr = IA32_MSR_PERF_GLOBAL_STATUS;
			ioctl(dfd,ZXPERF_READ,&overflow);
			if(overflow.zxperf.msr_data_low >> 3 & 1)
			{
				pmeasureevents[j].overflowcount ++;
				//overflow control
				overflow.zxperf.msr = IA32_MSR_PERF_GLOBAL_OVF_CTRL;
				ioctl(dfd,ZXPERF_RDMSR,&overflow);
				setbit(overflow.zxperf.msr_data_low,3);
				ioctl(dfd,ZXPERF_WRMSR,&overflow);
				//PMC clear 0
				overflow.zxperf.msr = IA32_MSR_PERF_PMC3;
				overflow.zxperf.msr_data_high = 0x0;
				overflow.zxperf.msr_data_low = 0x0;
				ioctl(dfd,ZXPERF_WRMSR,&overflow);
			}	
		}
		else if(!strcmp(pmeasureevents[j].pmcname, "UNCOREPMC0"))
		{	
			//overflow status
			overflow.zxperf.msr = IA32_MSR_UNCORE_PERF_GLOBAL_STATUS;
			ioctl(dfd,ZXPERF_READ,&overflow);
			if(overflow.zxperf.msr_data_low >> 0 & 1)
			{
				pmeasureevents[j].overflowcount ++;
				//overflow control
				overflow.zxperf.msr = IA32_MSR_UNCORE_GLOBAL_OVF_CTRL;
				ioctl(dfd,ZXPERF_RDMSR,&overflow);
				setbit(overflow.zxperf.msr_data_low,0);
				ioctl(dfd,ZXPERF_WRMSR,&overflow);
				//PMC clear 0
				overflow.zxperf.msr = IA32_MSR_UNCORE_PMC0;
				overflow.zxperf.msr_data_high = 0x0;
				overflow.zxperf.msr_data_low = 0x0;
				ioctl(dfd,ZXPERF_WRMSR,&overflow);
			}	
		}
		else if(!strcmp(pmeasureevents[j].pmcname, "UNCOREPMC1"))
		{	
			//overflow status
			overflow.zxperf.msr = IA32_MSR_UNCORE_PERF_GLOBAL_STATUS;
			ioctl(dfd,ZXPERF_READ,&overflow);
			if(overflow.zxperf.msr_data_low >> 1 & 1)
			{
				pmeasureevents[j].overflowcount ++;
				//overflow control
				overflow.zxperf.msr = IA32_MSR_UNCORE_GLOBAL_OVF_CTRL;
				ioctl(dfd,ZXPERF_RDMSR,&overflow);
				setbit(overflow.zxperf.msr_data_low,1);
				ioctl(dfd,ZXPERF_WRMSR,&overflow);
				//PMC clear 0
				overflow.zxperf.msr = IA32_MSR_UNCORE_PMC1;
				overflow.zxperf.msr_data_high = 0x0;
				overflow.zxperf.msr_data_low = 0x0;
				ioctl(dfd,ZXPERF_WRMSR,&overflow);
			}	
		}
		else if(!strcmp(pmeasureevents[j].pmcname, "UNCOREPMC2"))
		{	
			//overflow status
			overflow.zxperf.msr = IA32_MSR_UNCORE_PERF_GLOBAL_STATUS;
			ioctl(dfd,ZXPERF_READ,&overflow);
			if(overflow.zxperf.msr_data_low >> 2 & 1)
			{
				pmeasureevents[j].overflowcount ++;
				//overflow control
				overflow.zxperf.msr = IA32_MSR_UNCORE_GLOBAL_OVF_CTRL;
				ioctl(dfd,ZXPERF_RDMSR,&overflow);
				setbit(overflow.zxperf.msr_data_low,2);
				ioctl(dfd,ZXPERF_WRMSR,&overflow);
				//PMC clear 0
				overflow.zxperf.msr = IA32_MSR_UNCORE_PMC2;
				overflow.zxperf.msr_data_high = 0x0;
				overflow.zxperf.msr_data_low = 0x0;
				ioctl(dfd,ZXPERF_WRMSR,&overflow);
			}	
		}
		else if(!strcmp(pmeasureevents[j].pmcname, "UNCOREPMC3"))
		{	
			//overflow status
			overflow.zxperf.msr = IA32_MSR_UNCORE_PERF_GLOBAL_STATUS;
			ioctl(dfd,ZXPERF_READ,&overflow);
			if(overflow.zxperf.msr_data_low >> 3 & 1)
			{
				pmeasureevents[j].overflowcount ++;
				//overflow control
				overflow.zxperf.msr = IA32_MSR_UNCORE_GLOBAL_OVF_CTRL;
				ioctl(dfd,ZXPERF_RDMSR,&overflow);
				setbit(overflow.zxperf.msr_data_low,3);
				ioctl(dfd,ZXPERF_WRMSR,&overflow);
				//PMC clear 0
				overflow.zxperf.msr = IA32_MSR_UNCORE_PMC3;
				overflow.zxperf.msr_data_high = 0x0;
				overflow.zxperf.msr_data_low = 0x0;
				ioctl(dfd,ZXPERF_WRMSR,&overflow);
			}	
		}
	}
}

void sig_handle(int sig)
{

	switch (sig)
	{
		case SIGALRM:
		{
			overflow_process();
			break;
		}
		case SIGINT:
		{
			perf_abort_flag = 1;
			/*
			if(mainpid == getpid())
			{
				printf("end to count\n");
			}
			*/
			break;
		}
		default:
			break;
	}
}
int zxperf_process(struct ZXPERFTAG funarg)
{
	cpu_set_t mask, set, get;
	int i = 0;
	int j = 0;
	int flag = 0;
	unsigned int timecount = 0;
	struct itimerval tick;
	unsigned long long starttemp = 0;
	unsigned long long endtemp = 0;
	CPU_ZERO(&mask);  //init set and set set null
	CPU_SET(funarg.whichcpu, &mask);//put cpu to the mask
	if(sched_setaffinity(0,sizeof(mask),&mask) == -1)  //set affinity to cpu
	{
		printf("core %d: sched_setaffinity is error: %s\n", funarg.whichcpu,strerror(errno));
		return -1;
	}
	CPU_ZERO(&get);
	if(sched_getaffinity(0,sizeof(get), &get) == -1)
	{
		printf("core %d: sched_getaffinity is error: %s\n", funarg.whichcpu,strerror(errno));
		return -1;
	}
	while(!flag)
	{
		for( i = 0; i < funarg.cpunum; i++)
		{
			if(CPU_ISSET(i, &get))
			{
				//printf("child process pid is %d run on the %d cpu\n", getpid(), i);
				flag = 1;
				break;
			}
		}
	}

	dfd = open("/dev/zxperf", O_RDWR);
	if(dfd < 0)
	{
		printf("core %d: zxperf driver load fail \n", funarg.whichcpu);
		return -1;
	}
	switch(funarg.function)
	{
		case RDMSR:
		{
			ioctl(dfd,ZXPERF_RDMSR,&funarg);
			printf("core %d: msr: 0x%x  high: 0x%x  low: 0x%x \n", funarg.whichcpu,funarg.zxperf.msr, funarg.zxperf.msr_data_high, funarg.zxperf.msr_data_low);
			break;
		}
			
		case WRMSR:
		{
			ioctl(dfd,ZXPERF_WRMSR,&funarg);
			printf("core: %d: msr: 0x%x  high: 0x%x  low: 0x%x \n", funarg.whichcpu,funarg.zxperf.msr, funarg.zxperf.msr_data_high, funarg.zxperf.msr_data_low);
			break;
		}

		case PERF:
		{
		
			zxperf_init();
			//read counter
			for(j = 0; j < (funarg.coreeventnum + funarg.uncoreeventnum); j++)
			{
				pmc_counter_write_read_config(&funarg,pmeasureevents[j],0);
				ioctl(dfd,ZXPERF_READ,&funarg);
				pmeasureevents[j].start_low = funarg.zxperf.msr_data_low;
				pmeasureevents[j].start_high = funarg.zxperf.msr_data_high;
				pmeasureevents[j].whichcore = funarg.whichcpu;
				//printf("start: core %d: %s high: 0x%x  low: 0x%x\n",pmeasureevents[j].whichcore,pmeasureevents[j].eventname,pmeasureevents[j].start_high,pmeasureevents[j].start_low);
			}
			tick.it_value.tv_sec = 0;
			tick.it_value.tv_usec = 10;
			tick.it_interval.tv_sec = 1;
			tick.it_interval.tv_usec = 0;
			if(setitimer(ITIMER_REAL,&tick,NULL))
			{
				printf("core %d: setitimer fail\n", funarg.whichcpu);
			}
			while(!perf_abort_flag)
			{
				sleep(1);
			}
			for(j = 0; j < (funarg.coreeventnum + funarg.uncoreeventnum); j++)
			{
				pmc_counter_write_read_config(&funarg,pmeasureevents[j],0);
				ioctl(dfd,ZXPERF_READ,&funarg);
				pmeasureevents[j].end_low = funarg.zxperf.msr_data_low;
				pmeasureevents[j].end_high = funarg.zxperf.msr_data_high;
				pmeasureevents[j].whichcore = funarg.whichcpu;
				//printf("end: core %d: %s high: 0x%x  low: 0x%x\n",pmeasureevents[j].whichcore,pmeasureevents[j].eventname,pmeasureevents[j].start_high,pmeasureevents[j].start_low);
				//+
				endtemp = ((unsigned long)pmeasureevents[j].end_high<<32) | pmeasureevents[j].end_low;
				starttemp = ((unsigned long)pmeasureevents[j].start_high<<32) | pmeasureevents[j].start_low;
				printf("core %d\t: %s: \tovercount: %d \tcounter: 0x%llx \n",pmeasureevents[j].whichcore,pmeasureevents[j].eventname, pmeasureevents[j].overflowcount, endtemp - starttemp);
			}
			break;
		}
	
		default:
			break;
	}
	close(dfd);
	if(childpids != NULL)
	{
		free(childpids);
		childpids = NULL;
	}
	zxperfevents_free(pzxperfevents);
	return 0;

}
int main(int argc, char **argv)
{
	int timecount = 0;
	unsigned long long msrdata = 0;
	int cpunum = 0;
	int i = 0;
	int j = 0;
	pid_t pid;
	int status;
	FILE* pconfig;
	char config[512];
	char *split;
	char *result[4];
	signal(SIGALRM, sig_handle);
	signal(SIGINT, sig_handle);
	cpunum = sysconf(_SC_NPROCESSORS_CONF);
	childpids = (pid_t*)malloc(sizeof(pid_t)*cpunum);
	mainpid = getpid();
	
	if(argc == 1)
	{
		printf("rdmsr: zxperf -rdmsr msr\neg:read 0x198: $sudo  ./zxperf -rdmsr 198\n\n");
		printf("wrmsr: zxperf -wrmsr msr high low\neg:write 0x198 0x3bb53bb5 0x0: $sudo ./zxperf -wrmsr 198 3bb53bb5 0\n\n");
		printf("perf:  zxperf -perf event\n");
		return 0;
	}
	if(!strcmp(argv[1],"-rdmsr"))
	{
		sscanf(argv[2],"%x", &funarg.zxperf.msr);
		funarg.function = RDMSR;
	}
	else if(!strcmp(argv[1], "-wrmsr"))
	{
		sscanf(argv[2],"%x", &funarg.zxperf.msr);
		sscanf(argv[3],"%x", &funarg.zxperf.msr_data_high);
		sscanf(argv[4],"%x", &funarg.zxperf.msr_data_low);
		funarg.function = WRMSR;
	}
	else if(!strcmp(argv[1], "-perf"))
	{
		printf("final counter is: overcount*0xffffffffffff + counter \n");
		pconfig = fopen("coreevent.txt","r");
		while(fgets(config,512,pconfig))
		{
			if(config[0] == ',')
			{
				funarg.coreeventnum++;
			}
		}
		fclose(pconfig);
		pconfig = fopen("uncoreevent.txt","r");
		while(fgets(config,512,pconfig))
		{
			if(config[0] == ',')
			{
				funarg.uncoreeventnum++;
			}
		}
		fclose(pconfig);
		pmeasureevents = (struct MEASUREEVENT *)malloc(sizeof(struct MEASUREEVENT) * (funarg.coreeventnum + funarg.uncoreeventnum));
		pconfig = fopen("coreevent.txt","r");
		j = 0;
		
		while(fgets(config,512,pconfig))
		{
			if(config[0] == ',' )
			{	
				
				split_config(config, strlen(config),&pmeasureevents[j]);
				//printf("eventname %s\n",pmeasureevents[j].eventname);
				//printf("pmcname %s\n",pmeasureevents[j].pmcname);
				//printf("confhigh %d\n", pmeasureevents[j].confighigh);
				//printf("conflow %d\n",pmeasureevents[j].configlow);
				j++;
				
			}
		}
		fclose(pconfig);
		pconfig = fopen("uncoreevent.txt","r");
		split = NULL;
		while(fgets(config,512,pconfig))
		{
			if(config[0] == ',')
			{	
				split_config(config, strlen(config),&pmeasureevents[j]);
				//printf("eventname %s\n",pmeasureevents[j].eventname);
				//printf("pmcname %s\n",pmeasureevents[j].pmcname);
				//printf("confhigh %x\n", pmeasureevents[j].confighigh);
				//printf("conflow %x\n",pmeasureevents[j].configlow);
				j++;
			}
		}
		fclose(pconfig);
		
		zxperfevents_init(&pzxperfevents); //init zx support total events 
		funarg.function = PERF;
		
		for(i = 0; i < (funarg.coreeventnum + funarg.uncoreeventnum); i++)
		{
			pmeasureevents[i].overflowcount = 0;
			if(pmeasureevents[i].confighigh == 0 && pmeasureevents[i].configlow == 0) // low =0 and high = 0, use default msr config else use user config
			{
				for(j = 0; j < ZXEVENTTOTAL; j++)
				{	
					if(!strcmp(pmeasureevents[i].eventname,pzxperfevents[j].name))
					{
						pmeasureevents[i].confighigh = pzxperfevents[j].config_high;
						pmeasureevents[i].configlow = pzxperfevents[j].config_low;
						//printf("confihigh %d\n",pmeasureevents[i].confighigh);
						//printf("confilow %d\n",pmeasureevents[i].configlow);
						break;
					}
				}
			}
			
		}
	}
	
	//return 0;
	for(processcount = 0; processcount < cpunum; processcount++)
	{
		 pid = fork();  //pid == 0 is new child process id   
		if(pid == 0 || pid == -1) //pid == -1 fork is fail
		{
			childpids[processcount] = getpid(); //child process id
			break;
		}
	}
	for(processcount = 0; processcount < cpunum; processcount++)
	{
		if(childpids[processcount] == getpid())
		{
			funarg.cpunum = cpunum;
			funarg.whichcpu = processcount;
			//printf("child process %d pid is %d\n",processcount,getpid());
			zxperf_process(funarg);
		}
	}
	// main process wait for cllect child process in block mode
	if(mainpid == getpid())
	{
		for(processcount = 0; processcount < cpunum; processcount++)
		{
			if(wait(&status) > 0 )
			{
				//printf("collect child process %d\n", processcount+1);
			}
			else
			{
				printf("collect child process fail\n");
			}
		}
		//printf("main process %d exit\n", getpid());

	}
	
	
	return 0;
}
