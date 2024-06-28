/*
 * File:   initializer.ino
 * Author: GIBIC UdeA
 *
 * Created on July 4, 2020, 13:41 PM
 */

/** ****************************************************************************
 ** ************ INCLUDES ******************************************************
 ** ****************************************************************************/
#include "initializer.h"

/** ****************************************************************************
 ** ************ DEFINES *******************************************************
 ** ****************************************************************************/

/** ****************************************************************************
 ** ************ EXTERN VARIABLES **********************************************
 ** ****************************************************************************/
// **********************************************************

/** ****************************************************************************
 ** ************ VARIABLES *****************************************************
 ** ****************************************************************************/

/** ****************************************************************************
 ** ************ FUNCTIONS *****************************************************
 ** ****************************************************************************/
// inicializacion de los pines del microcontrolador
void init_GPIO(void)
{
    // Configuracion de los pines de conexion con del driver para manejo de electrovalvulas

    // inicializacion de los pines controladores de las EV como salidas
    pinMode(EV_ESC_CAM, OUTPUT); // PIN 6   velocidad
    pinMode(EV_INSPIRA, OUTPUT); // PIN 3   velocidad
    pinMode(EV_ESPIRA, OUTPUT);  // PIN 12  velocidad
    pinMode(EV_AUX4, OUTPUT);    // PIN 3   velocidad
    pinMode(EV_AUX5, OUTPUT);    // PIN 6   velocidad

    pinMode(CURR_DETEC, INPUT); // PIN 6   velocidad

    // Inicializacion de los pines de ADC para conversion Analogo-digitalPinToInterrupt
    adcAttachPin(ADC_PRESS_1);
    adcAttachPin(ADC_PRESS_2);
    adcAttachPin(ADC_PRESS_3);
    adcAttachPin(ADC_FLOW_1);
    adcAttachPin(ADC_FLOW_2);

    // desactiva todas las salidas de electrovalvulas
    digitalWrite(EV_ESC_CAM, LOW); // PIN 3   velocidad
    digitalWrite(EV_INSPIRA, LOW); // PIN 6   velocidad
    digitalWrite(EV_ESPIRA, LOW);  // PIN 12  velocidad
    digitalWrite(EV_AUX4, LOW);    // PIN 3   velocidad
    digitalWrite(EV_AUX5, LOW);    // PIN 6   velocidad
}

/** ****************************************************************************
 ** ************ END OF THE CODE ***********************************************
 ** ****************************************************************************/