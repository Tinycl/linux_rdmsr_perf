#ifndef __ZXPERF_H__
#define __ZXPERF_H__

#define ZXPERF_MAGIC 'Z'
#define ZXPERF_RDMSR _IO(ZXPERF_MAGIC,0)
#define ZXPERF_WRMSR _IO(ZXPERF_MAGIC,1)
#define ZXPERF_INIT _IO(ZXPERF_MAGIC,2)
#define ZXPERF_EVENT_SET _IO(ZXPERF_MAGIC,3)
#define ZXPERF_COUNTER_READ _IO(ZXPERF_MAGIC,4)

#define IA32_MSR_PERF_GLOBAL_CTRL 0x38f
#define IA32_MSR_PERF_GLOBAL_STATUS 0x38e
#define IA32_MSR_PERF_GLOBAL_OVF_CTRL 0x390
#define IA32_MSR_PERFEVTSEL0 0x186
#define IA32_MSR_PERFEVTSEL1 0x187
#define IA32_MSR_PERFEVTSEL2 0x188
#define IA32_MSR_PERFEVTSEL3 0x189
#define IA32_MSR_PERF_PMC0 0xc1
#define IA32_MSR_PERF_PMC1 0xc2
#define IA32_MSR_PERF_PMC2 0xc3
#define IA32_MSR_PERF_PMC3 0xc4

#define IA32_MSR_PERF_FIXED_CTR_CTRL 0x38d
#define IA32_MSR_PERF_FIXED_CTR0 0x309
#define IA32_MSR_PERF_FIXED_CTR1 0x30a
#define IA32_MSR_PERF_FIXED_CTR2 0x30b

#define IA32_MSR_UNCORE_PERF_GLOBAL_CTRL 0x391
#define IA32_MSR_UNCORE_PERF_GLOBAL_STATUS 0x392
#define IA32_MSR_UNCORE_GLOBAL_OVF_CTRL 0x393
#define IA32_MSR_UNCORE_FIXED_CTR_CTRL 0x395
#define IA32_MSR_UNCORE_FIXED_CTR0 0x394
#define IA32_MSR_UNCORE_PERFEVTSEL0 0x3c0
#define IA32_MSR_UNCORE_PERFEVTSEL1 0x3c1
#define IA32_MSR_UNCORE_PERFEVTSEL2 0x3c2
#define IA32_MSR_UNCORE_PERFEVTSEL3 0x3c3
#define IA32_MSR_UNCORE_PERFEVTSEL4 0x3c4
#define IA32_MSR_UNCORE_PERFEVTSEL5 0x3c5
#define IA32_MSR_UNCORE_PERFEVTSEL6 0x3c6
#define IA32_MSR_UNCORE_PERFEVTSEL7 0x3c7
#define IA32_MSR_UNCORE_PMC0 0x3b0
#define IA32_MSR_UNCORE_PMC1 0x3b1
#define IA32_MSR_UNCORE_PMC2 0x3b2
#define IA32_MSR_UNCORE_PMC3 0x3b3
#define IA32_MSR_UNCORE_PMC4 0x3b4
#define IA32_MSR_UNCORE_PMC5 0x3b5
#define IA32_MSR_UNCORE_PMC6 0x3b6
#define IA32_MSR_UNCORE_PMC7 0x3b7

struct ZXPERF
{
	unsigned int msr;
	unsigned int msr_data_low;
	unsigned int msr_data_high;

	unsigned int core_msr_perf_global_ctrl;
	unsigned int core_msr_perf_global_ctrl_data_low;
	unsigned int core_msr_perf_global_ctrl_data_high;


	unsigned int core_msr_perf_config_addr;
	unsigned int core_msr_perf_config_data_low;
	unsigned int core_msr_perf_config_data_high;
	unsigned int core_msr_pmc_addr;
	unsigned int core_msr_pmc_data_low;
	unsigned int core_msr_pmc_data_high;
};



enum ZXFUNCTION
{
	RDMSR = 0,
	WRMSR,
	PERF,
};
struct ZXPERFTAG
{
	unsigned int cpunum;  //4
	unsigned int whichcpu; //0 1 2 3
	enum ZXFUNCTION function; 
	unsigned int coreeventnum;
	unsigned int uncoreeventnum;
	struct ZXPERF zxperf;
};





#endif
