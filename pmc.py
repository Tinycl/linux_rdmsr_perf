#!/usr/bin/python
import os
import sys
import time



IA32_MSR_PERF_GLOBAL_CTRL  = 0x38f
IA32_MSR_PERF_GLOBAL_STATUS  = 0x38e
IA32_MSR_PERF_GLOBAL_OVF_CTRL  = 0x390
'''
EVTSEL 
[7:0] event
[11:8] unit
[15:12] counter index 
[16] user
[17] os
[18] edge
[19] resrved
[20] overflow PMI
[21] reserved
[22] enable
[23] inverbit
[26:24] cmask
[27] inter except
[63:28] reserved
'''
IA32_MSR_PERFEVTSEL0  = 0x186
IA32_MSR_PERFEVTSEL1  = 0x187
IA32_MSR_PERFEVTSEL2  = 0x188
IA32_MSR_PERFEVTSEL3  = 0x189

'''
PMC 
[47:0] value   
[63:48] reserve
'''
IA32_MSR_PERF_PMC0  = 0xc1
IA32_MSR_PERF_PMC1  = 0xc2
IA32_MSR_PERF_PMC2  = 0xc3
IA32_MSR_PERF_PMC3  = 0xc4

IA32_MSR_PERF_FIXED_CTR_CTRL = 0x38d
IA32_MSR_PERF_FIXED_CTR0 = 0x309
IA32_MSR_PERF_FIXED_CTR1  = 0x30a
IA32_MSR_PERF_FIXED_CTR2 = 0x30b

IA32_MSR_UNCORE_PERF_GLOBAL_CTRL = 0x391
IA32_MSR_UNCORE_PERF_GLOBAL_STATUS = 0x392
IA32_MSR_UNCORE_GLOBAL_OVF_CTRL = 0x393
IA32_MSR_UNCORE_FIXED_CTR_CTRL = 0x395
IA32_MSR_UNCORE_FIXED_CTR0 = 0x394
IA32_MSR_UNCORE_PERFEVTSEL0 = 0x3c0
IA32_MSR_UNCORE_PERFEVTSEL1 = 0x3c1
IA32_MSR_UNCORE_PERFEVTSEL2 = 0x3c2
IA32_MSR_UNCORE_PERFEVTSEL3 = 0x3c3 
IA32_MSR_UNCORE_PERFEVTSEL4 = 0x3c4 # no work
IA32_MSR_UNCORE_PERFEVTSEL5 = 0x3c5
IA32_MSR_UNCORE_PERFEVTSEL6 = 0x3c6
IA32_MSR_UNCORE_PERFEVTSEL7 = 0x3c7
IA32_MSR_UNCORE_PMC0 = 0x3b0
IA32_MSR_UNCORE_PMC1 = 0x3b1
IA32_MSR_UNCORE_PMC2 = 0x3b2
IA32_MSR_UNCORE_PMC3 = 0x3b3
IA32_MSR_UNCORE_PMC4 = 0x3b4
IA32_MSR_UNCORE_PMC5 = 0x3b5
IA32_MSR_UNCORE_PMC6 = 0x3b6
IA32_MSR_UNCORE_PMC7 = 0x3b7

'''
event
'''
evtsel_disable = 0x0

evtsel_l1i_access = 0x430300
evtsel_l1i_miss = 0x430301
evtsel_inst_retires_any = 0x430000
evtsel_uop_ret_any = 0x430001

evtsel_replay_cycle = 0x43000c
evtsel_cpu_clk_unhalted = 0x430012
evtsel_fp_comp_ops_exe_x87 = 0x43001b
evtsel_fp_comp_ops_exe_sse_scalar_dp = 0x43001c

evtsel_fp_comp_ops_exe_sse_packed_sp = 0x43001d
evtsel_fp_comp_ops_exe_sse_fp_scalar_sp = 0x43001e
evtsel_fp_comp_ops_exe_sse_fp_scalar_dp = 0x43001f
evtsel_uops_issued_single_mul = 0x430026

evtsel_cpu_clk_core = 0x430082
evtsel_cpu_clk_bus = 0x430083
evtsel_branch_all = 0x430028
evtsel_rob_stall_cycle = 0x430100

evtsel_ucode_cycle = 0x430121
evtsel_l1d_miss = 0x430504
evtsel_load_retire = 0x430568
evtsel_store_retire = 0x430669

evtsel_bus_clk = 0x430d00
evetl_l2_total_miss = 0x430b16
evtsel_resource_stalls_any = 0x430410
evtsel_fill_quque_full = 0x43051d

infilename = "event_config.txt"
core_event_config_dic = {}
uncore_event_config_dic = {}
event_result_dic = {}
os.system("sudo modprobe msr")

'''
change scale
'''
# base = [0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F]
base = [str(x) for x in range(10)] + [chr(x) for x in range(ord('A'),ord('A')+6)]
def bin2dec(string_corepmcnum):
    return str(int(string_corepmcnum,2))
def hex2dec(string_corepmcnum):
    return str(int(string_corepmcnum.upper(),16))
def dec2bin(string_corepmcnum):
    corepmcnum = int(string_corepmcnum)
    mid = []
    while True:
        if corepmcnum == 0: break
        corepmcnum,rem = divmod(corepmcnum,2)
        mid.append(base[rem])
    return ''.join([str(x) for x in mid[::-1]])

def ExecCmdGetTernal(cmd):
    r = os.popen(cmd)
    text = r.read()
    r.close()
    return text

def RdmsrCmd(cpu, reg):
    if cpu == -1:
        cmd = "sudo rdmsr -a {}".format(reg)
    else:
        cmd = "sudo rdmsr -p {} {}".format(cpu,reg)
    result = ExecCmdGetTernal(cmd)
    return result

def WrmsrCmd(cpu,reg,hi,low):
    if cpu == -1:
        cmd = "sudo wrmsr -a {} {} {}".format(reg,hi,low)
    else:
        cmd = "sudo wrmsr -p {} {} {} {}".format(cpu,reg,hi,low)
    ExecCmdGetTernal(cmd)

def GetEventConfig():
    with open(infilename, "r") as file_read:
        while True:
            strline = file_read.readline()
            if not strline:
                break
            strtemp = strline.strip("\n").split(":")
            if strtemp[0]:
                if strtemp[0] == "core":
                    core_event_config_dic[strtemp[1]] = strtemp[2] 
                elif strtemp[0] == "uncore":
                    uncore_event_config_dic[strtemp[1]] = strtemp[2]
                else:
                    pass                                                                                                                                                                                                                                                                         

def ProcessNoFixedPMC():
    corepmcnum = 0
    corepmc0 = ""
    corepmc1 = ""
    corepmc2 = ""
    corepmc3 = ""
    for evkey, evvalue in core_event_config_dic.items():
        if corepmcnum % 4 == 0:
            corepmc0 = evkey
            WrmsrCmd(-1,IA32_MSR_PERFEVTSEL0,0x0,evvalue)
        elif corepmcnum % 4 == 1:
            corepmc1 = evkey
            WrmsrCmd(-1,IA32_MSR_PERFEVTSEL1,0x0,evvalue)
        elif corepmcnum % 4 == 2:
            corepmc2 = evkey
            WrmsrCmd(-1,IA32_MSR_PERFEVTSEL2,0x0,evvalue)
        elif corepmcnum % 4 == 3:
            corepmc3 = evkey
            WrmsrCmd(-1,IA32_MSR_PERFEVTSEL3,0x0,evvalue)
        else:
            pass
        corepmcnum = corepmcnum + 1
        if corepmcnum >= 4 and corepmcnum % 4 == 0:
            WrmsrCmd(-1,IA32_MSR_PERF_PMC0,0x0,0x0)
            WrmsrCmd(-1,IA32_MSR_PERF_PMC1,0x0,0x0)
            WrmsrCmd(-1,IA32_MSR_PERF_PMC2,0x0,0x0)
            WrmsrCmd(-1,IA32_MSR_PERF_PMC3,0x0,0x0)
            time.sleep(0.5)
            if corepmc0 != "":
                event_result_dic[corepmc0] = RdmsrCmd(-1, IA32_MSR_PERF_PMC0)
            if corepmc1 != "":   
                event_result_dic[corepmc1] = RdmsrCmd(-1, IA32_MSR_PERF_PMC1)
            if corepmc2 != "":
                event_result_dic[corepmc2] = RdmsrCmd(-1, IA32_MSR_PERF_PMC2)
            if corepmc3 != "":
                event_result_dic[corepmc3] = RdmsrCmd(-1, IA32_MSR_PERF_PMC3)
            corepmc0 = ""
            corepmc1 = ""
            corepmc2 = ""
            corepmc3 = ""
            WrmsrCmd(-1,IA32_MSR_PERFEVTSEL0,0x0,0x0)
            WrmsrCmd(-1,IA32_MSR_PERFEVTSEL1,0x0,0x0)
            WrmsrCmd(-1,IA32_MSR_PERFEVTSEL2,0x0,0x0)
            WrmsrCmd(-1,IA32_MSR_PERFEVTSEL3,0x0,0x0)
    

    
    uncorepmcnum = 0
    uncorepmc0 = ""
    uncorepmc1 = ""
    uncorepmc2 = ""
    uncorepmc3 = ""
    for evkey, evvalue in uncore_event_config_dic.items():
        if uncorepmcnum % 4 == 0:
            uncorepmc0 = evkey
            WrmsrCmd(-1,IA32_MSR_UNCORE_PERFEVTSEL0,0x0,evvalue)
        elif uncorepmcnum % 4 == 1:
            uncorepmc1 = evkey
            WrmsrCmd(-1,IA32_MSR_UNCORE_PERFEVTSEL1,0x0,evvalue)
        elif uncorepmcnum % 4 == 2:
            uncorepmc2 = evkey
            WrmsrCmd(-1,IA32_MSR_UNCORE_PERFEVTSEL2,0x0,evvalue)
        elif uncorepmcnum % 4 == 3:
            uncorepmc3 = evkey
            WrmsrCmd(-1,IA32_MSR_UNCORE_PERFEVTSEL3,0x0,evvalue)
        else:
            pass
        uncorepmcnum = uncorepmcnum + 1
        if uncorepmcnum >= 4 and uncorepmcnum % 4 == 0:
            WrmsrCmd(-1,IA32_MSR_UNCORE_PMC0,0x0,0x0)
            WrmsrCmd(-1,IA32_MSR_UNCORE_PMC1,0x0,0x0)
            WrmsrCmd(-1,IA32_MSR_UNCORE_PMC2,0x0,0x0)
            WrmsrCmd(-1,IA32_MSR_UNCORE_PMC3,0x0,0x0)
            time.sleep(0.5)
            if uncorepmc0 != "":
                event_result_dic[uncorepmc0] = RdmsrCmd(-1, IA32_MSR_UNCORE_PMC0)
            if uncorepmc1 != "":   
                event_result_dic[uncorepmc1] = RdmsrCmd(-1, IA32_MSR_UNCORE_PMC1)
            if uncorepmc2 != "":
                event_result_dic[uncorepmc2] = RdmsrCmd(-1, IA32_MSR_UNCORE_PMC2)
            if uncorepmc3 != "":
                event_result_dic[uncorepmc3] = RdmsrCmd(-1, IA32_MSR_UNCORE_PMC3)
            uncorepmc0 = ""
            uncorepmc1 = ""
            uncorepmc2 = ""
            uncorepmc3 = ""
            WrmsrCmd(-1,IA32_MSR_UNCORE_PERFEVTSEL0,0x0,0x0)
            WrmsrCmd(-1,IA32_MSR_UNCORE_PERFEVTSEL1,0x0,0x0)
            WrmsrCmd(-1,IA32_MSR_UNCORE_PERFEVTSEL2,0x0,0x0)
            WrmsrCmd(-1,IA32_MSR_UNCORE_PERFEVTSEL3,0x0,0x0)


def SaveResultToTxt(outfilename):
    with open(outfilename, 'a+') as file_write:
        for pmckey, pmcvalue in event_result_dic.items():
            pmclist = pmcvalue.strip("\n").split("\n")
            decvalue = 0
            for item in pmclist:
                decvalue = decvalue + int(hex2dec(item))
            print(pmclist)
            print(pmckey + ":" + str(decvalue))
            file_write.write(pmckey + ":" + str(decvalue) + "\n")


if __name__ == '__main__':
    GetEventConfig()
    ProcessNoFixedPMC()
    print(core_event_config_dic)
    print(uncore_event_config_dic)
    SaveResultToTxt("out.txt")
    #print(event_result_dic)

