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
{  0,	0}};
void start_delay_counts(TIM_TypeDef* TIMx, uint16_t counts){
TIMx->CR1 &= ~(1U<<0);  //cenable off
//uint16_t maxcount = ms*4000; //maybe safer to cast down
TIMx->ARR = counts;
TIMx->PSC = (uint16_t) 40-1; //prescaler to 100khz
TIMx ->EGR |=(1U<<0);  //update flag set
TIMx->SR &= ~(1U<<0); //clear uif flag

//while((TIMx->SR &= (1U<<0))==0){}
TIMx->CR1 |= (1U<<0);  //cenable on

}


void play_note(int note[2]) {
start_delay_counts(TIM6, note[1]*100);
while((TIM6->SR & (1U<<0))==0){
  if(note[0]!=0) {
  GPIOA->ODR ^= (1 << 5);
  uint32_t half_period_counts = 500000/note[0];
  start_delay_counts(TIM7, half_period_counts);
  while((TIM7->SR & (1U<<0))==0){
    }
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
for (int i = 0; i < num_notes; i++) {



  play_note(notes[i]);
}


//while(1){
//  GPIOA->ODR |= (1 << 5);
//  start_delay_counts(TIM6, 50000);
//  while((TIM6->SR & (1U<<0))==0){  
//  }
//    //GPIOA->ODR &= ~(1 << 5);
//  start_delay_counts(TIM6, 50000);
//  while((TIM6->SR & (1U<<0))==0){  
//  }
//    start_delay_counts(TIM6, 50000);
//  while((TIM6->SR & (1U<<0))==0){  
//  }
//    GPIOA->ODR &= ~(1 << 5);
//    start_delay_counts(TIM6, 50000);
//  while((TIM6->SR & (1U<<0))==0){  
//  }

//  start_delay_millis(TIM6, 1000);
//  while((TIM6->SR & (1U<<0))==0){  
//  }
//}

}



