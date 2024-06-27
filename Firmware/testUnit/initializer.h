/*
 * File:   initializer.h
 * Author: GIBIC UdeA
 *
 * Created on July 4, 2020, 13:41 PM
 */

#ifndef INITIALIZER_H
#define INITIALIZER_H

#ifdef __cplusplus
extern "C"
{
#endif

/** ****************************************************************************
 ** ************ INCLUDES ******************************************************
 ** ****************************************************************************/
#include <Arduino.h>
#include <Esp.h>
#include <nvs_flash.h>

/** ****************************************************************************
 ** ************ DEFINES *******************************************************
 ** ****************************************************************************/
//********DEFINICIONES DE HARDWARE******
//********DEFINICION DE VERSION*********
#define VERSION_1_2 TRUE

// Definiciones para controlar el shiel DFRobot quad motor driver
#define EV_ESC_CAM 18 // out 1 // Valvula 3/2 de activacion de la camara
#define EV_INSPIRA 5  // out 2 // Valvula 3/2 de control de la via inspiratoria
#define EV_ESPIRA 4   // out 3 // Valvula 3/2 de control de presiones PCON y PEEP
#define EV_AUX4 12    // out 3 // Valvula 3/2 de control de presiones PCON y PEEP
#define EV_AUX5 13    // out 3 // Valvula 3/2 de control de presiones PCON y PEEP

// Definiciones para el manejo del ADC
#define ADC_PRESS_1 27 // Sensor de presion para la camara
#define ADC_PRESS_2 39 // Sensor de presion distal de paciente (bolsa)
#define ADC_PRESS_3 35 // Sensor de presion proximal de paciente
#define ADC_FLOW_1 36  // Sensor de flujo linea inspiratoria
#define ADC_FLOW_2 34  // Sensor de flujo linea espiratoria

// Variables de control del protocolo serial 2
#define RXD2 16
#define TXD2 17

#define AUX_RXD 23
#define AUX_TXD 25

// Definiciones de la maquina de estados
#define MAIN_MENU 0 // Definicion de main menu
#define SEAL_TEST 1 // Definicion de seal test
#define PRES_TEST 2 // Definicion de pres test
#define CURR_TEST 3 // Definicion de curr test
#define ENDO_TEST 4 // Definicion de finalizacion de test
#define IDLE_TEST 5 // Definicion de finalizacion de test

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
