#include "nvic.h"
#include "ebox.h"


  
const DevToIRQn_t dev_to_IRQn_table[] = 
{
    {TIM1_BASE,TIM1_BRK_IRQn,TIM1_UP_IRQn,TIM1_TRG_COM_IRQn,TIM1_CC_IRQn},
    {TIM2_BASE,TIM2_IRQn},
    {TIM3_BASE,TIM3_IRQn},
    {TIM4_BASE,TIM4_IRQn},
    
    {ADC1_BASE,ADC1_2_IRQn},
    {ADC2_BASE,ADC1_2_IRQn},
    
    {SPI1_BASE,SPI1_IRQn},
    {SPI2_BASE,SPI2_IRQn},
    
    {I2C1_BASE,I2C1_EV_IRQn,I2C1_ER_IRQn},
    {I2C2_BASE,I2C2_EV_IRQn,I2C2_ER_IRQn},
    
    {DMA1_Channel1_BASE,DMA1_Channel1_IRQn},
    {DMA1_Channel2_BASE,DMA1_Channel2_IRQn},
    {DMA1_Channel3_BASE,DMA1_Channel3_IRQn},
    {DMA1_Channel4_BASE,DMA1_Channel4_IRQn},
    {DMA1_Channel5_BASE,DMA1_Channel5_IRQn},
    {DMA1_Channel6_BASE,DMA1_Channel6_IRQn},
    {DMA1_Channel7_BASE,DMA1_Channel7_IRQn},
    
   
    {USART1_BASE,USART1_IRQn},
    {USART2_BASE,USART2_IRQn},
    {USART3_BASE,USART3_IRQn},
    
    #ifdef STM32F10X_HD
    
    {TIM5_BASE,TIM5_IRQn},
    {TIM6_BASE,TIM6_IRQn},
    {TIM7_BASE,TIM7_IRQn},
    {TIM8_BASE,TIM8_BRK_IRQn,TIM8_UP_IRQn,TIM8_TRG_COM_IRQn,TIM8_CC_IRQn},

    {DMA2_Channel1_BASE,DMA2_Channel1_IRQn},
    {DMA2_Channel2_BASE,DMA2_Channel2_IRQn},
    {DMA2_Channel3_BASE,DMA2_Channel3_IRQn},
    {DMA2_Channel4_BASE,DMA2_Channel4_IRQn},
    {DMA2_Channel5_BASE,DMA2_Channel5_IRQn},
    
    {SPI3_BASE,SPI3_IRQn},
    
    {UART4_BASE,UART4_IRQn},
    {UART5_BASE,UART5_IRQn},
    
    #endif
    
};
#define AIRCR_VECTKEY_MASK    ((uint32_t)0x05FA0000)

void nvic_priority_group_config(uint32_t NVIC_PriorityGroup)
{
  /* Check the parameters */
  assert_param(IS_NVIC_PRIORITY_GROUP(NVIC_PriorityGroup));
  
  /* Set the PRIGROUP[10:8] bits according to NVIC_PriorityGroup value */
  SCB->AIRCR = AIRCR_VECTKEY_MASK | NVIC_PriorityGroup;
}

static IRQn_Type dev_to_irqn(uint32_t dev,uint8_t index)
{
    int i;
    for(i = 0; i < sizeof(dev_to_IRQn_table)/sizeof(DevToIRQn_t); i++)
    {
        if(dev == dev_to_IRQn_table[i].dev)
            break;
    }
    return (IRQn_Type)dev_to_IRQn_table[i].irqn[index];
}






void nvic_irq_set_priority(uint32_t dev, uint8_t index , uint8_t PreemptionPriority,uint8_t SubPriority) 
{
    
    uint32_t tmppriority = 0x00, tmppre = 0x00, tmpsub = 0x0F;

    IRQn_Type irq_num = dev_to_irqn(dev,index);
    tmppriority = (0x700 - ((SCB->AIRCR) & (uint32_t)0x700))>> 0x08;
    tmppre = (0x4 - tmppriority);
    tmpsub = tmpsub >> tmppriority;

    tmppriority = (uint32_t)PreemptionPriority<< tmppre;
    tmppriority |=  SubPriority & tmpsub;
    tmppriority = tmppriority << 0x04;
        
    NVIC->IP[irq_num] = tmppriority;
}
void nvic_irq_set_priority(IRQn_Type irq_num, uint8_t PreemptionPriority,uint8_t SubPriority) 
{
    
    uint32_t tmppriority = 0x00, tmppre = 0x00, tmpsub = 0x0F;

    tmppriority = (0x700 - ((SCB->AIRCR) & (uint32_t)0x700))>> 0x08;
    tmppre = (0x4 - tmppriority);
    tmpsub = tmpsub >> tmppriority;

    tmppriority = (uint32_t)PreemptionPriority<< tmppre;
    tmppriority |=  SubPriority & tmpsub;
    tmppriority = tmppriority << 0x04;
        
    NVIC->IP[irq_num] = tmppriority;

}




void nvic_irq_enable(uint32_t dev,uint8_t index) 
{
    IRQn_Type irq_num = dev_to_irqn(dev,index);
    NVIC->ISER[irq_num / 32] = bit_shift(irq_num % 32);
}
void nvic_irq_enable(IRQn_Type irq_num) 
{
    NVIC->ISER[irq_num / 32] = bit_shift(irq_num % 32);
}





void nvic_irq_disable(uint32_t dev,uint8_t index) 
{
    IRQn_Type irq_num = dev_to_irqn(dev,index);
    NVIC->ICER[irq_num / 32] = bit_shift(irq_num % 32);
}
void nvic_irq_disable(IRQn_Type irq_num) 
{
    NVIC->ICER[irq_num / 32] = bit_shift(irq_num % 32);
}





