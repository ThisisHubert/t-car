



#include <tm/tmonitor.h>
#include <tk/tkernel.h>
#define SCB_STIR 0xE000EF00
#define CGICRCG 0x400F3060
#define INT3 3

ID cycid;

void sw_int(UINT intno) 
{
  *(_UW*)SCB_STIR = intno;    
}


void inthdr(UINT intno)   // input from SW3 
{
  ClearInt(INT3);
  *(_UW*) CGICRCG = INT3;
  tk_wup_tsk(cycid);
  return;
}


void cychdr(void *exinf)
{
   

  static BOOL on_off = FALSE; 
  
  tk_slp_tsk(TMO_FEVR);
  if (on_off)  {
    tk_stp_cyc(cycid);
   tm_printf("LED_OFF\n");
    on_off = FALSE;
  
  }else{
    tk_sta_cyc(cycid);
    tm_printf("LED_ON\n");
    on_off = TRUE; 
  
  }

  return;


}
 



  
 
// create blink cycle handler     
T_CCYC   ccyc; 
T_DINT dint;

ccyc.cycatr = TA_HLNG;
ccyc.cychdr = cychdr;
ccyc.cyctim = 500;
ccyc.cycphs = 500;
cycid = tk_cre_cyc(&ccyc);  //Create alarm handler 

tk_sta_cyc(cycid); //Start alarm handler 


// Register INT3 interrupt handler 

// Enable INT3 interrupt 
  
  UINT intsts;
  DI(intsts);
  SetIntMode(INT3, IM_EDGE | IM_LOW);
  ClearInt(INT3);
  EnableInt(INT3, 5);
  EI(intsts);
  dint.intatr = TA_HLNG;
  dint.inthdr = inthdr;
  tk_def_int(INT3, &dint);


while(1) 
    {
      sw_int(INT3);
      tk_dly_tsk(1000);
    }
 

//tk_slp_tsk(TMO_FEVR);

return 0; 

}