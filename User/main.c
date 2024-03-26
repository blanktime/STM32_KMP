#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "stm32f4xx.h"
#include "gpiof.h"
#include "delay.h"
#include "uart.h"

//#define rw_spor
//#define fix_spor
//#define random_spor
#define progress_spor
#define detect_word

volatile int keywordDetected = 0;

void u32tostr(uint32_t dat, uint8_t *str) // unsigned int to str
{
    int_least8_t temp[20];
    uint8_t i = 0, j = 0;
    i = 0;
    while (dat)
    {
        temp[i] = dat % 10 + 0x30;
        i++;
        dat /= 10;
    }
    j = i;
    for (i = 0; i < j; i++)
    {
        str[i] = temp[j - i - 1];
    }
    if (!i)
    {
        str[i++] = '0';
    }
    str[i] = 0;
}

void Usart_Printf(uint8_t *str)
{
    uint8_t data = 0;
    do
    {
        USART_SendData(USART1, str[data]); // send a byte
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
            ; // wait a byte finish sending
        data++;
    } while (str[data] != 0); // check whether data transmission is complete
}

int main(void)
{
    delay_init();
    Power_Pin();
    Led_Init();
    Key_Init();
    Out_Pin();

    Uart_Init(115200);
    
	uint32_t total = 60000;   //cycle
    uint8_t str[6];
	
#ifdef random_spor
    uint32_t seed = 0;
#endif
    
#ifdef progress_spor
    uint32_t time_ms = 10;
    uint32_t sum = 0;
#endif
    buildKMPTable(TARGET_KEYWORD);
    
    while (1)
    {
#ifdef random_spor
        srand(total);
        seed = rand() % 60 + 1;  //random number 1 - 60
#endif

#ifdef progress_spor        
        /* if(sum == 600){
            sum = 0;
            time_ms = 100;
        } */
		if(time_ms == 54900){
			time_ms = 0;
		}
#endif
		
#ifdef rw_spor
		delay_s(15);
#endif
		
#if defined(fix_spor) || defined(random_spor) || defined(progress_spor) 
		delay_ms(500);
#endif

        GPIO_ResetBits(GPIOA, GPIO_Pin_4); //1 start
        Usart_Printf("power start\n");

        delay_s(1);
        GPIO_ResetBits(GPIOA, GPIO_Pin_5); //2 start
        Usart_Printf("phone start\n");

        delay_s(5);                        //phone_power_on time
        GPIO_SetBits(GPIOA, GPIO_Pin_5);   //2 stop
        Usart_Printf("phone stop\n");
		 
/********** total_power_on time **********/
#ifdef rw_spor
		delay_s(60);
#endif

#ifdef random_spor
        delay_s(seed);                     
#endif

#ifdef progress_spor 
		delay_ms(time_ms);
#endif

#ifdef fix_spor
		delay_ms(500);
#endif
/******************************************/

// detect action
#ifdef detect_word
        if(keywordDetected){
            break;
        }
#endif
        
        GPIO_SetBits(GPIOA, GPIO_Pin_4);   //1 stop
        Usart_Printf("power stop\n");

#ifdef progress_spor 
		if(time_ms >= 10050 && time_ms < 30100){
			time_ms += 50;
		}
		else if(time_ms >= 30100){
			time_ms += 100;
		}
		else{
			time_ms += 10;
		}
        // sum++;
#endif

        total = total - 1;
        u32tostr(total, str);
        Usart_Printf("residue times: ");
        Usart_Printf(str);
        if(total == 0){
            break;
        }
    }

    Usart_Printf("Func end\n");
    return 0;
}

//void USART1_IRQHandler(void)
//{
//    static char keyword[18];
//    static uint8_t keywordIndex = 0;

//    if (USART1->SR & USART_SR_RXNE) {
//        char receivedData = USART1->DR;

//        // 如果进入中断处理函数时 keywordIndex 不为 0，则说明上一次检测中断时还未完全匹配关键字
//        if (keywordIndex > 0) {
//            if (receivedData == 'i' && keyword[keywordIndex - 1] == ' ') {
//                keywordIndex = 0;
//            }
//            keyword[keywordIndex++] = receivedData;

//            // 如果 keywordIndex 达到 17，表示关键字已经完全匹配
//            if (keywordIndex == 17) {
//                keyword[17] = '\0';

//                //Usart_Printf(keyword);

//                // 比较关键字
//                if (strstr(keyword, "inconsistent node") != NULL) {
//                    // 获取关键字
//                    // Usart_Printf("get\n");
//                    keywordDetected = 1;
//                }

//                // 重置索引
//                keywordIndex = 0;
//            }
//        } else if (receivedData == 'i') {
//            keyword[0] = 'i';
//            keywordIndex = 1;
//        }
//    }
//}


#define KEYWORD_LENGTH 17
#define TARGET_KEYWORD "inconsistent node"

void buildKMPTable(const char *pattern, int *kmpTable) {
    int len = 0;
    int i = 1;
    kmpTable[0] = 0;

    while (i < KEYWORD_LENGTH) {
        if (pattern[i] == pattern[len]) {
            len++;
            kmpTable[i] = len;
            i++; 
        } else {
            if (len != 0) {
                len = kmpTable[len - 1];
            } else {
                kmpTable[i] = 0;
                i++;
            }
        }
    }
}

void KMP(char receivedData, int *kmpTable, char *keyword, uint8_t *keywordIndex) {
    if (*keywordIndex > 0 && receivedData == TARGET_KEYWORD[*keywordIndex - 1] && keyword[*keywordIndex - 1] == ' ') {
        *keywordIndex = 0;
    }
    keyword[(*keywordIndex)++] = receivedData;

    if (*keywordIndex == KEYWORD_LENGTH) {
        keyword[KEYWORD_LENGTH] = '\0';

        // 使用KMP算法比较关键字
        int matchPosition = -1;
        for (int i = 0, j = 0; i < KEYWORD_LENGTH; ) {
            if (keyword[i] == TARGET_KEYWORD[j]) {
                i++;
                j++;
                if (j == KEYWORD_LENGTH) {
                    matchPosition = i - j;
                    break;
                }
            } else {
                if (j != 0) {
                    j = kmpTable[j - 1];
                } else {
                    i++;
                }
            }
        }

        // 检查关键字是否找到
        if (matchPosition != -1) {
            keywordDetected = 1;
        }

        // 重置索引
        *keywordIndex = 0;
    }
}

void USART1_IRQHandler(void) {
    static char keyword[KEYWORD_LENGTH + 1];
    static uint8_t keywordIndex = 0;
    static int kmpTable[KEYWORD_LENGTH];

    if (USART1->SR & USART_SR_RXNE) {
        char receivedData = USART1->DR;

        if (keywordIndex == 0 && receivedData == TARGET_KEYWORD[0]) {
            keyword[0] = receivedData;
            keywordIndex = 1;
        } else {
            buildKMPTable(keyword, kmpTable);
            KMP(receivedData, kmpTable, keyword, &keywordIndex);
        }
    }
}

