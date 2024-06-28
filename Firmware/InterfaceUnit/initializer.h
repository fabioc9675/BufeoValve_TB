/*
 * File:   initializer.h
 * Author: GIBIC UdeA
 *
 * Created on June 26, 2024, 13:41 PM
 */

#ifndef INITIALIZER_H
#define INITIALIZER_H

#ifdef __cplusplus
extern "C"
{
#endif

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

/** ****************************************************************************
 ** ************ INCLUDES ******************************************************
 ** ****************************************************************************/
#include <Arduino.h>
#include <Esp.h>
#include <nvs_flash.h>

#include "interruption.h"

/** ****************************************************************************
 ** ************ DEFINES *******************************************************
 ** ****************************************************************************/
//********DEFINICIONES DE HARDWARE******
//********DEFINICION DE VERSION*********
#define VERSION_1_2 TRUE

    //********DEFINICION DE PINES***********
#define BUZZER_PIN 12
#define ACTION_BTN 34
#define ACTION_LED 25

#define LUMING 13 // Alarma luminosa
#define LUMINR 14
#define LUMINB 15

    // #define LED 2
#define ESP_INTR_FLAG_DEFAULT 0
#define LOW_ATT_INT 50 // Interrupcion cada 10 ms

// Variables de control del protocolo serial 2
#define RXD2 16
#define TXD2 17

// Definiciones de la maquina de estados
#define MAIN_MENU 0 // Definicion de main menu
#define SEAL_TEST 1 // Definicion de seal test
#define PRES_TEST 2 // Definicion de pres test
#define CURR_TEST 3 // Definicion de curr test
#define ENDO_TEST 4 // Definicion de finalizacion de test

// Definiciones de los resultados de la prueba
#define WAIT_TEST 0 // Estado de espera de test
#define FAIL_TEST 1 // Estado de falla de test
#define BUSY_TEST 2 // Estado de ocupado de test
#define PASS_TEST 3 // Estado de pasa de test

    /** ****************************************************************************
     ** ************ VARIABLES *****************************************************
     ** ****************************************************************************/

    /** ****************************************************************************
     ** ************ FUNCTIONS *****************************************************
     ** ****************************************************************************/
    void init_GPIO(void); // inicializacion de los pines del microcontrolador

    /** ****************************************************************************
     ** ************ TASKS *********************************************************
     ** ****************************************************************************/
    void task_Prueba(void *arg); // Task para probar perifericos

    /* *****************************************************************************
     * *****************************************************************************
     * ******************** USO DE MODULO ADS **************************************
     * *****************************************************************************
     * *****************************************************************************/

    /* *****************************************************************************
     * *****************************************************************************
     * ***************** PROTOTYPE DEFINITION **************************************
     * *****************************************************************************
     * *****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* INITIALIZER_H */
