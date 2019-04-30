#ifndef __ZXPERFEVENT_H__
#define __ZXPERFEVENT_H__
#include <unistd.h>
#include <stdlib.h>

#define ZXEVENTTOTAL 12
struct ZXPERFEVENT
{
    unsigned int eventnum;
    unsigned int config_low;
    unsigned int config_high;
    unsigned char useflag; //useflag = 0 not use; useflag = 1 use;
    unsigned char position; //position = 0 core; position = 1 uncore
    char* name;
};

//Fetch_Icache
struct ZXPERFEVENT L1I_ACCESS = {.eventnum = 0, .config_low = 0x430300, .config_high = 0x0, .useflag = 0, .position = 0, .name="L1I_ACCESS",};
struct ZXPERFEVENT L1I_MISS = {.eventnum = 1, .config_low = 0x430301, .config_high = 0x0, .useflag = 0, .position = 0, .name="L1I_MISS",};
struct ZXPERFEVENT L1I_L2_HIT = {.eventnum = 2, .config_low = 0x430302, .config_high = 0x0, .useflag = 0, .position = 0, .name="L1I_L2_HIT",};
struct ZXPERFEVENT L1I_L2_CAST_OUT = {.eventnum = 3, .config_low = 0x430303, .config_high = 0x0, .useflag = 0, .position = 0, .name="L1I_L2_CAST_OUT",};
struct ZXPERFEVENT L1I_STATE_C = {.eventnum = 4, .config_low = 0x430304, .config_high = 0x0, .useflag = 0, .position = 0, .name="L1I_STATE_C",};
struct ZXPERFEVENT L1I_STATE_U = {.eventnum = 5, .config_low = 0x430305, .config_high = 0x0, .useflag = 0, .position = 0, .name="L1I_STATE_D",};
struct ZXPERFEVENT L1I_STATE_D = {.eventnum = 6, .config_low = 0x430306, .config_high = 0x0, .useflag = 0, .position = 0, .name="L1I_STATE_D",};
struct ZXPERFEVENT L1I_STATE_R = {.eventnum = 7, .config_low = 0x430307, .config_high = 0x0, .useflag = 0, .position = 0, .name="L1I_STATE_R",};
struct ZXPERFEVENT ICACHE_IFETCH_STALL = {.eventnum = 8, .config_low = 0x430308, .config_high = 0x0, .useflag = 0, .position = 0, .name="ICACHE_IFETCH_STALL",};
struct ZXPERFEVENT ITLB_ITLB_FLUSH = {.eventnum = 9, .config_low = 0x430309, .config_high = 0x0, .useflag = 0, .position = 0, .name="ITLB_ITLB_FLUSH",};
struct ZXPERFEVENT L1I_PREFETCH = {.eventnum = 10, .config_low = 0x43030a, .config_high = 0x0, .useflag = 0, .position = 0, .name="L1I_PREFETCH",};
struct ZXPERFEVENT L1I_PREFETCH_MISS = {.eventnum = 11, .config_low = 0x43030b, .config_high = 0x0, .useflag = 0, .position = 0, .name="L1I_PREFETCH_MISS",};

//BTAC_IFETCH
struct ZXPERFEVENT BR_INST_EXEC_ALL_BRANCHES = {.eventnum = 12, .config_low = 0x430700, .config_high = 0x0, .useflag = 0, .position = 0, .name="BR_INST_EXEC_ALL_BRANCHES",};
struct ZXPERFEVENT BR_INST_EXEC_COND = {.eventnum = 13, .config_low = 0x430701, .config_high = 0x0, .useflag = 0, .position = 0, .name="BR_INST_EXEC_COND",};
struct ZXPERFEVENT BR_INST_EXEC_DIRECT_JMP = {.eventnum = 14, .config_low = 0x430702, .config_high = 0x0, .useflag = 0, .position = 0, .name="BR_INST_EXEC_DIRECT_JMP",};
struct ZXPERFEVENT BR_INST_EXEC_DIRECT_NEAR_CALL = {.eventnum = 15, .config_low = 0x430703, .config_high = 0x0, .useflag = 0, .position = 0, .name="BR_INST_EXEC_DIRECT_NEAR_CALL",};
struct ZXPERFEVENT BR_INST_EXEC_INDIRECT_JMP_NON_CALL_RET = {.eventnum = 16, .config_low = 0x430704, .config_high = 0x0, .useflag = 0, .position = 0, .name="BR_INST_EXEC_INDIRECT_JMP_NON_CALL_RET",};
struct ZXPERFEVENT BR_INST_EXEC_INDIRECT_NEAR_CALL = {.eventnum = 17, .config_low = 0x430705, .config_high = 0x0, .useflag = 0, .position = 0, .name="BR_INST_EXEC_INDIRECT_NEAR_CALL",};
struct ZXPERFEVENT BR_INST_EXEC_NONTAKEN = {.eventnum = 18, .config_low = 0x430706, .config_high = 0x0, .useflag = 0, .position = 0, .name="BR_INST_EXEC_NONTAKEN",};
struct ZXPERFEVENT BR_INST_EXEC_RETURN_NEAR = {.eventnum = 19, .config_low = 0x430707, .config_high = 0x0, .useflag = 0, .position = 0, .name="BR_INST_EXEC_RETURN_NEAR",};
struct ZXPERFEVENT BR_INST_EXEC_TAKEN = {.eventnum = 20, .config_low = 0x430708, .config_high = 0x0, .useflag = 0, .position = 0, .name="BR_INST_EXEC_TAKEN",};
struct ZXPERFEVENT BR_MISP_EXEC_ALL_BRANCHES = {.eventnum = 21, .config_low = 0x430709, .config_high = 0x0, .useflag = 0, .position = 0, .name="BR_MISP_EXEC_ALL_BRANCHES",};
struct ZXPERFEVENT BR_MISP_EXEC_COND = {.eventnum = 22, .config_low = 0x43070a, .config_high = 0x0, .useflag = 0, .position = 0, .name="BR_MISP_EXEC_COND",};
struct ZXPERFEVENT BR_MISP_EXEC_DIRECT_NEAR_CALL = {.eventnum = 23, .config_low = 0x43070b, .config_high = 0x0, .useflag = 0, .position = 0, .name="BR_MISP_EXEC_DIRECT_NEAR_CALL",};
struct ZXPERFEVENT BR_MISP_EXEC_INDIRECT_JMP_NON_CALL_RET = {.eventnum = 24, .config_low = 0x43070c, .config_high = 0x0, .useflag = 0, .position = 0, .name="BR_MISP_EXEC_INDIRECT_JMP_NON_CALL_RET",};
struct ZXPERFEVENT BR_MISP_EXEC_INDIRECT_NEAR_CALL = {.eventnum = 25, .config_low = 0x43070d, .config_high = 0x0, .useflag = 0, .position = 0, .name="BR_MISP_EXEC_INDIRECT_NEAR_CALL",};
struct ZXPERFEVENT BR_MISP_EXEC_NONTAKEN = {.eventnum = 26, .config_low = 0x43070e, .config_high = 0x0, .useflag = 0, .position = 0, .name="BR_MISP_EXEC_NONTAKEN",};
struct ZXPERFEVENT BR_MISP_EXEC_RETURN_NEAR = {.eventnum = 27, .config_low = 0x43070f, .config_high = 0x0, .useflag = 0, .position = 0, .name="BR_MISP_EXEC_RETURN_NEAR",};
struct ZXPERFEVENT BR_MISP_EXEC_TAKEN = {.eventnum = 28, .config_low = 0x430710, .config_high = 0x0, .useflag = 0, .position = 0, .name="BR_MISP_EXEC_TAKEN",};
struct ZXPERFEVENT LSD_UOPS = {.eventnum = 29, .config_low = 0x430711, .config_high = 0x0, .useflag = 0, .position = 0, .name="LSD_UOPS",};
struct ZXPERFEVENT BACLEARS_ANY = {.eventnum = 30, .config_low = 0x430712, .config_high = 0x0, .useflag = 0, .position = 0, .name="BACLEARS_ANY",};
struct ZXPERFEVENT IDQ_EMPTY = {.eventnum = 31, .config_low = 0x430713, .config_high = 0x0, .useflag = 0, .position = 0, .name="IDQ_EMPTY",};
struct ZXPERFEVENT BR_TKN_BUBBLE = {.eventnum = 32, .config_low = 0x430714, .config_high = 0x0, .useflag = 0, .position = 0, .name="BR_TKN_BUBBLE",};

//XIB_FMT
struct ZXPERFEVENT ILD_STALL_LCP = {.eventnum = 33, .config_low = 0x430800, .config_high = 0x0, .useflag = 0, .position = 0, .name="ILD_STALL_LCP",};
struct ZXPERFEVENT ILD_STALL_IQ_FULL = {.eventnum = 34, .config_low = 0x430801, .config_high = 0x0, .useflag = 0, .position = 0, .name="ILD_STALL_IQ_FULL",};
struct ZXPERFEVENT SIMD_FP_256_PACKED_SINGLE = {.eventnum = 35, .config_low = 0x430802, .config_high = 0x0, .useflag = 0, .position = 0, .name="SIMD_FP_256_PACKED_SINGLE",};
struct ZXPERFEVENT SIMD_FP_256_PACKED_DOUBLE = {.eventnum = 36, .config_low = 0x430803, .config_high = 0x0, .useflag = 0, .position = 0, .name="SIMD_FP_256_PACKED_DOUBLE",};

//XLATE
struct ZXPERFEVENT UOPS_RETIRED_MACRO_FUSION_2INST = {.eventnum = 37, .config_low = 0x430900, .config_high = 0x0, .useflag = 0, .position = 0, .name="UOPS_RETIRED_MACRO_FUSION_2INST",};
struct ZXPERFEVENT UOPS_RETIRED_MACRO_FUSION_3INST = {.eventnum = 38, .config_low = 0x430901, .config_high = 0x0, .useflag = 0, .position = 0, .name="UOPS_RETIRED_MACRO_FUSION_3INST",};

//RAT
struct ZXPERFEVENT ROB_STALL_CYCLE = {.eventnum = 39, .config_low = 0x430100, .config_high = 0x0, .useflag = 0, .position = 0, .name="ROB_STALL_CYCLE",};
struct ZXPERFEVENT UOPS_ISSUED_ANY = {.eventnum = 40, .config_low = 0x430101, .config_high = 0x0, .useflag = 0, .position = 0, .name="UOPS_ISSUED_ANY",};
struct ZXPERFEVENT IDQ_MITE_ALL_UOPS = {.eventnum = 41, .config_low = 0x430102, .config_high = 0x0, .useflag = 0, .position = 0, .name="IDQ_MITE_ALL_UOPS",};
struct ZXPERFEVENT IDQ_MS_UOPS = {.eventnum = 42, .config_low = 0x430103, .config_high = 0x0, .useflag = 0, .position = 0, .name="IDQ_MS_UOPS",};
struct ZXPERFEVENT XLATE_3UOPS = {.eventnum = 43, .config_low = 0x430104, .config_high = 0x0, .useflag = 0, .position = 0, .name="XLATE_3UOPS",};
struct ZXPERFEVENT XLATE_2UOPS = {.eventnum = 44, .config_low = 0x430105, .config_high = 0x0, .useflag = 0, .position = 0, .name="XLATE_2UOPS",};
struct ZXPERFEVENT XLATE_1UOPS = {.eventnum = 45, .config_low = 0x430106, .config_high = 0x0, .useflag = 0, .position = 0, .name="XLATE_1UOPS",};
struct ZXPERFEVENT XLATE_0UOPS = {.eventnum = 46, .config_low = 0x430107, .config_high = 0x0, .useflag = 0, .position = 0, .name="XLATE_0UOPS",};
struct ZXPERFEVENT UCODE_3UOPS = {.eventnum = 47, .config_low = 0x430108, .config_high = 0x0, .useflag = 0, .position = 0, .name="UCODE_3UOPS",};
struct ZXPERFEVENT UCODE_2UOPS = {.eventnum = 48, .config_low = 0x430109, .config_high = 0x0, .useflag = 0, .position = 0, .name="UCODE_2UOPS",};
struct ZXPERFEVENT UCODE_1UOPS = {.eventnum = 49, .config_low = 0x43010a, .config_high = 0x0, .useflag = 0, .position = 0, .name="UCODE_1UOPS",};
struct ZXPERFEVENT UCODE_0UOPS = {.eventnum = 50, .config_low = 0x43010b, .config_high = 0x0, .useflag = 0, .position = 0, .name="UCODE_0UOPS",};
struct ZXPERFEVENT NATTIVE_BRANCH_STALL = {.eventnum = 51, .config_low = 0x43010c, .config_high = 0x0, .useflag = 0, .position = 0, .name="NATTIVE_BRANCH_STALL",};
struct ZXPERFEVENT IDQ_MITE_UOPS = {.eventnum = 52, .config_low = 0x43010d, .config_high = 0x0, .useflag = 0, .position = 0, .name="IDQ_MITE_UOPS",};
struct ZXPERFEVENT RESOURCE_STALLS_FPCW = {.eventnum = 53, .config_low = 0x43010e, .config_high = 0x0, .useflag = 0, .position = 0, .name="RESOURCE_STALLS_FPCW",};
struct ZXPERFEVENT IDQ_MS_MITE_UOPS = {.eventnum = 54, .config_low = 0x43010f, .config_high = 0x0, .useflag = 0, .position = 0, .name="IDQ_MS_MITE_UOPS",};
struct ZXPERFEVENT GPR_READ_STALL = {.eventnum = 55, .config_low = 0x430110, .config_high = 0x0, .useflag = 0, .position = 0, .name="GPR_READ_STALL",};
struct ZXPERFEVENT MPR_READ_STALL = {.eventnum = 56, .config_low = 0x430111, .config_high = 0x0, .useflag = 0, .position = 0, .name="MPR_READ_STALL",};
struct ZXPERFEVENT EFLAGS_READ_STALL = {.eventnum = 57, .config_low = 0x430112, .config_high = 0x0, .useflag = 0, .position = 0, .name="EFLAGS_READ_STALL",};
struct ZXPERFEVENT DREG_READ_STALL = {.eventnum = 58, .config_low = 0x430113, .config_high = 0x0, .useflag = 0, .position = 0, .name="DREG_READ_STALL",};
struct ZXPERFEVENT SRC3_MUTIPLE_DEPENDENCY = {.eventnum = 59, .config_low = 0x430114, .config_high = 0x0, .useflag = 0, .position = 0, .name="SRC3_MUTIPLE_DEPENDENCY",};
struct ZXPERFEVENT FPSW_READ_STALL = {.eventnum = 60, .config_low = 0x430115, .config_high = 0x0, .useflag = 0, .position = 0, .name="FPSW_READ_STALL",};
struct ZXPERFEVENT DOUBLE_FENCE_CYCLE = {.eventnum = 61, .config_low = 0x430116, .config_high = 0x0, .useflag = 0, .position = 0, .name="DOUBLE_FENCE_CYCLE",};
struct ZXPERFEVENT DELAYED_BYPASS_FP = {.eventnum = 62, .config_low = 0x430117, .config_high = 0x0, .useflag = 0, .position = 0, .name="DELAYED_BYPASS_FP",};
struct ZXPERFEVENT CYCLE_ACTIVITY_CYCLES_NO_EXECUTE = {.eventnum = 63, .config_low = 0x430118, .config_high = 0x0, .useflag = 0, .position = 0, .name="CYCLE_ACTIVITY_CYCLES_NO_EXECUTE",};
struct ZXPERFEVENT RESOURCE_STALLS_ROB	 = {.eventnum = 64, .config_low = 0x430119, .config_high = 0x0, .useflag = 0, .position = 0, .name="RESOURCE_STALLS_ROB",};
struct ZXPERFEVENT IDQ_ALL_MITE_CYCLES_3_UOPS	 = {.eventnum = 65, .config_low = 0x43011a, .config_high = 0x0, .useflag = 0, .position = 0, .name="IDQ_ALL_MITE_CYCLES_3_UOPS",};
struct ZXPERFEVENT IDQ_ALL_MITE_CYCLES_ANY_UOPS	 = {.eventnum = 66, .config_low = 0x43011b, .config_high = 0x0, .useflag = 0, .position = 0, .name="IDQ_ALL_MITE_CYCLES_ANY_UOPS",};
struct ZXPERFEVENT RAT_STALLS_PARTIAL_CYCLES	 = {.eventnum = 67, .config_low = 0x43011c, .config_high = 0x0, .useflag = 0, .position = 0, .name="RAT_STALLS_PARTIAL_CYCLES",};
struct ZXPERFEVENT RAT_STALLS_FLAGS 	 = {.eventnum = 68, .config_low = 0x43011d, .config_high = 0x0, .useflag = 0, .position = 0, .name="RAT_STALLS_FLAGS",};
struct ZXPERFEVENT RAT_IDLE 	 = {.eventnum = 69, .config_low = 0x43011f, .config_high = 0x0, .useflag = 0, .position = 0, .name="RAT_IDLE",};
struct ZXPERFEVENT UCODE_CYCLES 	 = {.eventnum = 70, .config_low = 0x430120, .config_high = 0x0, .useflag = 0, .position = 0, .name="UCODE_CYCLES",};
struct ZXPERFEVENT FUSED_STRENGTH 	 = {.eventnum = 71, .config_low = 0x430121, .config_high = 0x0, .useflag = 0, .position = 0, .name="FUSED_STRENGTH",};
struct ZXPERFEVENT INT2X87_CYCLE 	 = {.eventnum = 72, .config_low = 0x430122, .config_high = 0x0, .useflag = 0, .position = 0, .name="INT2X87_CYCLE",};
struct ZXPERFEVENT UCFCEXIT_UOP 	 = {.eventnum = 73, .config_low = 0x430123, .config_high = 0x0, .useflag = 0, .position = 0, .name="UCFCEXIT_UOP",};

//RS
























void zxperfevents_init(struct  ZXPERFEVENT **ppzxperfevents)
{

    struct  ZXPERFEVENT *pzxperfevents = (struct  ZXPERFEVENT *)malloc(ZXEVENTTOTAL*sizeof(struct  ZXPERFEVENT));
    pzxperfevents[0] = L1I_ACCESS;
    pzxperfevents[1] = L1I_MISS;
    pzxperfevents[2] = L1I_L2_HIT;
    pzxperfevents[3] = L1I_L2_CAST_OUT;
    pzxperfevents[4] = L1I_STATE_C;
    pzxperfevents[5] = L1I_STATE_U;
    pzxperfevents[6] = L1I_STATE_D;
    pzxperfevents[7] = L1I_STATE_R;
    pzxperfevents[8] = ICACHE_IFETCH_STALL;
    pzxperfevents[9] = ITLB_ITLB_FLUSH;
    pzxperfevents[10] = L1I_PREFETCH;
    pzxperfevents[11] = L1I_PREFETCH_MISS;

    *ppzxperfevents =  pzxperfevents;
    pzxperfevents = NULL;
}

void zxperfevents_free(struct  ZXPERFEVENT *pzxperfevents)
{
    if(pzxperfevents != NULL)
    {
        free(pzxperfevents);
        pzxperfevents = NULL;
    }
}

struct COUNTERTAG
{
    unsigned int carry;
    unsigned int low;
    unsigned int high;
};

struct ZXCOUNTERTAG
{
    struct COUNTERTAG start;
    struct COUNTERTAG end;
};

struct ZXCOUNTERTAG zxcounter = 
{
    .start = {.carry = 0, .low = 0, .high = 0,},
    .end = {.carry = 0, .low = 0, .high = 0,},
};
#endif