//TI_TIME(^1^,^2^)
//璁剧疆杞悜鍜岀數娴佸彉鍖栧悗鐨勫仠鐣欐椂闂?
<TI_TIME(^1^,^2^)>
[v,0,0,T_TIME,^1^]
[v,0,0,I_TIME,^2^]
><
<nav_30v()>
[n,0,0,%01;12;02\r]
[p,0,0,2]
><
<nav_1v()>
[n,0,0,%01;12;01\r]
[p,0,0,2]
[v,0,0,VRANGE,1v]
><
<nav_120mv()>
[n,0,0,%01;12;00\r]
[p,0,0,2]
[v,0,0,VRANGE,120mv]
><
<nav_init()>
[n,0,0,%01;00\r]
><
<nav_afltoff()>
[n,0,0,%01;26\r]
[p,0,0,1]
><
<nav_remote()>
><
<nav_close()>
><
<nav_readonly()>
><
<nav_aflton()>
[n,0,0,%01;27\r]
[p,0,0,1]
><
<nav_slowmode()>
[n,0,0,%01;27\r]
[p,0,0,1]
><
<nav_read()>
[n,0,0,%01;01\r]
><
<nav_zeroon()>
[n,0,0,%01:06\r]
><
<nav_dflton()>
><
<nav_zerooff()>
><
<nav_dfltoff()>
><

//SINGLE_VTEST(),缁撴灉淇濆瓨鍦╚1^
<SINGLE_VTEST(^1^)>
{nav_read()}
[t,0,0,^1^]

//[c,"VRANGE@prg",120mv,f,^1^,"^1^@prg"/1000.0]
><

//鍙屽悜娴嬮噺鐢靛帇鍊煎埌V1,V2锛堣浆鍚戝仠TK绉掞級
<DOUBLE_VTEST(^1^)>
{SINGLE_VTEST(A1)}
[s,0,0,OKTT]
[p,0,0,"T_TIME@prg"]
{SINGLE_VTEST(A2)}
[c,0,0,f,^1^,"A1@prg"+"A2@prg"/2]
><

<TURNON_EXT()>
[s,"rx@prg",1,OEXT,OHX1]
[s,"rx@prg",0,FEXT,OHX1]
><
<USE_EXT()>
[s,"rx@prg",0,OEXT]
[s,"rx@prg",1,FEXT]
[j,"rx@prg",1,NOEXT]
{TURNOFF_STD()}
[NOEXT]
><
<TURNOFF_EXT()>
[s,"rx@prg",1,FHX1,FEXT]
[s,"rx@prg",0,FHX1,OEXT]
><

//0,1,2,3,4,5 = 1,10,100,1k,10k,100k ohm
<TURNOFF_STD()>
[s,"prb_ref@prg",0,FH1]
[s,"prb_ref@prg",1,FH2]
[s,"prb_ref@prg",2,FH3]
[s,"prb_ref@prg",3,FH4]
[s,"prb_ref@prg",4,FH5]
[s,"prb_ref@prg",5,FH6]
><

//0,1,2,3,4,5 = 1,10,100,1k,10k,100k ohm
<TURNON_STD()>
[w,0,0,rx]
[j,"rx@prg",1,HASSTD]
{TURNOFF_STD()}
[HASSTD]
[j,"rx@prg",0,NOSTD]
[s,"prb_ref@prg",0,OH1,FH2,FH3,FH4,FH5,FH6]
[s,"prb_ref@prg",1,OH2,FH1,FH3,FH4,FH5,FH6]
[s,"prb_ref@prg",2,OH3,FH2,FH1,FH4,FH5,FH6]
[s,"prb_ref@prg",3,OH4,FH2,FH3,FH1,FH5,FH6]
[s,"prb_ref@prg",4,OH5,FH2,FH3,FH4,FH1,FH6]
[s,"prb_ref@prg",5,OH6,FH2,FH3,FH4,FH5,FH1]
[NOSTD]
{USE_EXT()}
><
<GETDEF_RSTD()>
[v,"prb_ref@prg",0,DEF_RSTD,"u_1@prg"]
[v,"prb_ref@prg",1,DEF_RSTD,"u_10@prg"]
[v,"prb_ref@prg",2,DEF_RSTD,"u_100@prg"]
[v,"prb_ref@prg",3,DEF_RSTD,"u_1k@prg"]
[v,"prb_ref@prg",4,DEF_RSTD,"u_10k@prg"]
[v,"prb_ref@prg",5,DEF_RSTD,"u_100k@prg"]
><

//0,1,2,3,4,5 = 1,10,100,1k,10k,100k ohm
<GET_RSTD()>
[j,"rx@prg",0,USEEXT]
[v,"prb_ref@prg",0,RSTD,"d_1@env"]
[v,"prb_ref@prg",1,RSTD,"d_10@env"]
[v,"prb_ref@prg",2,RSTD,"d_100@env"]
[v,"prb_ref@prg",3,RSTD,"d_1k@env"]
[v,"prb_ref@prg",4,RSTD,"d_10k@env"]
[v,"prb_ref@prg",5,RSTD,"d_100k@env"]
[j,0,0,RETSTD]
[USEEXT]
[v,"prb_ref@prg",0,RSTD,"u_1@prg"]
[v,"prb_ref@prg",1,RSTD,"u_10@prg"]
[v,"prb_ref@prg",2,RSTD,"u_100@prg"]
[v,"prb_ref@prg",3,RSTD,"u_1k@prg"]
[v,"prb_ref@prg",4,RSTD,"u_10k@prg"]
[v,"prb_ref@prg",5,RSTD,"u_100k@prg"]
[RETSTD]
><

//0,1,2,3,4,5,6 = 10mA, 3mA, 1mA, 0.1mA, 0.01mA, 0.005mA, 0.002mA
<TURNTO_CURRENT()>
[s,"prb_curr@prg",0,FKI2,FKI3,FKI4,FKI5,FKI6,FKI7,OKI1]
[s,"prb_curr@prg",1,FKI1,FKI3,FKI4,FKI5,FKI6,FKI7,OKI2]
[s,"prb_curr@prg",2,FKI2,FKI1,FKI4,FKI5,FKI6,FKI7,OKI3]
[s,"prb_curr@prg",3,FKI2,FKI3,FKI1,FKI5,FKI6,FKI7,OKI4]
[s,"prb_curr@prg",4,FKI2,FKI3,FKI4,FKI1,FKI6,FKI7,OKI5]
[s,"prb_curr@prg",5,FKI2,FKI3,FKI4,FKI5,FKI1,FKI7,OKI6]
[s,"prb_curr@prg",6,FKI2,FKI3,FKI4,FKI5,FKI6,FKI1,OKI7]
[s,"prb_sqrt@prg",0,FSQT]
[s,"prb_sqrt@prg",1,OSQT]
><
<SHIFT_CHAN()>
[s,"chid@prg",2,OCH1,FCH2]
[s,"chid@prg",1,OCH2,FCH1]
[v,"chid@prg",2,lastch,2]
[v,"chid@prg",1,lastch,1]
><

//AGAIN_RSTD_SIN_A(^1^,^2^,^3^,^4^) 
//鏍囧噯鐢甸樆娴嬮噺鏂规硶1娴嬮噺,鏍?>琚?>鏍?璁＄畻
//^1^,^2^,^3^ 鍒嗗埆涓?KI,KN,HP),^4^淇濆瓨缁撴灉
<AGAIN_RSTD_SIN_A(^1^)>
{SHIFT_CHAN()}
{TURNON_STD()}
//{TURNTO_CURRENT()}
{TURNOFF_EXT()}
[p,0,0,"T_TIME@prg"]
{SINGLE_VTEST(VSTD1)}

{TURNON_EXT()}
//{TURNOFF_STD()}
[p,0,0,"T_TIME@prg"]
{SINGLE_VTEST(VEXT1)}

{TURNOFF_EXT()}
{TURNON_STD()}
[p,0,0,"T_TIME@prg"]
{SINGLE_VTEST(VSTD2)}

[s,0,0,OKTT]

{GET_RSTD()}
[c,0,0,f,^1^,"VSTD1@prg"+"VSTD2@prg"+"VSTD3@prg"+"VSTD4@prg"/4]
[c,0,0,f,VEXT,"VEXT1@prg"+"VEXT2@prg"/2]
[c,0,0,f,^1^,"RSTD@prg"*"VEXT@prg"/"^1^@prg"]
//copy data to VSTD3,VSTD4,VEXT2
[c,0,0,f,VSTD3,"VSTD1@prg"*1]
[c,0,0,f,VSTD4,"VSTD2@prg"*1]
[c,0,0,f,VEXT2,"VEXT1@prg"*1]
><

<SINGLE_VAR()>
[c,0,0,f,READING^1^_VAR,"READING^1^@prg"-"VAR_RESULT_AVG@prg"]
[c,0,0,f,READING^1^_VAR,"READING^1^_VAR@prg"*"READING^1^_VAR@prg"]
><


//RSTD_SIN_A(^1^,^2^,^3^,^4^) 
//鏍囧噯鐢甸樆娴嬮噺鏂规硶1娴嬮噺,鏍?>琚?>鏍?璁＄畻
//^1^,^2^,^3^ 鍒嗗埆涓?KI,KN,HP),^4^淇濆瓨缁撴灉
<RSTD_SIN_A(^1^)>
{SHIFT_CHAN()}
{TURNON_STD()}
[s,0,0,OKTT]
{TURNTO_CURRENT()}
{TURNOFF_EXT()}
[p,0,0,"T_TIME@prg"]
{SINGLE_VTEST(VSTD1)}

{TURNON_EXT()}
//{TURNOFF_STD()}
[p,0,0,"T_TIME@prg"]
{SINGLE_VTEST(VEXT1)}

//[c,"VRANGE@prg",1v,i,GT120MV,"VEXT1@prg">0.12]
//[c,"VRANGE@prg",120mv,i,GT120MV,"VEXT1@prg">120]
//[m,0,0,"VRANGE@prg"+"VEXT1@prg"+"GT120MV@prg"]
//[j,1,"GT120MV@prg",GTRANGE]
//[j,,"GT120MV@prg",ENDRANGE]
//[j,0,0,LTRANGE]

//[GTRANGE]
//[j,1v,"VRANGE@prg",ENDRANGE]
//{nav_1v()}
//[p,0,0,"T_TIME@prg"]
//{SINGLE_VTEST(VEXT1)}

//[j,0,0,ENDRANGE]
//[LTRANGE]
//[j,120mv,"VRANGE@prg",ENDRANGE]
//[j,"flt_speed@prg",0,ENDRANGE]
//{nav_120mv()}
//[p,0,0,"T_TIME@prg"]
//{SINGLE_VTEST(VEXT1)}
//[ENDRANGE]

{TURNOFF_EXT()}
{TURNON_STD()}
[p,0,0,"T_TIME@prg"]
{SINGLE_VTEST(VSTD2)}

[s,0,0,OKTT]

[p,0,0,"I_TIME@prg"]
{SINGLE_VTEST(VSTD3)}

{TURNON_EXT()}
//{TURNOFF_STD()}
[p,0,0,"T_TIME@prg"]
{SINGLE_VTEST(VEXT2)}

{TURNOFF_EXT()}
{TURNON_STD()}
[p,0,0,"T_TIME@prg"]
{SINGLE_VTEST(VSTD4)}

[s,0,0,OKTT]

{GET_RSTD()}
[c,0,0,f,^1^,"VSTD1@prg"+"VSTD2@prg"+"VSTD3@prg"+"VSTD4@prg"/4]
[c,0,0,f,VEXT,"VEXT1@prg"+"VEXT2@prg"/2]
[c,0,0,f,^1^,"RSTD@prg"*"VEXT@prg"/"^1^@prg"]
><

<SINGLE_VAR()>
[c,0,0,f,READING^1^_VAR,"READING^1^@prg"-"VAR_RESULT_AVG@prg"]
[c,0,0,f,READING^1^_VAR,"READING^1^_VAR@prg"*"READING^1^_VAR@prg"]
><

<LOOPVAR()>
{RSTD_SIN_A(READING9)}
{SHIFT_READING()}
//check the variance
[c,0,0,f,VAR_RESULT_AVG,"READING0@prg"+"READING1@prg"+"READING2@prg"+"READING3@prg"+"READING4@prg"+"READING5@prg"+"READING6@prg"+"READING7@prg"+"READING8@prg"/9]
{SINGLE_VAR(0)}
{SINGLE_VAR(1)}
{SINGLE_VAR(2)}
{SINGLE_VAR(3)}
{SINGLE_VAR(4)}
{SINGLE_VAR(5)}
{SINGLE_VAR(6)}
{SINGLE_VAR(7)}
{SINGLE_VAR(8)}

[c,0,0,f,VAR_RESULT_AVG,"READING0_VAR@prg"+"READING1_VAR@prg"+"READING2_VAR@prg"+"READING3_VAR@prg"+"READING4_VAR@prg"+"READING5_VAR@prg"+"READING6_VAR@prg"+"READING7_VAR@prg"+"READING8_VAR@prg"+"READING9_VAR@prg"/9]
[c,0,0,i,VAR_CMP,"VAR_RESULT@prg"<0.01]
><

<SHIFT_READING()>
//shift the reading
[c,0,0,f,READING0,"READING1@prg"]
[c,0,0,f,READING1,"READING2@prg"]
[c,0,0,f,READING2,"READING3@prg"]
[c,0,0,f,READING3,"READING4@prg"]
[c,0,0,f,READING4,"READING5@prg"]
[c,0,0,f,READING5,"READING6@prg"]
[c,0,0,f,READING6,"READING7@prg"]
[c,0,0,f,READING7,"READING8@prg"]
[c,0,0,f,READING8,"READING9@prg"]
><
<setrange()>
[j,"flt_speed@prg",0,SETFAST]
[j,"flt_speed@prg",1,SETSLOW]
[j,0,0,ENDSET]
[SETSLOW]
{nav_120mv()}
[j,0,0,ENDSET]
[SETFAST]
{nav_1v()}
[j,0,0,ENDSET]
[ENDSET]
><
//result should be put in "OUTPUT@prg"
<bore_test()>
[v,0,0,status,正开始通道"chid@prg"测量]
[v,"flt_speed@prg",0,T_TIME,4]
[v,"flt_speed@prg",0,I_TIME,4]
[v,"flt_speed@prg",1,T_TIME,8]
[v,"flt_speed@prg",1,I_TIME,8]
[j,"PTR@prg",0,DOINIT]
[j,0,0,INITDONE]
[DOINIT]
{nav_slowmode()}
{nav_aflton()}
{setrange()}
[INITDONE]

[j,prb_ref@prg,last_ref@prg,NORST]
[r,0,0,nothing]
{nav_init()}
{nav_slowmode()}
{nav_aflton()}
{setrange()}
[v,0,0,last_ref,prb_ref@prg]
[NORST]
[c,0,0,i,PTR,1]
{RSTD_SIN_A(OUTPUT)}
[p,0,0,1]
><

<thmo_test()>
[j,"PTR@prg",0,DOTHMOINIT]
[j,0,0,INITTHMODONE]
[DOTHMOINIT]
{nav_120mv()}
[INITTHMODONE]

{SHIFT_CHAN()}
[p,0,0,1]
{nav_read()}
[t,0,0,OUTPUT]
[p,0,0,1]
><
<bore_again()>
[v,0,0,status,切换到通道"chid@prg"测量]
[v,"flt_speed@prg",0,T_TIME,8]
[v,"flt_speed@prg",0,I_TIME,8]
[v,"flt_speed@prg",1,T_TIME,16]
[v,"flt_speed@prg",1,I_TIME,16]
[j,"lastch@prg","chid@prg",JUSTAGAIN]
{RSTD_SIN_A(OUTPUT)}
[j,0,0,AGAIN_END]
[JUSTAGAIN]
{AGAIN_RSTD_SIN_A(OUTPUT)}
[AGAIN_END]
><

//internal calibration ^1^ return variable name
<syscaliball()>
[v,0,0,T_TIME,8]
[v,0,0,I_TIME,8]
//init
[j,"PTR^1^@prg",1,NO_INIT]
[r,0,0,nothing]
{nav_init()}
{nav_slowmode()}
{nav_aflton()}
{nav_1v()}
[NORST]
[v,0,0,PTR^1^,1]
[NO_INIT]
[s,1,1,FCH1,OCH2]

{TURNON_STD()}
{TURNTO_CURRENT()}
{TURNOFF_EXT()}
[p,0,0,"T_TIME@prg"]
{SINGLE_VTEST(VSTD1)}

{TURNON_EXT()}
//{TURNOFF_STD()}
[p,0,0,"T_TIME@prg"]
{SINGLE_VTEST(VEXT1)}

{TURNOFF_EXT()}
{TURNON_STD()}
[p,0,0,"T_TIME@prg"]
{SINGLE_VTEST(VSTD2)}

[s,0,0,OKTT]

[p,0,0,"I_TIME@prg"]
{SINGLE_VTEST(VSTD3)}

{TURNON_EXT()}
//{TURNOFF_STD()}
[p,0,0,"T_TIME@prg"]
{SINGLE_VTEST(VEXT2)}

{TURNOFF_EXT()}
{TURNON_STD()}
[p,0,0,"T_TIME@prg"]
{SINGLE_VTEST(VSTD4)}

[s,0,0,OKTT]

{GET_RSTD()}
{GETDEF_RSTD()}
[c,0,0,f,^1^,"VSTD1@prg"+"VSTD2@prg"+"VSTD3@prg"+"VSTD4@prg"/4]
[c,0,0,f,VEXT,"VEXT1@prg"+"VEXT2@prg"/2]
[c,0,0,f,^1^,"DEF_RSTD@prg"*"^1^@prg"/"VEXT@prg"]
><

<syscalib1()>
[v,0,0,prb_ref,0]
[v,0,0,status,开始校准1欧姆内标准]
{syscaliball(SYSRES1)}
><
<syscalib10()>
[v,0,0,prb_ref,1]
[v,0,0,status,开始校准10欧姆内标准]
{syscaliball(SYSRES10)}
><
<syscalib100()>
[v,0,0,prb_ref,2]
[v,0,0,status,开始校准100欧姆内标准]
{syscaliball(SYSRES100)}
><
<syscalib1k()>
[v,0,0,prb_ref,3]
[v,0,0,status,开始校准1K欧姆内标准]
{syscaliball(SYSRES1K)}
><
<syscalib10k()>
[v,0,0,prb_ref,4]
[v,0,0,status,开始校准10K欧姆内标准]
{syscaliball(SYSRES10K)}
><


