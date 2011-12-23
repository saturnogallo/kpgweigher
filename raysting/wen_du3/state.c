#include <mega64.h>
#include "lcd.h"
#include "key.h"
#include "utili.h"
#include "window.h"
#include "utili.h"
#include "scanner.h"

extern uchar nextwin;                                         //draw_label(&PRE##chvalue,SW_NORMAL);
#define REFRESH_TABLES1(PRE)	sprintf(strbuf,"%i",curr_pos);\
                		if(new_page == 1){\
		                	for(i = min_option; i <= max_option; i++){\
				                j = ((curr_pos-1)/max_option)*max_option + i;
				                
#define REFRESH_TABLES2(PRE)	        draw_label(&(PRE##items[i-1]),SW_NORMAL);\
				        if((curr_pos-1) % max_option == (i-1))\
					        draw_label(&(PRE##items[i-1]),SW_REVERSE);\
        			        }\
                                }else{\
	                		for(i = min_option; i <= max_option; i++) {\
        		        		if((last_pos-1) % max_option == (i-1)) 	draw_label(&(PRE##items[i-1]),SW_REVERSE);\
				                if((curr_pos-1) % max_option == (i-1))	draw_label(&(PRE##items[i-1]),SW_REVERSE);\
			                }\
		                }	

#define INC_TABLE	last_pos = curr_pos;	curr_pos++;\
			if(curr_pos > max_index) curr_pos = min_index;\
			if(last_pos == curr_pos)	return;\
			if(curr_pos % max_option == 1)  {new_page = 1;}else{new_page = 0;}

#define DEC_TABLE	last_pos = curr_pos;   curr_pos--;\
			if(curr_pos < min_index) curr_pos = max_index;\
			if(last_pos == curr_pos)  return;\
			if(curr_pos % max_option == 0)  {new_page = 1;}else{new_page = 0;}

#define KEY_TABLE	if((msg > KEY_NUM0) && (uchar)((msg-KEY_NUM0)*max_option) <= max_index)\
					{new_page = 1; curr_pos = (uchar)((msg-KEY_NUM1)*max_option)+1;}
					
#define INIT_OPTIONS(PRE)	for(i = min_option;i <= max_option; i++) {\
                                        draw_label(&PRE##options[i-1], SW_NORMAL);\
			        }	
#define REFRESH_OPTIONS(PRE)	if(last_sel == curr_sel)\
                                        return;\
        			for(i = min_option;i <= max_option; i++){\
	        			if(last_sel == i)       draw_label(&PRE##options[i-1], SW_REVERSE);\
        				if(curr_sel == i)	draw_label(&PRE##options[i-1], SW_REVERSE);\
        			}

#define INC_OPTION	last_sel = curr_sel;\
			curr_sel++;\
			if(curr_sel > max_option)\
				curr_sel = min_option;
				
#define DEC_OPTION	last_sel = curr_sel;\
			curr_sel--;\
			if(curr_sel < min_option)\
				curr_sel = max_option;

extern int   curr_ch;	//index of current channel in configuation   menu
extern int   curr_prb;	//index of current probe selection
extern int   curr_dispch; //start of current channel for display in main menu

void State_Init() {
	display_buttons(KEY_BTN1,sysdata.prbmode);
	display_buttons(KEY_BTN2,sysdata.kttmode);
	display_buttons(KEY_BTN3,0); //probe type
	display_buttons(KEY_BTN4,0); //zero
}
char* getprbtype(uchar);
LABEL flash pgmain_chs[] = {
		{LBL_HZ6X8,3, 3,5,strbuf},
		{LBL_HZ6X8,3,16,5,strbuf},
		{LBL_HZ6X8,3,29,5,strbuf},
		{LBL_HZ6X8,3,42,5,strbuf}
	};
LABEL flash pgmain_values[] = {
		{LBL_HZ6X8,40,3,10,strbuf},
		{LBL_HZ6X8,40,16,10,strbuf},
		{LBL_HZ6X8,40,29,10,strbuf},
		{LBL_HZ6X8,40,42,10,strbuf}
	};
LABEL flash pgmain_temps[] = {
		{LBL_HZ6X8,140,3, 10,strbuf},
		{LBL_HZ6X8,140,16,10,strbuf},
		{LBL_HZ6X8,140,29,10,strbuf},
		{LBL_HZ6X8,140,42,10,strbuf}
};
/*      Main display      */                          

void pgmain_handler(uchar msg) {                                      
        int max_option = 4;
        uchar i,old_ch,isinit;	
                uchar *cptr;
	//LABEL flash usage = {LBL_HZ6X8,210,60,5,"usage"};
                             
	isinit = 0;
        //Menu         
        if(msg == KEY_CE) {
                nextwin = PG_BOOTTYPE;
                return;
        }
	if(msg == KEY_TAB) {
		if(IS_THERM_MODE)
			nextwin = PG_TCONFIG;
		else
			nextwin = PG_RCONFIG;
		return;
	}
        if(key == KEY_NUM1) //R0
        {
/*                                             
                window_setup(10);                
                if(IS_BORE_MODE)
                {
                        sprintf(strbuf,"请输入铂电阻R0阻值");
	        	sysdata.R0 = wnd_floatinput(sysdata.R0);
		        msg = MSG_INIT;                         
		}else{
		        return;
                        sprintf(strbuf,"请输入V0值");
	        	sysdata.V0 = wnd_floatinput(sysdata.V0);
                        msg = MSG_INIT;
		}
*/		
        }
        if(key == KEY_NUM2) //Rs1
        {   
/*            
                if(IS_BORE_MODE)
                {
                        window_setup(10);                
                        sprintf(strbuf,"请输入PT100内标准阻值");
		        sysdata.Rs1 = wnd_floatinput(sysdata.Rs1);
        		msg = MSG_INIT;		
                }
*/
        }
        if(key == KEY_NUM3) //Rs2
        {       
/*        
                if(IS_BORE_MODE)
                {
                        window_setup(10);                
                        sprintf(strbuf,"请输入PT1000内标准阻值");
		        sysdata.Rs2 = wnd_floatinput(sysdata.Rs2);
        		msg = MSG_INIT;		
                }
*/                
        }
        
	if(msg == KEY_UP) {    
		DEC_DISPCH;
		msg = MSG_INIT;
	}
	if(msg == KEY_DN) {
		INC_DISPCH;
		msg = MSG_INIT;
	}
	
	if(msg == MSG_INIT) {
	        isinit = 1;
		LCD_Cls();
		old_ch = curr_dispch;     

	        curr_dispch = curr_dispch - ((curr_dispch-1) % max_option);		 
		for(i=0;i<sizeof(pgmain_chs)/sizeof(LABEL);i++)//
		{
		        if(curr_dispch == old_ch)
		        {
        			sprintf(strbuf,"CH%02i:",curr_dispch);
                                draw_label(&pgmain_chs[i],SW_NORMAL);
                                draw_label(&pgmain_chs[i],SW_REVERSE);                                
        		}
        		else{
        			sprintf(strbuf,"CH%02i:",curr_dispch);        			
        			draw_label(&pgmain_chs[i],SW_NORMAL);
        		}
			INC_DISPCH;
		}
		curr_dispch = old_ch;
		msg = MSG_REFRESH;
	}
	
	if(msg == MSG_REFRESH) {
		old_ch = curr_dispch;          
		curr_dispch = curr_dispch - ((curr_dispch-1) % max_option);		 
		for(i=0;i<sizeof(pgmain_chs)/sizeof(LABEL);i++)//
		{                         
		        if(IS_THERM_MODE)
		        {
			        cptr = getprbtype(tprbdata.type[sysdata.tid[curr_dispch-1]]);
        			if((sysdata.tid[curr_dispch - 1] == INVALID_PROBE) ||\
	        			(cptr[0] == 'P')){
               				sprintf(strbuf," -------  ");
                                        if((curr_dispch-1 == ch_to_search) || (isinit == 1))
                                        {
                        			draw_label(&pgmain_values[i],SW_NORMAL);
		                		draw_label(&pgmain_temps[i],SW_NORMAL);
		                        }else{
                        			draw_label(&pgmain_values[i],SW_NORMAL|SW_OVERLAP);
		                		draw_label(&pgmain_temps[i],SW_NORMAL|SW_OVERLAP);
		                        }
			        }else{
					sprintf(strbuf,"%8f",rundata.reading[curr_dispch - 1]);
					if(curr_dispch-1 == ch_to_search)
        					sprintf(strbuf+8,"*mV");
                                        else                            
                                                sprintf(strbuf+8," mV");
                                        if((curr_dispch-1 == ch_to_search) || (isinit == 1))
                                        {
        					draw_label(&pgmain_values[i],SW_NORMAL);
        					if(rundata.temperature[curr_dispch - 1] < -9000)
        					        sprintf(strbuf," OVER    ");
        					else
                				        sprintf(strbuf," %8f",rundata.temperature[curr_dispch - 1]);
                				        
                				sprintf(strbuf+8," ! %c",cptr[0]);                  
                			        draw_label(&pgmain_temps[i],SW_NORMAL); 
                                        }else{                                            
                                                draw_label(&pgmain_values[i],SW_NORMAL|SW_OVERLAP);
                				if(rundata.temperature[curr_dispch - 1] < -9000)
                				{
                				        sprintf(strbuf," OVER    ");
                				}else{
                				        sprintf(strbuf," %8f",rundata.temperature[curr_dispch - 1]);
                				}
        				        sprintf(strbuf+8," ! %c",cptr[0]);                                 
	        			        draw_label(&pgmain_temps[i],SW_NORMAL|SW_OVERLAP);                                                 
                                        }
				        if(pgmain_temps[i].type == LBL_HZ6X8)
        				        LCD_Rectange(221,pgmain_temps[i].y-2,221 + 12, pgmain_temps[i].y -2 +10);
        				else                                     //Hz12                            
                				LCD_Rectange(221,pgmain_temps[i].y-2,221 + 12,pgmain_temps[i].y -2 + 16);	        			
	                        }
			}else{
       			        cptr = getprbtype(rprbdata.type[sysdata.rid[curr_dispch-1]]);
        			if((sysdata.rid[curr_dispch - 1] == INVALID_PROBE) ||\
        				(rundata.temperature[curr_dispch - 1] < -9000) ||\
	        			(cptr[0] != 'P'))
	        		{                                           
	        		        if(sysdata.rid[curr_dispch - 1] == INVALID_PROBE)
                				sprintf(strbuf," ------- ohm");
                			if(rundata.temperature[curr_dispch - 1] < -9000)
                        			sprintf(strbuf," NO READ    ");
                                        else
                                        	sprintf(strbuf," -------    ");
                                        if((curr_dispch-1 == ch_to_search) || (isinit == 1))
                                        {
        	        			draw_label(&pgmain_values[i],SW_NORMAL);
                				sprintf(strbuf," -------  ");	        			
		                		draw_label(&pgmain_temps[i],SW_NORMAL);
		                	}else{
        	        			draw_label(&pgmain_values[i],SW_NORMAL|SW_OVERLAP);
                				sprintf(strbuf," -------  ");	        			
		                		draw_label(&pgmain_temps[i],SW_NORMAL|SW_OVERLAP);
		                	}
			        }else{
					sprintf(strbuf,"%8f",rundata.reading[curr_dispch - 1]);     
					if(curr_dispch-1 == ch_to_search)
        					sprintf(strbuf+8,"*ohm");
                                        else
        					sprintf(strbuf+8," ohm");        					
                                        if((curr_dispch-1 == ch_to_search) || (isinit == 1))
                                        {        					
	        			        draw_label(&pgmain_values[i],SW_NORMAL); 
        		        		sprintf(strbuf," %8f",rundata.temperature[curr_dispch - 1]);
	        		        	sprintf(strbuf+8," ! %c",cptr[0]);
		        		        draw_label(&pgmain_temps[i],SW_NORMAL);
		        		 }else{
	        			        draw_label(&pgmain_values[i],SW_NORMAL|SW_OVERLAP); 
        		        		sprintf(strbuf," %8f",rundata.temperature[curr_dispch - 1]);
	        		        	sprintf(strbuf+8," ! %c",cptr[0]);
		        		        draw_label(&pgmain_temps[i],SW_NORMAL|SW_OVERLAP);
		        		 }
				        if(pgmain_temps[i].type == LBL_HZ6X8)
        				        LCD_Rectange(221,pgmain_temps[i].y-2,221+12,pgmain_temps[i].y -2 +10);
        				else                                     //Hz12                            
                				LCD_Rectange(221,pgmain_temps[i].y-2,221+12,pgmain_temps[i].y -2 + 16);
        			}       			        
	                }
			INC_DISPCH;
		}
		curr_dispch = old_ch;
	}
}

//main configuration window of R
LABEL flash pgr_banner = {LBL_HZ16,3,3,7,"铂电阻参数配置"};
LABEL flash pgr_calibrate = {LBL_HZ16,30,30,8,"内标准校准中..."};
LABEL flash pgr_klbl = {LBL_HZ6X8, 88,28, 3,strbuf}; //ktime label
LABEL flash pgr_options[] = {
	{LBL_HZ16, 10,23,7,"1.电流换向"},
	{LBL_HZ16,130,23,7,"2.内标准校准"},
	{LBL_HZ16, 10,45,7,"3.通道探头选择"},
	{LBL_HZ16,130,45,7,"4.设置探头参数"}
};

//LABEL flash usage = {LBL_HZ6X8,100,55,5,"usage"};
#define SLEEPKTIME      cnt = sysdata.ktime * ONESEC; while(cnt > 0) cnt--;
void pgcalibrate()
{                             
        double oldvalue = 0;  
        window_setup(10);
	sprintf(strbuf,"请输入外标准(CH1)阻值");
	oldvalue = wnd_floatinput(oldvalue);
	if(oldvalue == 0)
                return;	
        wnd_msgbox(&pgr_calibrate);   
        if(rprbdata.type[sysdata.rid[curr_dispch-1]] == PRBTYPE_PT1000)
                sysdata.Rs2 = (oldvalue + sysdata.R0)*sysdata.Rs2/(rundata.reading[curr_dispch-1]+sysdata.R0);
        else      
                sysdata.Rs1 = (oldvalue + sysdata.R0)*sysdata.Rs1/(rundata.reading[curr_dispch-1]+sysdata.R0);

        if((sysdata.Rs1 > 101) || (sysdata.Rs1 < 99))      
        {
                sysdata.Rs1 = 100;                         
        }
        if((sysdata.Rs2 > 1010) || (sysdata.Rs2 < 990))                      
        {
                sysdata.Rs2 = 1000;                          
        }
}       
//main menu of bore config
void pgrconfig_handler(uchar msg) {
	uchar i;
	
	static uchar curr_sel = 1;
	static uchar last_sel = 0xff;
	uchar min_option = 1;
	uchar max_option = sizeof(pgr_options)/sizeof(LABEL);

	if(msg == KEY_TAB) {
	        SET_BORE_MODE;          
	        SET_TORS;
		nextwin = PG_MAIN;
		return;
	}                                    
	if(msg == KEY_CE)
	{                
	        nextwin = PG_BOOTTYPE;
	        return;
	}
	if(msg == MSG_INIT) {
		LCD_Cls();
		last_sel = 0xff;
		draw_label(&pgr_banner, SW_NORMAL);
		INIT_OPTIONS(pgr_);
		sprintf(strbuf,"(%i)",sysdata.ktime);
		draw_label(&pgr_klbl, SW_NORMAL);
		msg = MSG_REFRESH;
	}                                 
	if(msg == KEY_DN)
	{                
	        last_sel = curr_sel;
	        curr_sel++;
	        if(curr_sel > max_option)
	                curr_sel = 1;
	        msg = MSG_REFRESH;
	}
	if(msg == KEY_UP)
	{                
	        last_sel = curr_sel;
	        curr_sel--;
	        if(curr_sel == 0)
	                curr_sel = max_option;
	        msg = MSG_REFRESH;
	}
	if(msg == KEY_OK)
	{
	        msg = curr_sel + KEY_NUM0;
	}
	if(msg == KEY_NUM1) {
		window_setup(4); //4 char at max
		sprintf(strbuf,"电流换向时间设置");
		sysdata.ktime = wnd_intinput(sysdata.ktime);
		nextwin = PG_RCONFIG;
		return;
	}
	if(msg == KEY_NUM2)
	{
                pgcalibrate();
		nextwin = PG_RCONFIG;
		return;
	}
	if(msg == KEY_NUM3) {
		nextwin = PG_CHSET;
		return;
	}
	if(msg == KEY_NUM4) {
		nextwin = PG_PRBSET;
		return;
	}
	if(msg == MSG_REFRESH) {
		REFRESH_OPTIONS(pgr_);
	}
}
//main configuration window of T
LABEL flash pgt_banner = {LBL_HZ16,3,3,7,"热电偶参数配置"};
LABEL flash pgt_options[] = {
	{LBL_HZ16,40,25,8,"1.选择各通道探头"},
	{LBL_HZ16,40,45,8,"2.设置各探头参数"}
};
                          
//main menu of thermo config
void pgtconfig_handler(uchar msg) {
	uchar i;
	static uchar curr_sel = 1;
	static uchar last_sel = 0xff;
	uchar min_option = 1;
	uchar max_option = sizeof(pgt_options)/sizeof(LABEL);
	if(msg == KEY_TAB) {
	        SET_THERM_MODE;
		nextwin = PG_MAIN;
		return;
	}              
	if(msg == KEY_CE)
	{                
	        nextwin = PG_BOOTTYPE;
	        return;
	}	
	if(msg == MSG_INIT) {
		LCD_Cls();
		last_sel = 0xff;
		draw_label(&pgt_banner, SW_NORMAL);
		INIT_OPTIONS(pgt_);
		msg = MSG_REFRESH;
	}
	if(msg == KEY_DN)
	{                
	        last_sel = curr_sel;
	        curr_sel++;
	        if(curr_sel > max_option)
	                curr_sel = 1;
	        msg = MSG_REFRESH;
	}
	if(msg == KEY_UP)
	{                
	        last_sel = curr_sel;
	        curr_sel--;
	        if(curr_sel == 0)
	                curr_sel = max_option;
	        msg = MSG_REFRESH;
	}
	if(msg == KEY_OK)
	{
	        msg = curr_sel + KEY_NUM0;
	}	
	if(msg == KEY_NUM1) {
		nextwin = PG_CHSET;
		return;
	}
	if(msg == KEY_NUM2) {
		nextwin = PG_PRBSET;
		return;
	}
	if(msg == MSG_REFRESH) {
		REFRESH_OPTIONS(pgt_);
	}
	return;
}
//channel probe setup
LABEL flash pgch_banner =    {LBL_HZ16,3,3,6,"选择要配置的通道"};
LABEL flash pgch_chvalue = {LBL_HZ6X8,130,3,2,strbuf};
LABEL flash pgch_items[] = {
	{LBL_HZ6X8, 10, 30, 10, strbuf},
	{LBL_HZ6X8, 130,30, 10, strbuf},
	{LBL_HZ6X8, 10, 40, 10, strbuf},
	{LBL_HZ6X8, 130,40, 10, strbuf},
	{LBL_HZ6X8, 10, 50, 10, strbuf},
	{LBL_HZ6X8, 130,50, 10, strbuf}
};

//select the channel to config
void pgchset_handler(uchar msg) {
	static uint curr_pos = 1; //absolute postion in options 1-24
	static uint last_pos = 1;
	uchar min_option = 1;
	uchar max_option = sizeof(pgch_items)/sizeof(LABEL);
	uchar min_index = 1;
	uchar max_index = MAX_CH_NUM;//rundata.scanmode;
	uchar new_page = 0;
	int i,j;
	if(msg == KEY_CE) {
		if(IS_BORE_MODE)
			nextwin = PG_RCONFIG;
		else
			nextwin = PG_TCONFIG;
		return;
	}

	if(msg == KEY_UP) {
		DEC_TABLE;
		msg = MSG_REFRESH;
	}
	if(msg == KEY_DN) {
		INC_TABLE;
		msg = MSG_REFRESH;
	}
	if(msg == KEY_OK) {	//show window to select the probe
		curr_ch = curr_pos;
		nextwin = PG_PRBLIST;
		return;
	}
	if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
	/*
		KEY_TABLE;
		msg = MSG_REFRESH;
	*/
	}
	if(msg == KEY_DOT)
	{                                    
	        if(IS_THERM_MODE)
	        {                         
		        sysdata.tid[curr_pos - 1] = INVALID_PROBE;
	        }else{
       		        sysdata.rid[curr_pos - 1] = INVALID_PROBE;
	        }
		msg = MSG_REFRESH;        
		new_page = 1; //refresh the whole page
	}
	if(msg == MSG_INIT) {
		LCD_Cls();         
		draw_label(&pgch_banner, SW_NORMAL);
		curr_pos = 1;
		new_page = 1;
		msg = MSG_REFRESH;
	}
	if(msg == MSG_REFRESH) {      
	        
		REFRESH_TABLES1(pgch_);
		        if(IS_THERM_MODE)
        	        {
				if(sysdata.tid[j-1] == INVALID_PROBE){
					sprintf(strbuf,"CH%i:*",j);
				}else{
			        	//draw line item j
				        sprintf(strbuf,"CH%i:%s",j,tname2b((sysdata.tid[j-1])));
				}           
		        }else{
				if(sysdata.rid[j-1] == INVALID_PROBE){
					sprintf(strbuf,"CH%i:*",j);
				}else{
			        	//draw line item j
				        sprintf(strbuf,"CH%i:%s",j,rname2b((sysdata.rid[j-1])));
				}           		        
		        }
		REFRESH_TABLES2(pgch_);
	}
	return;
}
//select probe to edit.
LABEL flash pgprbset_banner = {LBL_HZ16,3,3,8,"请选择要配置的探头"};
LABEL flash pgprbset_chvalue = {LBL_HZ6X8,180,3,2,strbuf};
LABEL flash pgprbset_items[] = {
	{LBL_HZ6X8, 10, 30, 10, strbuf},
	{LBL_HZ6X8, 130,30, 10, strbuf},
	{LBL_HZ6X8, 10, 40, 10, strbuf},
	{LBL_HZ6X8, 130,40, 10, strbuf},
	{LBL_HZ6X8, 10, 50, 10, strbuf},
	{LBL_HZ6X8, 130,50, 10, strbuf}
};
//select probe to config
void pgprbset_handler(uchar msg) {
	//LABEL flash usage = {LBL_HZ6X8,100,55,5,"usage"};
	static uint curr_pos = 1;
	static uint last_pos = 1;
	uchar min_option = 1;
	uchar max_option = sizeof(pgprbset_items)/sizeof(LABEL);
	uchar min_index = 1;
	uchar max_index = MAX_PROBE_NUM;
	int i,j;
	uchar new_page = 1;
	if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
	/*
		KEY_TABLE;
		msg = MSG_REFRESH;
	*/
	}                             
	
	if(msg == KEY_CE)
	{
		if(IS_THERM_MODE)
			nextwin = PG_TCONFIG;
		else
			nextwin = PG_RCONFIG;
		return;
	}
	if(msg == KEY_OK)
	{
		curr_prb = curr_pos;
		nextwin = PG_PRBCONFIG;
		return;
	}
	if(msg == MSG_INIT)
	{
		LCD_Cls();         
		draw_label(&pgprbset_banner, SW_NORMAL);
		curr_pos = 1;
		new_page = 1;
		msg = MSG_REFRESH;
	}
	if(msg == KEY_UP) {
		DEC_TABLE;
		msg = MSG_REFRESH;
	}
	if(msg == KEY_DN) {
		INC_TABLE;
		msg = MSG_REFRESH;
	}
	if(msg == MSG_REFRESH) {
		REFRESH_TABLES1(pgprbset_);	
		        if(IS_THERM_MODE)
		        {
			        sprintf(strbuf,"%i.%s",j,tname2b(j-1));
		        }else{                                        
        		        sprintf(strbuf,"%i.%s",j,rname2b(j-1));
		        }
        		draw_label(&(pgprbset_items[i-1]),SW_NORMAL);
		REFRESH_TABLES2(pgprbset_);
	}
	return;
}
//select probe for channel setup
LABEL flash prblist_banner = {LBL_HZ16,3,3,8,strbuf};
LABEL flash prblist_chvalue = {LBL_HZ6X8,180,3,2,strbuf};
LABEL flash prblist_items[] = {
	{LBL_HZ6X8, 10, 30, 10, strbuf},
	{LBL_HZ6X8, 130,30, 10, strbuf},
	{LBL_HZ6X8, 10, 40, 10, strbuf},
	{LBL_HZ6X8, 130,40, 10, strbuf},
	{LBL_HZ6X8, 10, 50, 10, strbuf},
	{LBL_HZ6X8, 130,50, 10, strbuf}
};
//select channel's probe
void pgprblist_handler(uchar msg) {
	//LABEL flash usage = {LBL_HZ6X8,100,55,5,"usage"};
	static uint curr_pos = 1;
	static uint last_pos = 1;
	uchar min_option = 1;
	uchar max_option = sizeof(prblist_items)/sizeof(LABEL);
	uchar min_index = 1;
	uchar max_index = MAX_PROBE_NUM;
	uchar i,j;
	uchar new_page = 1;
	if(msg >= KEY_NUM0 && msg <= KEY_NUM9) {
	/*
		KEY_TABLE;
		msg = MSG_REFRESH;
	*/
	}
	if(msg == KEY_CE)
	{
		nextwin = PG_CHSET;
		return;
	}
	if(msg == KEY_OK)
	{                                      
	        if(IS_THERM_MODE)
	        {                          
       	                if(tprbdata.name[curr_pos-1][0] != '\0')           
        		        sysdata.tid[curr_ch-1] = curr_pos - 1;
	        }else{
                        if(rprbdata.name[curr_pos-1][0] != '\0')           
		                sysdata.rid[curr_ch-1] = curr_pos - 1;	        
	        }
		nextwin = PG_CHSET;
		return;
	}
	if(msg == MSG_INIT)
	{
		LCD_Cls();
		sprintf(strbuf,"请选择通道%i使用的探头",curr_ch);
		draw_label(&prblist_banner, SW_NORMAL);
		curr_pos = 1;
		new_page = 1;
		msg = MSG_REFRESH;
	}
	if(msg == KEY_UP) {
		DEC_TABLE;
		msg = MSG_REFRESH;
	}
	if(msg == KEY_DN) {
		INC_TABLE;
		msg = MSG_REFRESH;
	}              
	if(msg == KEY_DOT){
	        if(IS_THERM_MODE)
	        {
		        sysdata.tid[curr_ch-1] = INVALID_PROBE;
	        }else{
		        sysdata.rid[curr_ch-1] = INVALID_PROBE;	        
	        }	        
       		msg = MSG_REFRESH;
       		new_page = 1;
	}
	if(msg == MSG_REFRESH) {
		REFRESH_TABLES1(prblist_);  
		        if(IS_THERM_MODE)
		        {	
		                sprintf(strbuf,"%s",tname2b(j-1));
		        }else{
        		        sprintf(strbuf,"%s",rname2b(j-1));
        		}
			draw_label(&(prblist_items[i-1]),SW_NORMAL);
		REFRESH_TABLES2(prblist_);
	}
	return;
}
//select probe type list
LABEL flash tplist_banner = {LBL_HZ16,3,3,8,strbuf};
LABEL flash tplist_options[] = {
	{LBL_HZ6X8,10,20,8,"0.PT1000"},
	{LBL_HZ6X8,80,20,8,"1.PT100"},
	{LBL_HZ6X8,150,20,8,"2.PT25"},	
	{LBL_HZ6X8,10,30,8,"3.K-TYPE"},
	{LBL_HZ6X8,80,30,8,"4.N-TYPE"},
	{LBL_HZ6X8,150,30,8,"5.E-TYPE"},
	{LBL_HZ6X8,10,40,8,"6.B-TYPE"},
	{LBL_HZ6X8,80,40,8,"7.J-TYPE"},
	{LBL_HZ6X8,150,40,8,"8.S-TYPE"},
        {LBL_HZ6X8,10,50,8,"9.T-TYPE"},
	{LBL_HZ6X8,10,50,8,"*.R-TYPE"}

};                            
//select probe type thermo
void pgprbtypelist_handler(uchar msg) {
	static int curr_sel = 1;
	static int last_sel = 0xff;
	uchar min_option = 1;
	uchar max_option = sizeof(tplist_options)/sizeof(LABEL);
	uchar i;                     
	if(msg == KEY_DOT)
	{
	        curr_sel = 10;
	        msg = KEY_OK;
	}
	if(msg >= KEY_NUM1 && msg <= KEY_NUM9) {
	        curr_sel = (u8)(msg - KEY_NUM1 + 1) ;
	        msg = KEY_OK;
	}
        if (msg == KEY_NUM0)
        {
                curr_sel = 0;
                msg = KEY_OK;
        }
	if(msg == KEY_CE ) {
		nextwin = PG_PRBCONFIG;
		return;
	}                   
	if(msg == KEY_OK)
	{
	        if(IS_THERM_MODE)
	        {
        		switch(curr_sel)
	        	{                   
	        	        case 0:
	        	               return;
		        	case 1:
        			       return;
	        		case 2:
			               return;
        			default:
	        		       tprbdata.type[curr_prb-1] = curr_sel - 3+PRBTYPE_K;
		        	       break;
        		}            
        	}else{
        		switch(curr_sel)
	        	{            
	        	        case 0:
	        	               rprbdata.type[curr_prb-1] = PRBTYPE_PT1000;
	        	               break;
		        	case 1:
			               rprbdata.type[curr_prb-1] = PRBTYPE_PT100;
        			       break;
	        		case 2:
                       	               sprintf(strbuf,"输入Rtp:");                                              
        	                       rprbdata.rtp[curr_prb-1] = wnd_floatinput(rprbdata.rtp[curr_prb-1]);
		        	       rprbdata.type[curr_prb-1] = PRBTYPE_PT25;        	                       
			               break;
        			default:
                                       return; 
        		}            
        	}
		nextwin = PG_PRBCONFIG;
		return;
	}
	if(msg == MSG_INIT)
	{
		LCD_Cls();
		sprintf(strbuf,"探头%i类型设置为:",curr_prb);
		draw_label(&tplist_banner, SW_NORMAL);
		INIT_OPTIONS(tplist_);
		msg = MSG_REFRESH;
	}
	if(msg == KEY_UP) {
		DEC_OPTION;
		msg = MSG_REFRESH;
	}
	if(msg == KEY_DN) {
		INC_OPTION;
		msg = MSG_REFRESH;
	}
	if(msg == MSG_REFRESH) {
		REFRESH_OPTIONS(tplist_);
	}
	return;
}
/*
 *       Convert the probe type const to string
 */
char PRBSTR[7];
char* getprbtype(uchar prbtype)
{
	switch(prbtype)
	{                           
	        case PRBTYPE_PT1000:
		        sprintf(PRBSTR,"PT1000");
		        return PRBSTR;
		case PRBTYPE_PT25: 	
		        sprintf(PRBSTR,"PT  25");
		        return PRBSTR;
		case PRBTYPE_PT100:
		        sprintf(PRBSTR,"PT 100");
		        return PRBSTR;
		case PRBTYPE_T:
		        sprintf(PRBSTR,"T TYPE");
		        return PRBSTR;

		case PRBTYPE_K:
		        sprintf(PRBSTR,"K TYPE");
		        return PRBSTR;
		case PRBTYPE_N:
		        sprintf(PRBSTR,"N TYPE");
		        return PRBSTR;
		case PRBTYPE_E:
		        sprintf(PRBSTR,"E TYPE");
		        return PRBSTR;
		case PRBTYPE_B:
		        sprintf(PRBSTR,"B TYPE");
		        return PRBSTR;
		case PRBTYPE_J:
		        sprintf(PRBSTR,"J TYPE");
		        return PRBSTR;
		case PRBTYPE_S:
		        sprintf(PRBSTR,"S TYPE");
		        return PRBSTR;
		case PRBTYPE_R:
		        sprintf(PRBSTR,"R TYPE");
		        return PRBSTR;

	}

		        sprintf(PRBSTR,"? TYPE");
		        return PRBSTR;

}
//config r probe parameter
LABEL flash pgprbcfg_banner = {LBL_HZ12,3,3,8,strbuf};
LABEL flash snlbl = {LBL_HZ12,10,17,4,"1.序列号"};
LABEL flash snval = {LBL_HZ6X8,75,22,10,strbuf};
LABEL flash typelbl = {LBL_HZ12,10,31,3,"2.类型"};
LABEL flash typeval = {LBL_HZ6X8,59,36,10,strbuf};
LABEL flash paramlbl1 = {LBL_HZ12,130,31,3,"3."};
LABEL flash paramval1 = {LBL_HZ6X8,147,36,10,strbuf};
LABEL flash paramlbl2 = {LBL_HZ12,10,45,3,"4."};
LABEL flash paramval2 = {LBL_HZ6X8,27,50,10,strbuf};
LABEL flash paramlbl3 = {LBL_HZ12,130,45,3,"5."};
LABEL flash paramval3 = {LBL_HZ6X8,147,50,10,strbuf};
LABEL flash paramlbl3b = {LBL_HZ12,10,45,7,"3."};
LABEL flash paramval3b = {LBL_HZ6X8,27,50,10,strbuf};

//configuration of BORE probe parameter
void pgprbconfig_handler(uchar msg) {
	if( msg == KEY_CE) {
		nextwin = PG_PRBSET;
		return;
	}
	if(msg == MSG_INIT)
	{
		LCD_Cls();                       
		if(IS_THERM_MODE)
		{
		        sprintf(strbuf,"热电偶探头%i参数",curr_prb);
        		draw_label(&pgprbcfg_banner,SW_NORMAL);
	        	//name            
		        strbuf[0]=':';
        		sprintf(strbuf,":%s",tname2b(curr_prb-1));
	        	draw_label(&snlbl,SW_NORMAL);draw_label(&snval,SW_NORMAL);
        		//type
	        	sprintf(strbuf,":%s",getprbtype(tprbdata.type[curr_prb-1]));
		        draw_label(&typelbl,SW_NORMAL);draw_label(&typeval,SW_NORMAL);
		}else{
		        sprintf(strbuf,"铂电阻探头%i参数",curr_prb);
        		draw_label(&pgprbcfg_banner,SW_NORMAL);
	        	//name            
		        strbuf[0]=':';
		        if(rprbdata.type[curr_prb-1] != PRBTYPE_PT25)
		        {
        		        sprintf(strbuf,":%s",rname2b(curr_prb-1));
        	        	draw_label(&snlbl,SW_NORMAL);draw_label(&snval,SW_NORMAL);
        	        }else{  //PT1000 or PT100
               		        sprintf(strbuf,":%s Rtp:%7f",rname2b(curr_prb-1),rprbdata.rtp[curr_prb-1]);
        	        	draw_label(&snlbl,SW_NORMAL);draw_label(&snval,SW_NORMAL);
        	        }
               		//type	        	
                       	sprintf(strbuf,":%s",getprbtype(rprbdata.type[curr_prb-1]));
	                draw_label(&typelbl,SW_NORMAL);draw_label(&typeval,SW_NORMAL);
	                
		        if(rprbdata.type[curr_prb-1] != PRBTYPE_PT25)
		        {
		                sprintf(strbuf,"R(0!):%7f",rprbdata.param3[curr_prb-1]);
        		        draw_label(&paramlbl3b,SW_NORMAL);draw_label(&paramval3b,SW_NORMAL);
        		}else{
        		        //param1
                		sprintf(strbuf,"a:%9.3E",rprbdata.param1[curr_prb-1]);
	        	        draw_label(&paramlbl1,SW_NORMAL);draw_label(&paramval1,SW_NORMAL);
        	        	//param2
	        	        sprintf(strbuf,"b:%9.3E",rprbdata.param2[curr_prb-1]);
                		draw_label(&paramlbl2,SW_NORMAL);draw_label(&paramval2,SW_NORMAL);
	                	//param3
		                sprintf(strbuf,"c:%9.3E",rprbdata.param3[curr_prb-1]);
        		        draw_label(&paramlbl3,SW_NORMAL);draw_label(&paramval3,SW_NORMAL);
        		}
                }
		return;
	}
	if(msg == KEY_NUM1) {
		window_setup(7);
		sprintf(strbuf,"请输入探头序列号");          
		prbsninput();
		nextwin = PG_PRBCONFIG;
		return;
	}
	if(msg == KEY_NUM2){
		nextwin = PG_PRBTYPELIST;
		return;
	}
	if(msg == KEY_NUM3 ){                    
	
	        if(IS_BORE_MODE)
	        {              
               		window_setup(10);
	                if(rprbdata.type[curr_prb-1] != PRBTYPE_PT25)
	                {
        	                sprintf(strbuf,"输入R(0!)");                                              
        	                rprbdata.param3[curr_prb-1] = wnd_floatinput(rprbdata.param1[curr_prb-1]);
                        }else{
        	        	sprintf(strbuf,"输入系数a");
        	        	rprbdata.param1[curr_prb-1] = wnd_floatinput(rprbdata.param1[curr_prb-1]);
        	        }
        		nextwin = PG_PRBCONFIG;
        	}
		return;
	}
	if(msg == KEY_NUM4) {   
        	if(IS_BORE_MODE) 
        	{
	                if(rprbdata.type[curr_prb-1] == PRBTYPE_PT100)
	                        return;
	                if(rprbdata.type[curr_prb-1] == PRBTYPE_PT1000)
	                        return;
        		window_setup(10);
	        	sprintf(strbuf,"输入系数b");
		        rprbdata.param2[curr_prb-1] = wnd_floatinput(rprbdata.param2[curr_prb-1]);
        		nextwin = PG_PRBCONFIG;
                }
		return;
	}
	if(msg == KEY_NUM5) {     
	        if(IS_BORE_MODE)
	        {       
       	                if(rprbdata.type[curr_prb-1] == PRBTYPE_PT100)
       	                        return;     
       	                if(rprbdata.type[curr_prb-1] == PRBTYPE_PT1000)
       	                        return;
	                window_setup(10);       	                        
       		        sprintf(strbuf,"输入系数c");
        		rprbdata.param3[curr_prb-1] = wnd_floatinput(rprbdata.param3[curr_prb-1]);
		        nextwin = PG_PRBCONFIG;
	        }
		return;
	}
	return;
}

LABEL flash boot_banner = {LBL_HZ16,3,3,7,"请选择工作模式"};
LABEL flash boot_options[] = {
 	{LBL_HZ16, 10,23,7,"1.铂电阻测量"},
 	{LBL_HZ16, 130,23,7,"2.铂电阻配置"},
 	{LBL_HZ16, 10,45,7,"3.热电偶测量"},
 	{LBL_HZ16, 130,45,7,"4.热电偶配置"}
};       
//boot up menu                               
extern u8 eeprom scanner_type;
void pgboottype_handler(uchar msg) {
	uchar i;
	static uchar curr_sel = 1;
	static uchar last_sel = 0xff;

	uchar min_option = 1;
	uchar max_option = sizeof(boot_options)/sizeof(LABEL);
	if(msg == KEY_NUM5) { //select scanner type
       		window_setup(4); //2 char at max
		sprintf(strbuf,"Scanner(1:MI,2:GUIDLINE)");    
		i = scanner_type;
		i = wnd_intinput(i); 
		if(i == 1 || i == 2)
		        scanner_type = i;
		else
		        scanner_type = 1;
	        msg = MSG_INIT;
	}

	if(msg == MSG_INIT) {
		LCD_Cls();
		last_sel = 0xff;
		draw_label(&boot_banner, SW_NORMAL);
		INIT_OPTIONS(boot_);
		msg = MSG_REFRESH;
	}
	if(msg == KEY_DN)
	{                
	        last_sel = curr_sel;
	        curr_sel++;
	        if(curr_sel > max_option)
	                curr_sel = 1;
	        msg = MSG_REFRESH;
	}
	if(msg == KEY_UP)
	{                
	        last_sel = curr_sel;
	        curr_sel--;
	        if(curr_sel == 0)
	                curr_sel = max_option;
	        msg = MSG_REFRESH;
	}
	if(msg == KEY_OK)
	{
	        msg = curr_sel + KEY_NUM0;
	}
	if(msg == KEY_NUM1) {
		SET_BORE_MODE;
		SET_TORS;
		nextwin = PG_MAIN;
		return;
	}
	if(msg == KEY_NUM2) {
		SET_BORE_MODE;
		SET_TORS;
		nextwin = PG_RCONFIG;
		return;
	}
	if(msg == KEY_NUM3) {
		SET_THERM_MODE;
		nextwin = PG_MAIN;
		return;
	}
	if(msg == KEY_NUM4) {
		SET_THERM_MODE;
		nextwin = PG_TCONFIG;
		return;
	}                      
	if(msg == MSG_REFRESH) {
		REFRESH_OPTIONS(boot_);
	}
}