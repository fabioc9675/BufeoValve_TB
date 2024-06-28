/*
 * File:   controlUnit.ino
 * Author: GIBIC UdeA
 *
 * Created on July 4, 2020, 13:41 PM
 */

/** ****************************************************************************
 ** ************ INCLUDES ******************************************************
 ** ****************************************************************************/
#include <Arduino.h>
#include <Esp.h>
#include <nvs_flash.h>
#include <stdio.h>

#include "initializer.h"
#include "serialHMI.h"

/** ****************************************************************************
 ** ************ DEFINES *******************************************************
 ** ****************************************************************************/

//********DEFINICIONES CONDICIONES******
#define TRUE 1
#define FALSE 0

/** ****************************************************************************
 ** ************ EXTERN VARIABLES **********************************************
 ** ****************************************************************************/
// variables de operacion de calibracion

/** ****************************************************************************
 ** ************ VARIABLES *****************************************************
 ** ****************************************************************************/
// definicion de los core para ejecucion
uint8_t taskCoreZero = 0;
uint8_t taskCoreOne = 1;

// manejadores de las tareas
xTaskHandle receiveTaskHandle;

// manejadores para los semaforos binarios
SemaphoreHandle_t xSemaphoreTimer = NULL;
SemaphoreHandle_t xSemaphoreAdc = NULL;

// bandera de activacion de timer
volatile uint8_t flagTimerInterrupt = false;

// Variables maquina de estados de modos ventilatorios
int stateMachine = MAIN_MENU;
int currentStateMachine = MAIN_MENU;
int newStateMachine = MAIN_MENU;

// banderas de cambio de valores
volatile uint8_t flagTest = false;
volatile uint8_t flagEstanquedad = false;
volatile uint8_t flagPresion = false;
volatile uint8_t flagCorriente = false;
volatile uint8_t flagEstanTest = false;
volatile uint8_t flagPresiTest = false;
volatile uint8_t flagCorriTest = false;

// variables de calculo de parametros ventilatorios
float estanquedad = 0;
float presion = 0;
float corriente = 0;
int estan_test = WAIT_TEST;
int presi_test = WAIT_TEST;
int corri_test = WAIT_TEST;

/* ***************************************************************************
 * **** CONFIGURACION ********************************************************
 * ***************************************************************************/
void setup()
{
    // init_Memory();
    init_GPIO();

    // init_TextPayload();
    Serial.begin(115200);
    Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
    Serial2.setTimeout(100);

    vTaskDelay(1000 / portTICK_PERIOD_MS);

    nvs_flash_init();

    // se crea el semaforo binario
    xSemaphoreTimer = xSemaphoreCreateBinary();
    xSemaphoreAdc = xSemaphoreCreateBinary();
    // init_Timer();

    // Tareas de operacion normal del ventilador
    // xTaskCreatePinnedToCore(task_Timer, "task_Timer", 2048, NULL, 7, NULL, taskCoreOne);
    // xTaskCreatePinnedToCore(task_Adc, "task_Adc", 4096, NULL, 4, NULL, taskCoreOne);
    xTaskCreatePinnedToCore(task_Receive, "task_Receive", 4096, NULL, 1, NULL, taskCoreOne);
    xTaskCreatePinnedToCore(task_sendSerialData, "task_sendSerialData", 4096, NULL, 1, NULL, taskCoreOne);
    // xTaskCreatePinnedToCore(task_Raspberry, "task_Raspberry", 4096, NULL, 4, NULL, taskCoreOne);

    // Clean Serial buffers
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}

/* ***************************************************************************
 * **** LOOP MAIN_MENU *******************************************************
 * ***************************************************************************/
void loop()
{
}

/* ***************************************************************************
 * **** FIN DEL PROGRAMA *****************************************************
 * ***************************************************************************/
