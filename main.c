#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
struct gpio{
	volatile uint32_t MODER,OTYPER,OSPEEDR,PUPDR,IDR,ODR,BSRR,LCKR,AFR[2];
};
#define GPIOD ((struct gpio *) 0x40020C00)

struct rcc {
  volatile uint32_t CR, PLLCFGR, CFGR, CIR, AHB1RSTR, AHB2RSTR, AHB3RSTR,
      RESERVED0, APB1RSTR, APB2RSTR, RESERVED1[2], AHB1ENR, AHB2ENR, AHB3ENR,
      RESERVED2, APB1ENR, APB2ENR, RESERVED3[2], AHB1LPENR, AHB2LPENR,
      AHB3LPENR, RESERVED4, APB1LPENR, APB2LPENR, RESERVED5[2], BDCR, CSR,
      RESERVED6[2], SSCGR, PLLI2SCFGR;
  //structure that holds all the RCC registers that are responsible for managing the clock and reset functionalities of peripherals
  //with some of these registers we are able to turn on peripherals that are turned off initially

};

#define RCC ((struct rcc *) 0x40023800)

struct systick{
	volatile uint32_t CSR, RVR,CVR,CALIB;
};
#define SYSTICK ((struct systick*) 0xE000E010)

static inline void systick_init(uint32_t ticks){
	if(ticks-1 > 0xffffff ) return;
	SYSTICK->RVR = ticks-1;
	SYSTICK->CVR = 0;
	SYSTICK->CSR |= (1UL << 0) | (1UL << 1) | (1UL << 2);
	RCC->APB2ENR |= (1UL << 14); //write all the reference manual pages and shit here on comments


}

volatile uint32_t counter;

void SysTick_Handler(void)
{
	//the original way was to increment a counter each time a clock cycle occured once that counter reached a certain number
	//of miliseconds  (16 MHZ means there is 16 million clock cycles per second)
	// the state of an LED changed
	//either rework that do it ur way without the using a expired_timer function
	//OR
	//find a diff way on the internet to use systick to

	// pass a different number of ticks to the initialization systick_init so that every interrupt the LED changes state
	// instead of using processor clock use an external clock that will trigger an interrupt every however wanted seconds
	// make it configurable so it can be easily changable.
	counter++;
}

void delay(uint32_t time) {
	counter =0;
	while(counter < time);
}


#define FREQ 16000000


int main(void) {
	RCC->AHB1ENR |= (1UL << 3);

	GPIOD->MODER |= (1U << 26);
	GPIOD->MODER |= (1U << 24);
	GPIOD->MODER |= (1U << 28);
	GPIOD->MODER |= (1U << 30);




	systick_init(FREQ/1000);



	while(1){
		GPIOD->ODR |= (1U<<13);
		delay(100);
		GPIOD->ODR |= (1U<<14);
		delay(100);
		GPIOD->ODR |= (1U<<15);
		delay(100);
		GPIOD->ODR |= (1U<<12);
		delay(100);

		GPIOD->ODR &= ~(1U <<13);
		delay(100);
		GPIOD->ODR &= ~(1U <<14);
		delay(100);
		GPIOD->ODR &= ~(1U <<15);
		delay(100);
		GPIOD->ODR &= ~(1U <<12);
		delay(100);



	}


}
