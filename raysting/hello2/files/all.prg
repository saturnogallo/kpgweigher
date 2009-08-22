9i�?/*****************函数(一�?定义************************//
//TI_TIME(^1^,^2^)
//设置转向和电流变化后的停留时�?
<TI_TIME(^1^,^2^)>
[v,0,0,T_TIME,^1^]
[v,0,0,I_TIME,^2^]
><
<nav_range6()>
[n,0,0,:SENS:VOLT:DC:RANG 100\n]
><
<nav_range4()>
[n,0,0,:SENS:VOLT:DC:RANG 1\n]
><
<nav_init()>
[n,0,0,*RST\n*CLS\n:INIT:CONT ON\n:ABORT\n:SENS:FUNC 'VOLT:DC'\n]
[n,0,0,:SENS:VOLT:DC:RANG:AUTO ON\n:SENS:VOLT:DC:AVER:STAT OFF\n]
[n,0,0,:SENS:VOLT:DC:DIG 7\n:SYST:AZER:STAT ON\n:SENS:VOLT:DC:NPLC 1\n]
><
<nav_afltoff()>
[n,0,0,:SENS:VOLT:DC:AVER:STAT OFF\n]
><
<nav_remote()>
[n,0,0,\n]
><
<nav_close()>
[n,0,0,\n]
><
<nav_readonly()>
[n,0,0,\n]
><
<nav_aflton()>
[n,0,0,:SENS:VOLT:DC:AVER:COUN 10\n:SENS:VOLT:DC:AVER:TCON MOV\n:SENS:VOLT:DC:AVER:STAT ON\n]
><
<nav_slowmode()>
[n,"flt_speed@env",1,:SENS:VOLT:DC:NPLC 10\n]
[n,"flt_speed@env",0,:SENS:VOLT:DC:NPLC 1\n]
><
<nav_read()>
[n,0,0,:FETC?\n]
><
<nav_range5()>
[n,0,0,:SENS:VOLT:DC:RANG 10\n]
><
<nav_zeroon()>
[n,0,0,:SENS:VOLT:DC:REF:ACQ\n:SENS:VOLT:DC:REF:STAT ON\n]
><
<nav_dflton()>
[n,0,0,:SENS:VOLT:DC:AVER:COUN 10\n:SENS:VOLT:DC:AVER:TCON MOV\n:SENS:VOLT:DC:AVER:STAT ON\n]
><
<nav_zerooff()>
[n,0,0,:SENS:VOLT:DC:REF:STAT OFF\n]
><
<nav_range1()>
[n,0,0,:SENS:VOLT:DC:RANG:AUTO ON\n]
><
<nav_range3()>
[n,0,0,:SENS:VOLT:DC:RANG .1\n]
><
<nav_range2()>
[n,0,0,:SENS:VOLT:DC:RANG .01\n]
><
<nav_dfltoff()>
[n,0,0,:SENS:VOLT:DC:AVER:STAT OFF\n]
><

//SINGLE_VTEST(),结果保存在^1^
<SINGLE_VTEST(^1^)>
{nav_read()}
[t,0,0,^1^]
><

//双向测量电压值到V1,V2（转向停TK秒）
<DOUBLE_VTEST(^1^)>
{SINGLE_VTEST(A1)}
[s,0,0,OKTT]
[p,0,0,"T_TIME@env"]
{SINGLE_VTEST(A2)}
[c,0,0,f,^1^,"A1@env"+"A2@env"/2]
><

<TURNON_EXT()>
[s,0,0,OHX1]
><

<TURNOFF_EXT()>
[s,0,0,FHX1]
><

//0,1,2,3,4,5 = 1,10,100,1k,10k,100k ohm
<TURNOFF_STD()>
[s,"prb_ref@env",0,FH1]
[s,"prb_ref@env",1,FH2]
[s,"prb_ref@env",2,FH3]
[s,"prb_ref@env",3,FH4]
[s,"prb_ref@env",4,FH5]
[s,"prb_ref@env",5,FH6]
><

//0,1,2,3,4,5 = 1,10,100,1k,10k,100k ohm
<TURNON_STD()>
//[m,0,0,"prb_ref@env"111]
[s,"prb_ref@env",0,OH1,FH2,FH3,FH4,FH5,FH6,OKN1]
[s,"prb_ref@env",1,OH2,FH1,FH3,FH4,FH5,FH6,OKN2]
[s,"prb_ref@env",2,OH3,FH2,FH1,FH4,FH5,FH6,OKN3]
[s,"prb_ref@env",3,OH4,FH2,FH3,FH1,FH5,FH6,OKN4]
[s,"prb_ref@env",4,OH5,FH2,FH3,FH4,FH1,FH6,OKN5]
[s,"prb_ref@env",5,OH6,FH2,FH3,FH4,FH5,FH1,OKN5]
><

//0,1,2,3,4,5 = 1,10,100,1k,10k,100k ohm
<GET_RSTD()>
[v,"prb_ref@env",0,RSTD,"d_1@env"]
[v,"prb_ref@env",1,RSTD,"d_10@env"]
[v,"prb_ref@env",2,RSTD,"d_100@env"]
[v,"prb_ref@env",3,RSTD,"d_1k@env"]
[v,"prb_ref@env",4,RSTD,"d_10k@env"]
[v,"prb_ref@env",5,RSTD,"d_100k@env"]
[v,"prb_useext@env",1,RSTD,"prb_extref@env"]
><

//0,1,2,3,4,5,6 = 10mA, 3mA, 1mA, 0.1mA, 0.01mA, 0.005mA, 0.002mA
<TURNTO_CURRENT()>
[s,"prb_curr@env",0,OKI1]
[s,"prb_curr@env",1,OKI2]
[s,"prb_curr@env",2,OKI3]
[s,"prb_curr@env",3,OKI4]
[s,"prb_curr@env",4,OKI5]
[s,"prb_curr@env",5,OKI6]
[s,"prb_curr@env",6,OKI7]
[s,"prb_sqrt@env",0,FSQT]
[s,"prb_sqrt@env",1,OSQT]
><

//RSTD_SIN_A(^1^,^2^,^3^,^4^) 
//标准电阻测量方法1测量,�?>�?>�?计算
//^1^,^2^,^3^ 分别�?KI,KN,HP),^4^保存结果
<RSTD_SIN_A(^1^)>
{TURNON_STD()}
{TURNTO_CURRENT()}
{TURNOFF_EXT()}
[p,0,0,"I_TIME@env"]
{SINGLE_VTEST(VSTD1)}

{TURNON_EXT()}
{TURNOFF_STD()}
[p,0,0,"I_TIME@env"]
{SINGLE_VTEST(VEXT1)}

{TURNON_STD()}
{TURNOFF_EXT()}
[p,0,0,"I_TIME@env"]
{SINGLE_VTEST(VSTD2)}

[s,0,0,OKTT]

[p,0,0,"I_TIME@env"]
{SINGLE_VTEST(VSTD3)}

{TURNON_EXT()}
{TURNOFF_STD()}
[p,0,0,"I_TIME@env"]
{SINGLE_VTEST(VEXT2)}

{TURNON_STD()}
{TURNOFF_EXT()}
[p,0,0,"I_TIME@env"]
{SINGLE_VTEST(VSTD4)}

[s,0,0,OKTT]

{GET_RSTD()}
[c,0,0,f,^1^,"VSTD1@env"+"VSTD2@env"+"VSTD3@env"+"VSTD4@env"/4]
[c,0,0,f,VEXT,"VEXT1@env"+"VEXT2@env"/2]
[c,0,0,f,^1^,"RSTD@env"*"VEXT@env"/"^1^@env"]
><

<SINGLE_VAR()>
[c,0,0,f,READING^1^_VAR,"READING^1^@env"-"VAR_RESULT_AVG@env"]
[c,0,0,f,READING^1^_VAR,"READING^1^_VAR@env"*"READING^1^_VAR@env"]
><

<LOOPVAR()>
{RSTD_SIN_A(READING9)}
{SHIFT_READING()}
//check the variance
[c,0,0,f,VAR_RESULT_AVG,"READING0@env"+"READING1@env"+"READING2@env"+"READING3@env"+"READING4@env"+"READING5@env"+"READING6@env"+"READING7@env"+"READING8@env"/9]
{SINGLE_VAR(0)}
{SINGLE_VAR(1)}
{SINGLE_VAR(2)}
{SINGLE_VAR(3)}
{SINGLE_VAR(4)}
{SINGLE_VAR(5)}
{SINGLE_VAR(6)}
{SINGLE_VAR(7)}
{SINGLE_VAR(8)}

[c,0,0,f,VAR_RESULT_AVG,"READING0_VAR@env"+"READING1_VAR@env"+"READING2_VAR@env"+"READING3_VAR@env"+"READING4_VAR@env"+"READING5_VAR@env"+"READING6_VAR@env"+"READING7_VAR@env"+"READING8_VAR@env"+"READING9_VAR@env"/9]
[c,0,0,i,VAR_CMP,"VAR_RESULT@env"<0.01]
><

<SHIFT_READING()>
//shift the reading
[c,0,0,f,READING0,"READING1@env"]
[c,0,0,f,READING1,"READING2@env"]
[c,0,0,f,READING2,"READING3@env"]
[c,0,0,f,READING3,"READING4@env"]
[c,0,0,f,READING4,"READING5@env"]
[c,0,0,f,READING5,"READING6@env"]
[c,0,0,f,READING6,"READING7@env"]
[c,0,0,f,READING7,"READING8@env"]
[c,0,0,f,READING8,"READING9@env"]
><

<READ_UPDATE(^1^)>
{TURNON_EXT()}
{TURNOFF_STD()}
[p,0,0,"T_TIME@env"]
{SINGLE_VTEST(VEXT)}
{GET_RSTD()}
[c,0,0,f,^1^,"RSTD@env"*"VEXT@env"/"VSTD2@env"]
><

<mytest()>
[r,0,0,nothing]
[m,0,0,reset]
{test(KI1)}
{test(KI2)}
{test(KI3)}
{test(KI4)}
{test(KI5)}
{test(KI6)}
{test(KI7)}
{test(KTT)}
{test(SQT)}
{test(HX1)}
{test(H1)}
{test(H2)}
{test(H3)}
{test(H4)}
{test(H5)}
{test(H6)}
{test(KN1)}
{test(KN2)}
{test(KN3)}
{test(KN4)}
{test(KN5)}

[r,0,0,nothing]
{TURNON_STD()}
{TURNTO_CURRENT()}
{TURNOFF_EXT()}
[m,0,0,std_cur_on_ext_off]
[p,0,0,"I_TIME@env"]

{TURNON_EXT()}
{TURNOFF_STD()}
[m,0,0,ext_on_std_off]

{TURNON_STD()}
{TURNOFF_EXT()}
[m,0,0,std_on_ext_off]
><

<test(^1^)>
[s,0,0,O^1^]
[m,0,0,O^1^]
[s,0,0,F^1^]
[m,0,0,F^1^]
[s,0,0,O^1^]
[m,0,0,O^1^]
[s,0,0,F^1^]
[m,0,0,F^1^]
><

//result should be put in "OUTPUT@env"
<resistance()>
[v,"flt_speed@env",0,T_TIME,2]
[v,"flt_speed@env",0,I_TIME,2]
[v,"flt_speed@env",1,T_TIME,8]
[v,"flt_speed@env",1,I_TIME,8]
{mytest()}
[j,"PTR@env",1,NO_INIT]
[r,0,0,nothing]
{nav_init()}
{nav_slowmode()}
{nav_aflton()}
//fill the first 8 reading
//[v,0,0,IREAD,0]
//[i,0,0,END_IREAD]
//{RSTD_SIN_A(READING"IREAD@env")}
//[c,0,0,i,IREAD,"IREAD@env"+1]
//[j,"IREAD@env",8,QUIT_IREAD]
//[o,0,0,END_IREAD]
//[QUIT_IREAD]


//[i,0,0,END_LOOP]
//{LOOPVAR()}
//[j,"VAR_CMP@env",1,EXIT_LOOP]
//[o,"VAR_CMP@env",0,END_LOOP]
//[EXIT_LOOP]
[j,prb_ref@env,last_ref@env,NORST]
[r,0,0,nothing]
[v,0,0,last_ref,prb_ref@env]
[NORST]
[c,0,0,i,PTR,1]
[NO_INIT]
//{READ_UPDATE(OUTPUT)}
{RSTD_SIN_A(OUTPUT)}
><
