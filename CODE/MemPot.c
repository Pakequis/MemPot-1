/** ###################################################################
**     Filename  : MemPot.C
**     Project   : MemPot
**     Processor : MC9S08QG8MPB
**     Version   : Driver 01.11
**     Compiler  : CodeWarrior HCS08 C Compiler
**     Date/Time : 14/12/2013, 18:29
**     Abstract  :
**         Main module.
**         This module contains user's application code.
**     Settings  :
**     Contents  :
**         No public methods
**
** ###################################################################*/
/* MODULE MemPot */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "MotorCtrl.h"
#include "AD1.h"
#include "Teclas.h"
/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */

/*###################################################################
**   Codigo para teste da ideia do "Potenciometro com memoria"
**   
**   Rodrigo Feliciano (Voyage) - 22/12/2013
**   
**   Meu blog: http://www.pakequis.com.br
**
**   TODO:
**   - Melhorar a comparacao para diminuir a margem de erro no retorno
**     do potenciometro para o valor memorizado.
**   - Colocar "debounce" nas teclas (usar interrupcao?)
**   - Colocar LEDs indicadores
**
#####################################################################*/
#define MOTOR_STDBY 0   /* Motor parado */
#define MOTOR_CCW   1   /* Sentido anti-horario */
#define MOTOR_CW    2   /* Sentido horario */
#define MOTOR_BRAKE 3   /* Freio */

#define MIN_VALUE   5     /* Menor valor de leitura no conversor AD */
#define MAX_VALUE   65400 /* Maior valor de leitura no conversor AD */

#define MEM_KEY     2   /* Pressionada a tecla de memoria */
#define RM_KEY      1   /* Pressionada a tecla de recuperar memoria */
#define NO_KEY      3   /* Nenhuma tecla pressionada */


word AD_Value = 0;      /* Leitura do conversor AD - Carregada na interrupcao do AD (em Events.c) */
word Mem_Value = 32000; /* Valor memorizado */

byte Key = NO_KEY;      /* Indica qual chave foi pressionada */

void main(void)
{
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  /* For example: for(;;) { } */
  
  MotorCtrl_PutVal(MOTOR_STDBY);  /* Desliga o motor */
  
  for(;;)
  {
    (void)AD1_Measure(0);         /* Inicia medida no conversor AD */
    
    Key = Teclas_GetVal();        /* Le as teclas */
    
    /* Verifica se uma tecla foi pressionada */
    if (Key == MEM_KEY)           /* Tecla de memoria? */
    {
      Mem_Value = AD_Value;       /* Memoriza novo valor */
      if (Mem_Value > MAX_VALUE)  /* Novo valor eh maior que o valor maximo permitido? */
      {                                
        Mem_Value = MAX_VALUE;    /* Nao passa do valor maximo permitido */
      }
      
      if (Mem_Value < MIN_VALUE)  /* Novo valor eh menor que o valor minimo permitido? */
      {
        Mem_Value = MIN_VALUE;    /* Nao passa do valor minimo permitido */
      }
      
      Key = NO_KEY;   /* Nenhuma chave pressioanada */
    } 
    else if (Key == RM_KEY) /* Tecla de recuperar? */
    {
      if (Mem_Value > AD_Value)         /* Verifica qual eh o valor maior */
      {
        while (Mem_Value >= AD_Value)   /* Enquanto nao ultrapassar o valor memorizado */
        {
           (void)AD1_Measure(0);        /* Inicia medida no conversor AD */
           MotorCtrl_PutVal(MOTOR_CW);  /* Gira motor no sentido horario */
        } 
        /* Talvez nao seja necessario freiar o motor, mas... */
        MotorCtrl_PutVal(MOTOR_BRAKE);  /* Freia o motor */
        Cpu_Delay100US(1000);           /* Espera 100ms */
        MotorCtrl_PutVal(MOTOR_STDBY);  /* Desliga o motor */
                   
      }else
      {
        while (AD_Value >= Mem_Value)   /* Enquanto nao ultrapassar o valor memorizado */
        {
           (void)AD1_Measure(0);        /* Inicia medida no conversor AD */
           MotorCtrl_PutVal(MOTOR_CCW); /* Gira o motor no sentido anti-horario*/       
        }  
        /* Talvez nao seja necessario freiar o motor, mas... */
        MotorCtrl_PutVal(MOTOR_BRAKE);  /* Freia o motor */
        Cpu_Delay100US(1000);           /* Espera 100ms */
        MotorCtrl_PutVal(MOTOR_STDBY);  /* Desliga o motor */       
      }
      
      Key = NO_KEY;   /* Nenhuma chave pressionada */  
      
    }
  }
  
  
  

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END MemPot */
/*
** ###################################################################
**
**     This file was created by Processor Expert 3.07 [04.34]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
