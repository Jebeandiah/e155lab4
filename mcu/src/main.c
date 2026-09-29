#include <stdint.h>
#define GPIOA_BASE_ADR (0x48000000UL)
#define TIM6_BASE_ADR (0x40001000UL)
#define TIM7_BASE_ADR (0x40001400UL)
#define RCC_BASE_ADR (0x40021000UL)
#define RCC_APB1ENR  (*(uint32_t *) (RCC_BASE_ADR + 0x58))
#define RCC_AHB2ENR (*(uint32_t *) (RCC_BASE_ADR + 0x4C))
//#define GPIOA_MODER ((uint32_t *) (GPIOA_BASE_ADR + 0x00))
//#define GPIOA_ODR ((uint32_t *) (GPIOA_BASE_ADR + 0x14))
typedef struct {
    volatile uint32_t MODER;   // 0x00
    volatile uint32_t na[4];  // 0x04
    volatile uint32_t ODR;     // 0x14
} GPIO_TypeDef;
typedef struct {
    volatile uint32_t CR1;          // 0x00 - register 1
    volatile uint32_t na[3];          // 0x04
    volatile uint32_t SR;           // 0x10
    volatile uint32_t EGR;   
    volatile uint32_t na1[4]; // Reserved: 0x18, 0x1C, 0x20         // 0x28
    volatile uint32_t PSC; 
    volatile uint32_t ARR;          // 0x2C   only first 16 bits
} TIM_TypeDef;
#define GPIOA ((GPIO_TypeDef *) GPIOA_BASE_ADR)
#define TIM6 ((TIM_TypeDef *) TIM6_BASE_ADR)
#define TIM7 ((TIM_TypeDef *) TIM7_BASE_ADR)
 const int notes[][2] = {
 {523, 500}, 
{622,  500},
{698,  375}, 
{622,  375}, 
{698,  250}, 

// Measure 2
{698,  250},
{698,  250}, 
{932, 250},
{831,  250},
{784,  125},
{698,  250},
{784,  375},
{0,  250},

// Measure 3
{784,  500},
{932, 500},
{1047,  375}, 
{698, 375},
{622, 250},
// Measure 4
{932, 250},
{932, 250},
{784,  250},
{932, 250},
{932, 375},
{1047, 625}, 

//measure 4
{1047, 1000},

{0, 1000},

//measure5
{622,  250}, // C5 (Dotted quarter note)
{466,  1000}, // C5 (Dotted quarter note)
{622,  500},


{698, 500},
{466,  500},

{  0, 1000},  // End of sequence snippet
 {659,	125},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{494,	125},
{587,	125},
{523,	125},
{440,	250},
{  0,	125},
{262,	125},
{330,	125},
{440,	125},
{494,	250},
{  0,	125},
{330,	125},
{416,	125},
{494,	125},
{523,	250},
{  0,	125},
{330,	125},
{659,	125},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{494,	125},
{587,	125},
{523,	125},
{440,	250},
{  0,	125},
{262,	125},
{330,	125},
{440,	125},
{494,	250},
{  0,	125},
{330,	125},
{523,	125},
{494,	125},
{440,	250},
{  0,	125},
{494,	125},
{523,	125},
{587,	125},
{659,	375},
{392,	125},
{699,	125},
{659,	125},
{587,	375},
{349,	125},
{659,	125},
{587,	125},
{523,	375},
{330,	125},
{587,	125},
{523,	125},
{494,	250},
{  0,	125},
{330,	125},
{659,	125},
{  0,	250},
{659,	125},
{1319,	125},
{  0,	250},
{623,	125},
{659,	125},
{  0,	250},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{494,	125},
{587,	125},
{523,	125},
{440,	250},
{  0,	125},
{262,	125},
{330,	125},
{440,	125},
{494,	250},
{  0,	125},
{330,	125},
{416,	125},
{494,	125},
{523,	250},
{  0,	125},
{330,	125},
{659,	125},
{623,	125},
{659,	125},
{623,	125},
{659,	125},
{494,	125},
{587,	125},
{523,	125},
{440,	250},
{  0,	125},
{262,	125},
{330,	125},
{440,	125},
{494,	250},
{  0,	125},
{330,	125},
{523,	125},
{494,	125},
{440,	500},
{  0,	1000} //end of furelise


    };
void start_delay_micros(TIM_TypeDef* TIMx, uint32_t micros){
  TIMx->CR1 &= ~(1U<<0);  //cenable off
  //uint16_t maxcount = ms*4000; //maybe safer to cast down
  uint16_t prescaler = 1;
  if(micros%1000 ==0U) //if divisible into clean millis
    {
    prescaler = 4000; //scale to 1 khz
    } 

  
  TIMx->ARR = (uint16_t)((micros*4)/prescaler)-1;

  TIMx->PSC = (uint16_t) prescaler-1; //set presc
  TIMx ->EGR |=(1U<<0);  //update flag set
  TIMx->SR &= ~(1U<<0); //clear uif flag

  //while((TIMx->SR &= (1U<<0))==0){}
  TIMx->CR1 |= (1U<<0);  //cenable on

}


void play_note(int note[2]) 
{
  start_delay_micros(TIM6, note[1]*1000);
  while((TIM6->SR & (1U<<0))==0)
    {
    if(note[0]!=0) 
      {
      GPIOA->ODR ^= (1 << 5);
      uint32_t half_period_micros = 500000/note[0];
      start_delay_micros(TIM7, half_period_micros);
      while((TIM7->SR & (1U<<0))==0){}
      }
    }
}
int main(void) {
//strt gpioa clk
RCC_AHB2ENR |= (1U<<0);

//start tim67 clks
RCC_APB1ENR |= (1<<4);
RCC_APB1ENR |= (1<<5);


// Set PA5 as output 
GPIOA->MODER &= ~(1 << 11);
GPIOA->MODER |= (1 << 10);

TIM6->CR1 &= ~(1U<<0);  //cenable off
TIM7->CR1 &= ~(1U<<0);  //cenable off
int num_notes = sizeof(notes) / sizeof(notes[0]);
for (int i = 0; i < num_notes; i++) 
{
  play_note(notes[i]);
}


}



