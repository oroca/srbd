//----------------------------------------------------------------------------
//    프로그램명 	: 하드웨어 레지스터 Define
//
//    만든이     	: 
//
//    날  짜     	:
//
//    최종 수정  	:
//
//    MPU_Type		:
//
//    파일명     	: Hw_Regs.h
//----------------------------------------------------------------------------


#ifndef _HW_REGS_H_
#define _HW_REGS_H_



//-- PWR
//
#define REG_PWR_BASE					0x40007000
#define REG_PWR_CR						(*(volatile unsigned long *)(REG_PWR_BASE + 0x0000))
#define REG_PWR_CSR						(*(volatile unsigned long *)(REG_PWR_BASE + 0x0004))



//-- RCC (reset and clock control )
//
#define REG_RCC_BASE					0x40021000
#define REG_RCC_CR						(*(volatile unsigned long *)(REG_RCC_BASE + 0x0000))
#define REG_RCC_CFGR					(*(volatile unsigned long *)(REG_RCC_BASE + 0x0004))
#define REG_RCC_CIR						(*(volatile unsigned long *)(REG_RCC_BASE + 0x0008))
#define REG_RCC_APB2RSTR				(*(volatile unsigned long *)(REG_RCC_BASE + 0x000C))
#define REG_RCC_AHBENR					(*(volatile unsigned long *)(REG_RCC_BASE + 0x0014))
#define REG_RCC_APB2ENR					(*(volatile unsigned long *)(REG_RCC_BASE + 0x0018))
#define REG_RCC_APB1ENR					(*(volatile unsigned long *)(REG_RCC_BASE + 0x001C))
#define REG_RCC_BDCR					(*(volatile unsigned long *)(REG_RCC_BASE + 0x0020))
#define REG_RCC_CSR						(*(volatile unsigned long *)(REG_RCC_BASE + 0x0024))
#define REG_RCC_AHBSTR					(*(volatile unsigned long *)(REG_RCC_BASE + 0x0028))
#define REG_RCC_CFGR2					(*(volatile unsigned long *)(REG_RCC_BASE + 0x002C))



//-- FLASH Memory
//
#define REG_FLASH_BASE					0x40022000
#define REG_FLASH_ACR					(*(volatile unsigned long *)(REG_FLASH_BASE + 0x0000))
#define REG_FLASH_KEYR					(*(volatile unsigned long *)(REG_FLASH_BASE + 0x0004))
#define REG_FLASH_OPTKEYR				(*(volatile unsigned long *)(REG_FLASH_BASE + 0x0008))
#define REG_FLASH_SR					(*(volatile unsigned long *)(REG_FLASH_BASE + 0x000C))
#define REG_FLASH_CR					(*(volatile unsigned long *)(REG_FLASH_BASE + 0x0010))
#define REG_FLASH_AR					(*(volatile unsigned long *)(REG_FLASH_BASE + 0x0014))
#define REG_FLASH_OBR					(*(volatile unsigned long *)(REG_FLASH_BASE + 0x001C))
#define REG_FLASH_WRPR					(*(volatile unsigned long *)(REG_FLASH_BASE + 0x0020))



//-- USART
//
#define REG_USART1_BASE					0x40013800
#define REG_USART1_SR					(*(volatile unsigned long *)(REG_USART1_BASE + 0x0000))
#define REG_USART1_DR					(*(volatile unsigned long *)(REG_USART1_BASE + 0x0004))
#define REG_USART1_BRR					(*(volatile unsigned long *)(REG_USART1_BASE + 0x0008))
#define REG_USART1_CR1					(*(volatile unsigned long *)(REG_USART1_BASE + 0x000C))
#define REG_USART1_CR2					(*(volatile unsigned long *)(REG_USART1_BASE + 0x0010))
#define REG_USART1_CR3					(*(volatile unsigned long *)(REG_USART1_BASE + 0x0014))
#define REG_USART1_GTPR					(*(volatile unsigned long *)(REG_USART1_BASE + 0x0018))



//-- AFIO
//
#define REG_AFIO_BASE					0x40010000
#define REG_AFIO_EVCR					(*(volatile unsigned long *)(REG_AFIO_BASE + 0x0000))
#define REG_AFIO_MAPR					(*(volatile unsigned long *)(REG_AFIO_BASE + 0x0004))
#define REG_AFIO_EXTICR1				(*(volatile unsigned long *)(REG_AFIO_BASE + 0x0008))
#define REG_AFIO_EXTICR2				(*(volatile unsigned long *)(REG_AFIO_BASE + 0x000C))
#define REG_AFIO_EXTICR3				(*(volatile unsigned long *)(REG_AFIO_BASE + 0x0010))
#define REG_AFIO_EXTICR4				(*(volatile unsigned long *)(REG_AFIO_BASE + 0x0014))
#define REG_AFIO_MAPR2					(*(volatile unsigned long *)(REG_AFIO_BASE + 0x001C))



//-- GPIO
//
#define REG_GPIOA_BASE					0x40010800
#define REG_GPIOA_CRL					(*(volatile unsigned long *)(REG_GPIOA_BASE + 0x0000))
#define REG_GPIOA_CRH					(*(volatile unsigned long *)(REG_GPIOA_BASE + 0x0004))
#define REG_GPIOA_IDR					(*(volatile unsigned long *)(REG_GPIOA_BASE + 0x0008))
#define REG_GPIOA_ODR					(*(volatile unsigned long *)(REG_GPIOA_BASE + 0x000C))
#define REG_GPIOA_BSRR					(*(volatile unsigned long *)(REG_GPIOA_BASE + 0x0010))
#define REG_GPIOA_BRR					(*(volatile unsigned long *)(REG_GPIOA_BASE + 0x0014))
#define REG_GPIOA_LCKR					(*(volatile unsigned long *)(REG_GPIOA_BASE + 0x0018))


#define REG_GPIOB_BASE					0x40010C00
#define REG_GPIOB_CRL					(*(volatile unsigned long *)(REG_GPIOB_BASE + 0x0000))
#define REG_GPIOB_CRH					(*(volatile unsigned long *)(REG_GPIOB_BASE + 0x0004))
#define REG_GPIOB_IDR					(*(volatile unsigned long *)(REG_GPIOB_BASE + 0x0008))
#define REG_GPIOB_ODR					(*(volatile unsigned long *)(REG_GPIOB_BASE + 0x000C))
#define REG_GPIOB_BSRR					(*(volatile unsigned long *)(REG_GPIOB_BASE + 0x0010))
#define REG_GPIOB_BRR					(*(volatile unsigned long *)(REG_GPIOB_BASE + 0x0014))
#define REG_GPIOB_LCKR					(*(volatile unsigned long *)(REG_GPIOB_BASE + 0x0018))


#define REG_GPIOC_BASE					0x40011000
#define REG_GPIOC_CRL					(*(volatile unsigned long *)(REG_GPIOC_BASE + 0x0000))
#define REG_GPIOC_CRH					(*(volatile unsigned long *)(REG_GPIOC_BASE + 0x0004))
#define REG_GPIOC_IDR					(*(volatile unsigned long *)(REG_GPIOC_BASE + 0x0008))
#define REG_GPIOC_ODR					(*(volatile unsigned long *)(REG_GPIOC_BASE + 0x000C))
#define REG_GPIOC_BSRR					(*(volatile unsigned long *)(REG_GPIOC_BASE + 0x0010))
#define REG_GPIOC_BRR					(*(volatile unsigned long *)(REG_GPIOC_BASE + 0x0014))
#define REG_GPIOC_LCKR					(*(volatile unsigned long *)(REG_GPIOC_BASE + 0x0018))


//-- NVIC
//
#define REG_NVIC_BASE					0xE000E100
#define REG_NVIC_ISER(x)				(*(volatile unsigned long *)(REG_NVIC_BASE + 0x0000 + 4*(x)))
#define REG_NVIC_ICER(x)				(*(volatile unsigned long *)(REG_NVIC_BASE + 0x0080 + 4*(x)))
#define REG_NVIC_ISPR(x)				(*(volatile unsigned long *)(REG_NVIC_BASE + 0x0100 + 4*(x)))
#define REG_NVIC_ICPR(x)				(*(volatile unsigned long *)(REG_NVIC_BASE + 0x0180 + 4*(x)))
#define REG_NVIC_IABR(x)				(*(volatile unsigned long *)(REG_NVIC_BASE + 0x0200 + 4*(x)))



//-- SCB
//
#define REG_SCB_BASE					0xE000ED00
#define REG_SCB_CPUID					(*(volatile unsigned long *)(REG_SCB_BASE + 0x0000))
#define REG_SCB_ICSR					(*(volatile unsigned long *)(REG_SCB_BASE + 0x0004))
#define REG_SCB_VTOR					(*(volatile unsigned long *)(REG_SCB_BASE + 0x0008))
#define REG_SCB_AIRCR					(*(volatile unsigned long *)(REG_SCB_BASE + 0x000C))
#define REG_SCB_SCR						(*(volatile unsigned long *)(REG_SCB_BASE + 0x0010))
#define REG_SCB_CCR						(*(volatile unsigned long *)(REG_SCB_BASE + 0x0014))
#define REG_SCB_CCR						(*(volatile unsigned long *)(REG_SCB_BASE + 0x0014))



//-- STK ( SysTick )
//
#define REG_STK_BASE					0xE000E010
#define REG_STK_CTRL					(*(volatile unsigned long *)(REG_STK_BASE + 0x0000))
#define REG_STK_LOAD					(*(volatile unsigned long *)(REG_STK_BASE + 0x0004))
#define REG_STK_VAL						(*(volatile unsigned long *)(REG_STK_BASE + 0x0008))
#define REG_STK_CALIB					(*(volatile unsigned long *)(REG_STK_BASE + 0x000C))



//-- EXTI 
//
#define REG_EXTI_BASE					0x40010400
#define REG_EXTI_IMR					(*(volatile unsigned long *)(REG_EXTI_BASE + 0x0000))
#define REG_EXTI_EMR					(*(volatile unsigned long *)(REG_EXTI_BASE + 0x0004))
#define REG_EXTI_RTSR					(*(volatile unsigned long *)(REG_EXTI_BASE + 0x0008))
#define REG_EXTI_FTSR					(*(volatile unsigned long *)(REG_EXTI_BASE + 0x000C))
#define REG_EXTI_SWIER					(*(volatile unsigned long *)(REG_EXTI_BASE + 0x0010))
#define REG_EXTI_PR						(*(volatile unsigned long *)(REG_EXTI_BASE + 0x0014))



//-- I2C
//
#define REG_I2C1_BASE					0x40005400
#define REG_I2C1_CR1					(*(volatile unsigned long *)(REG_I2C1_BASE + 0x0000))
#define REG_I2C1_CR2					(*(volatile unsigned long *)(REG_I2C1_BASE + 0x0004))
#define REG_I2C1_OAR1					(*(volatile unsigned long *)(REG_I2C1_BASE + 0x0008))
#define REG_I2C1_OAR2					(*(volatile unsigned long *)(REG_I2C1_BASE + 0x000C))
#define REG_I2C1_DR						(*(volatile unsigned long *)(REG_I2C1_BASE + 0x0010))
#define REG_I2C1_SR1					(*(volatile unsigned long *)(REG_I2C1_BASE + 0x0014))
#define REG_I2C1_SR2					(*(volatile unsigned long *)(REG_I2C1_BASE + 0x0018))
#define REG_I2C1_CCR					(*(volatile unsigned long *)(REG_I2C1_BASE + 0x001C))
#define REG_I2C1_TRISE					(*(volatile unsigned long *)(REG_I2C1_BASE + 0x0020))


#define REG_I2C2_BASE					0x40005800
#define REG_I2C2_CR1					(*(volatile unsigned long *)(REG_I2C2_BASE + 0x0000))
#define REG_I2C2_CR2					(*(volatile unsigned long *)(REG_I2C2_BASE + 0x0004))
#define REG_I2C2_OAR1					(*(volatile unsigned long *)(REG_I2C2_BASE + 0x0008))
#define REG_I2C2_OAR2					(*(volatile unsigned long *)(REG_I2C2_BASE + 0x000C))
#define REG_I2C2_DR						(*(volatile unsigned long *)(REG_I2C2_BASE + 0x0010))
#define REG_I2C2_SR1					(*(volatile unsigned long *)(REG_I2C2_BASE + 0x0014))
#define REG_I2C2_SR2					(*(volatile unsigned long *)(REG_I2C2_BASE + 0x0018))
#define REG_I2C2_CCR					(*(volatile unsigned long *)(REG_I2C2_BASE + 0x001C))
#define REG_I2C2_TRISE					(*(volatile unsigned long *)(REG_I2C2_BASE + 0x0020))



#endif






















