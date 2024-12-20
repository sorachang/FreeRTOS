#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <conio.h>
#include <string.h>
void vGlobal(void *pvParameters);
void volumeTask(void *pvParameters);
void bootTask(void *pvParameters);
void RadioTask(void *pvParameters);
void BluetoothTask(void *pvParameters);
void USBTask(void *pvParameters);
boolean flag=TRUE;
char index=0;
TaskHandle_t radiohandle,bluetoothhandle,USBhandle,boothandle;
QueueHandle_t queuehandle,Qvolume;
static int mode=0;
int main(void)
{
    queuehandle=xQueueCreate(20,sizeof(uint32_t)); //mandatory
    Qvolume=xQueueCreate(20,sizeof(uint32_t));
    xTaskCreate(&bootTask,"Main",1024,NULL,1,&boothandle);
    xTaskCreate(&volumeTask,"Volume",1024,NULL,1,NULL);
    xTaskCreate(&vGlobal,"Global Task",1024,NULL,configMAX_PRIORITIES-1,NULL);
    xTaskCreate(&RadioTask, "RADIO", 1024, NULL, 1, &radiohandle);
    xTaskCreate(&BluetoothTask, "BLUETOOTH", 1024, NULL, 1, &bluetoothhandle);
    xTaskCreate(&USBTask, "USB", 1024, NULL, 1, &USBhandle);
    vTaskSuspend(radiohandle);
    vTaskSuspend(bluetoothhandle);
    vTaskSuspend(USBhandle);
    vTaskStartScheduler();

    return 0;
}
void vGlobal(void *pvParameters)
{
    char index=0;
    for(;;)
    {
        if(_kbhit())
        {
            index=_getch();
            //printf("Task Global %c\r\n",index);
            if(index=='0')
            {
                char *buf=(char*)calloc(1,2048);
                vTaskList(buf);
                printf("%s\r\n",buf); // task_name,task_state,priority,stack,task_num
                free(buf);
            }
            else if(index=='w' || index=='s' || index=='a' || index=='d')
                xQueueSend(Qvolume,&index,0);
            else
                xQueueSend(queuehandle,&index,0);
            index=0;
            
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
void volumeTask(void *pvParameters)
{
    char index=0;
    int volume=0;
    for(;;)
    {
        if(xQueueReceive(Qvolume,&index,100/portTICK_RATE_MS)==pdPASS)
        {
            switch (index)
            {
            case 'w':
                volume+=1;
                if(volume>40)
                    volume=40;
                printf("Volume %d\r\n",volume);
                break;
            case 's':
                volume-=1;
                if(volume<0)
                    volume=0;
                printf("Volume %d\r\n",volume);
                break;
            case 'd':
                volume+=5;
                if(volume>40)
                    volume=40;
                printf("Volume %d\r\n",volume);
                break;
            case 'a':
                volume-=5;
                if(volume<0)
                    volume=0;
                printf("Volume %d\r\n",volume);
                break;
            default:
                break;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
void bootTask(void *pvParameters)
{
    char index=0;
    for(;;)
    {
        if(xQueueReceive(queuehandle,&index,100/portTICK_RATE_MS)==pdPASS)
        {
            if(index=='1')
            {
            
                printf("Power on\r\n");
                        //vTaskResume(xhandle2);
                        //vTaskResume(xhandle3);
                switch(mode)
                {
                    case 1:
                        vTaskResume(radiohandle);
                        break;
                    case 2:
                        vTaskResume(bluetoothhandle);
                        break;
                    case 3:
                        vTaskResume(USBhandle);
                        break;
                    default:
                        vTaskResume(radiohandle);
                        break;
                }
                vTaskSuspend(boothandle);
            }

        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
void RadioTask(void *pvParameters)
{
    char index=0;
    for (;;)
    {
        if(flag==TRUE)
        {
            printf("RADIO\r\n");
            mode=1;
            flag=FALSE;
        }
        /*
        char q=0;
        printf("Task 1\r\n");
        //vTask3("Task 3");
        //if(_kbhit())
        if(xQueueReceive(queuehandle1,&q,100/portTICK_RATE_MS)==pdPASS)
        {
            switch (q)
            {
                case '2':
                    printf("Task 1-2\r\n");
                    break;
            
                default:
                    break;
            }
            q=0;
        }*/
        if(xQueueReceive(queuehandle,&index,0)==pdPASS)
        {  
            if(index=='1')
            {
                flag=TRUE;
                printf("Power off\r\n");
                vTaskResume(boothandle);
                vTaskSuspend(radiohandle);
            }
            else if(index=='2')
            {
                flag=TRUE;
                vTaskResume(bluetoothhandle);
                vTaskSuspend(radiohandle);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100)); // blocked, release cpu usage to others
    }
}

void BluetoothTask(void *pvParameters)
{

    char index=0;
    for (;;)
    {
        if(flag==TRUE)
        {
            printf("Bluetooth\r\n");
            mode=2;
            flag=FALSE; 
        }
        if(xQueueReceive(queuehandle,&index,100/portTICK_RATE_MS)==pdPASS)
        {  
            if(index=='1')
            {
                flag=TRUE;
                printf("Power off\r\n");
                vTaskResume(boothandle);
                vTaskSuspend(bluetoothhandle);
            }
            else if(index=='2')
            {
                flag=TRUE;
                vTaskResume(USBhandle);
                vTaskSuspend(bluetoothhandle);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100)); // blocked, release cpu usage to others
    
        /*
        printf("Task 2\r\n");
        //printf("%d\r\n",count);
        xQueueSend(queuehandle,&count,0);
        vTaskDelay(pdMS_TO_TICKS(1000));
        flag=TRUE;
        */
    }
}

void USBTask(void *pvParameters)
{

    char index=0;
    for (;;)
    {    
        if(flag==TRUE)
        {
            printf("USB\r\n");
            mode=3;
            flag=FALSE;    
        }
        if(xQueueReceive(queuehandle,&index,100/portTICK_RATE_MS)==pdPASS)
        {  
            if(index=='1')
            {
                flag=TRUE;
                printf("Power off\r\n");
                vTaskResume(boothandle);
                vTaskSuspend(USBhandle);
            }
            else if(index=='2')
            {
                flag=TRUE;
                vTaskResume(radiohandle);
                vTaskSuspend(USBhandle);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100)); // blocked, release cpu usage to others
        /*
        //char* c;
        uint32_t count=0;
        //c=malloc(10);
        //gets(c);
        printf("Task 3 \r\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
        //if(strcmp(c,"c")==0)
        if(xQueueReceive(queuehandle,&count,100/portTICK_RATE_MS)==pdPASS && count==20)
        {
            vTaskResume(xhandle1);
        }*/
    }
}