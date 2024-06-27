/*
 * File:   InterfaceUnit.h
 * Author: GIBIC UdeA
 *
 * Created on June 26, 2024, 13:41 PM
 */

/** ****************************************************************************
 ** ************ INCLUDES ******************************************************
 ** ****************************************************************************/
#include <Arduino.h>
#include <Esp.h>
#include <nvs_flash.h>
#include <stdio.h>

#include "LiquidCrystal_I2C.h"
#include "initializer.h"
#include "lcdConfig.h"
#include "timer.h"

/** ****************************************************************************
 ** ************ DEFINES *******************************************************
 ** ****************************************************************************/

//********DEFINICIONES CONDICIONES******
#define TRUE 1
#define FALSE 0

/** ****************************************************************************
 ** ************ EXTERN VARIABLES **********************************************
 ** ****************************************************************************/
// definiciones para el timer
extern hw_timer_t *timer;
extern portMUX_TYPE timerMux;
extern portMUX_TYPE mux;

/** ****************************************************************************
 ** ************ VARIABLES *****************************************************
 ** ****************************************************************************/
// LiquidCrystal_I2C lcd(0x27, 20, 4);
LiquidCrystal_I2C lcd(0x3F, 20, 4);

// definicion de los core para ejecucion
uint8_t taskCoreZero = 0;
uint8_t taskCoreOne = 1;

// manejadores para los semaforos binarios
SemaphoreHandle_t xSemaphoreTimer = NULL;

// banderas de botones de usuario
volatile uint8_t flagActionInterrupt = false; // inicia en modo standby

// bandera de activacion de timer
volatile uint8_t flagTimerInterrupt = false;

// banderas de cambio de valores
volatile uint8_t flagTest = false;
volatile uint8_t flagEstanquedad = false;
volatile uint8_t flagPresion = false;
volatile uint8_t flagCorriente = false;
volatile uint8_t flagEstanTest = false;
volatile uint8_t flagPresiTest = false;
volatile uint8_t flagCorriTest = false;

float estanquedad = 0;
float presion = 0;
float corriente = 0;
int estan_test = WAIT_TEST;
int presi_test = WAIT_TEST;
int corri_test = WAIT_TEST;

// Variable de la maquina de estados
byte stateMachine = MAIN_MENU;
String stateString = "Main Menu";

// Variables para el manejo de los mensajes en pantalla
volatile uint8_t flagAlreadyPrint = false;

// creo el manejador para el semaforo como variable global

// xQueueHandle timer_queue = NULL;

/** ****************************************************************************
 ** ************ FUNCTIONS *****************************************************
 ** ****************************************************************************/

/* ***************************************************************************
 * **** CONFIGURACION ********************************************************
 * ***************************************************************************/
void setup()
{
	init_GPIO();
	Serial.begin(115200);
	Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
	Serial2.setTimeout(10);

	vTaskDelay(100 / portTICK_PERIOD_MS);

	nvs_flash_init();

	// se crea el semaforo binario
	xSemaphoreTimer = xSemaphoreCreateBinary();
	init_Timer();

	// creo la tarea task_pulsador
	// xTaskCreatePinnedToCore(task_Prueba, "task_Prueba", 2048, NULL, 1, NULL, taskCoreOne);

	xTaskCreatePinnedToCore(task_timer, "task_timer", 2048, NULL, 2, NULL, taskCoreOne);
	xTaskCreatePinnedToCore(task_display, "task_display", 2048, NULL, 3, NULL, taskCoreOne); // se puede colocar en el core cero
	xTaskCreatePinnedToCore(task_Receive, "task_Receive", 2048, NULL, 1, NULL, taskCoreOne);

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
