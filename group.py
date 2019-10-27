#!/usr/bin/python

import os
import sys

core_event_config_dic = {}
uncore_event_config_dic = {}

with open("event_config.txt", "r") as file_read:
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
#print(core_event_config_dic)
#print(uncore_event_config_dic)

corelist = []
uncorelist = []

corekeylist = core_event_config_dic.keys()
for i in range(0,len(corekeylist),4):
    #print(corekeylist[i:i+4])
    corelist.append(corekeylist[i:i+4])
uncorekeylist = uncore_event_config_dic.keys()
for i in range(0,len(uncorekeylist),4):
    #print(uncorekeylist[i:i+4])
    uncorelist.append(uncorekeylist[i:i+4])
#print(len(corelist))
#print(len(uncorelist))
maxlen = 0
if len(corelist) >= len(uncorelist):
    maxlen = len(corelist)
else:
    maxlen = len(uncorelist)

coreconfigsel0 = ""
coreconfigsel1 = ""
coreconfigsel2 = ""
coreconfigsel3 = ""

uncoreconfigsel0 = ""
uncoreconfigsel1 = ""
uncoreconfigsel2 = ""
uncoreconfigsel3 = ""

for i in range(0,maxlen):
    print("i is %d", i)

    try:
        if uncorelist[i]:
            #print(uncorelist[i])
            
            try:
                if uncorelist[i][0]:
                    uncoreconfigsel0 = uncorelist[i][0]
            except:
                uncoreconfigsel0 = ""
                pass
            
            try:
                if uncorelist[i][1]:
                    uncoreconfigsel1 = uncorelist[i][1]
            except:
                uncoreconfigsel1 = ""
                pass

            try:
                if uncorelist[i][2]:
                    uncoreconfigsel2 = uncorelist[i][2]
            except:
                uncoreconfigsel2 = ""
                pass

            try:
                if uncorelist[i][3]:
                    uncoreconfigsel3 = uncorelist[i][3]
            except:
                uncoreconfigsel3 = ""
                pass

    except:
        uncoreconfigsel0 = ""
        uncoreconfigsel1 = ""
        uncoreconfigsel2 = ""
        uncoreconfigsel3 = ""
        pass

    try:
        if corelist[i]:
            #print(corelist[i])
            
            try:
                if corelist[i][0]:
                    coreconfigsel0 = corelist[i][0]
            except:
                    coreconfigsel0 = ""
                    pass

            try:
                if corelist[i][1]:
                    coreconfigsel1 = corelist[i][1]
            except:
                    coreconfigsel1 = ""
                    pass          

            try:
                if corelist[i][2]:
                    coreconfigsel2 = corelist[i][2]
            except:
                    coreconfigsel2 = ""
                    pass          

            try:
                if corelist[i][3]:
                    coreconfigsel3 = corelist[i][3]
            except:
                    coreconfigsel3 = ""
                    pass          

    except:
        coreconfigsel0 = ""
        coreconfigsel1 = ""
        coreconfigsel2 = ""
        coreconfigsel3 = ""
        pass
    
    if coreconfigsel0 != "":
        print(coreconfigsel0 + ":" + core_event_config_dic[coreconfigsel0])
    if coreconfigsel1 != "":
        print(coreconfigsel1+ ":" + core_event_config_dic[coreconfigsel1])
    if coreconfigsel2 != "":
        print(coreconfigsel2+ ":" + core_event_config_dic[coreconfigsel2])
    if coreconfigsel3 != "":
        print(coreconfigsel3+ ":" + core_event_config_dic[coreconfigsel3])
    if uncoreconfigsel0 != "":
        print(uncoreconfigsel0+ ":"+ uncore_event_config_dic[uncoreconfigsel0])
    if uncoreconfigsel1 != "":
        print(uncoreconfigsel1+ ":"+ uncore_event_config_dic[uncoreconfigsel1])
    if uncoreconfigsel2 != "":
        print(uncoreconfigsel2+ ":"+ uncore_event_config_dic[uncoreconfigsel2])
    if uncoreconfigsel3 != "":
        print(uncoreconfigsel3+ ":"+ uncore_event_config_dic[uncoreconfigsel3])

    

'''
lenmax = 0
core_len = len(core_event_config_dic)
uncore_len = len(uncore_event_config_dic)
if core_len >= uncore_len:
    lenmax = core_len
else:
    lenmax = uncore_len
while len(core_event_config_dic) > 0:
'''   

'''
uncore_event_config_dic.popitem()
print(uncore_event_config_dic)
uncore_event_config_dic.popitem()
print(uncore_event_config_dic)
try:
    uncore_event_config_dic.popitem()
except:
    print("exception")
    print(len(uncore_event_config_dic))
'''