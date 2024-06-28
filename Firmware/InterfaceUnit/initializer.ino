/*
 * File:   initializer.ino
 * Author: GIBIC UdeA
 *
 * Created on June 26, 2024, 13:41 PM
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
    // Pinout setup
    pinMode(ACTION_BTN, INPUT_PULLUP);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(LUMINR, OUTPUT);
    pinMode(LUMING, OUTPUT);
    pinMode(LUMINB, OUTPUT);
    pinMode(ACTION_LED, OUTPUT);

    attachInterrupt(digitalPinToInterrupt(ACTION_BTN), actionButtonInterrupt, FALLING);

    // digitalWrite(STANDBY_LED, HIGH);
    digitalWrite(ACTION_LED, HIGH);
    digitalWrite(LUMING, LOW);
    digitalWrite(LUMINR, LOW);
    digitalWrite(LUMINB, LOW);
    // pinMode(LED, OUTPUT);
}

/****************************************************************************
 ***** Atencion a interrupcion por encoder **********************************
 ****************************************************************************/
// Task para probar perifericos
void task_Prueba(void *arg)
{

    while (1)
    {
        digitalWrite(ACTION_LED, HIGH);
        digitalWrite(LUMING, HIGH);
        digitalWrite(LUMINR, HIGH);
        digitalWrite(LUMINB, HIGH);
        // digitalWrite(BUZZER_PIN, HIGH);
        vTaskDelay(100 / portTICK_PERIOD_MS);

        digitalWrite(ACTION_LED, LOW);
        digitalWrite(LUMING, LOW);
        digitalWrite(LUMINR, LOW);
        digitalWrite(LUMINB, LOW);
        // digitalWrite(BUZZER_PIN, LOW);
        vTaskDelay(900 / portTICK_PERIOD_MS);
    }
}

/** ****************************************************************************
 ** ************ END OF THE CODE ***********************************************
 ** ****************************************************************************/