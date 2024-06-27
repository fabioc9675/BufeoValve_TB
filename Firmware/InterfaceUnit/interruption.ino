/*
 * File:   interruption.ino
 * Author: GIBIC UdeA
 *
 * Created on June 26, 2024, 13:41 PM
 */

/** ****************************************************************************
 ** ************ INCLUDES ******************************************************
 ** ****************************************************************************/
#include "interruption.h"

/** ****************************************************************************
 ** ************ DEFINES *******************************************************
 ** ****************************************************************************/

/** ****************************************************************************
 ** ************ EXTERN VARIABLES **********************************************
 ** ****************************************************************************/
// **********************************************************
extern portMUX_TYPE mux;

// banderas de botones de usuario
extern volatile uint8_t flagActionInterrupt;

// variables de estado de ventilacion
extern byte stateMachine;

extern unsigned int contAction;

// Variables para imprimir en pantalla
extern String stateString;
extern volatile uint8_t flagAlreadyPrint;

// Variables para menu actualizado
extern float estanquedad;
extern float presion;
extern float corriente;
extern int estan_test;
extern int presi_test;
extern int corri_test;

// banderas de cambio de valores
extern volatile uint8_t flagTest;
extern volatile uint8_t flagEstanquedad;
extern volatile uint8_t flagPresion;
extern volatile uint8_t flagCorriente;
extern volatile uint8_t flagEstanTest;
extern volatile uint8_t flagPresiTest;
extern volatile uint8_t flagCorriTest;

/** ****************************************************************************
 ** ************ VARIABLES *****************************************************
 ** ****************************************************************************/
// Variable de estado del encoder

/** ****************************************************************************
 ** ************ FUNCTIONS *****************************************************
 ** ****************************************************************************/
// Interrupcion por boton accion
void IRAM_ATTR actionButtonInterrupt(void)
{
    portENTER_CRITICAL_ISR(&mux);
    flagActionInterrupt = true;
    detachInterrupt(digitalPinToInterrupt(ACTION_BTN));
    portEXIT_CRITICAL_ISR(&mux);
}

// Atencion a interrupcion por boton de accion
void actionInterruptAttention(void)
{
    if (flagActionInterrupt)
    {
        contAction++;
        if (stateMachine == MAIN_MENU && contAction > 500 && digitalRead(ACTION_BTN) == 1)
        {
            portENTER_CRITICAL(&mux);
            attachInterrupt(digitalPinToInterrupt(ACTION_BTN), actionButtonInterrupt, FALLING);
            flagActionInterrupt = false;
            portEXIT_CRITICAL(&mux);
            contAction = 0;

            Serial.println("ACT Interrupt main");
            // Serial.println("I am on Cycling state");
            // digitalWrite(ACTION_LED, LOW);

            // sendSerialData();

            stateMachine = SEAL_TEST;
            flagAlreadyPrint = false;
            stateString = "SEAL TEST";

            // Envio de datos hacia el micro control
            sendSerialData();
            debugText(stateString);
        }
        else if (stateMachine != MAIN_MENU)
        {
            // Serial.println(digitalRead(STANDBY));
            if (contAction > 500 && contAction < 3000 && digitalRead(ACTION_BTN) == 1)
            {
                contAction = 0;
                portENTER_CRITICAL_ISR(&mux);
                attachInterrupt(digitalPinToInterrupt(ACTION_BTN), actionButtonInterrupt, FALLING);
                flagActionInterrupt = false;
                portEXIT_CRITICAL_ISR(&mux);

                switch (stateMachine)
                {
                case SEAL_TEST:
                    stateMachine = PRES_TEST;
                    flagAlreadyPrint = false;
                    stateString = "PRES TEST";
                    break;

                case PRES_TEST:
                    stateMachine = CURR_TEST;
                    flagAlreadyPrint = false;
                    stateString = "CURR TEST";
                    break;

                case CURR_TEST:
                    stateMachine = SEAL_TEST;
                    flagAlreadyPrint = false;
                    stateString = "SEAL TEST";
                    break;

                case ENDO_TEST:
                    stateMachine = MAIN_MENU;
                    flagAlreadyPrint = false;
                    stateString = "MAIN MENU";
                    break;

                default:
                    stateMachine = MAIN_MENU;
                    flagAlreadyPrint = false;
                    stateString = "MAIN MENU";
                    break;
                }

                // Envio de datos hacia el micro control
                sendSerialData();
                debugText(stateString);
            }
            else if (contAction > 3000)
            {
                contAction = 0;
                stateMachine = MAIN_MENU;
                // digitalWrite(ACTION_LED, HIGH);
                portENTER_CRITICAL_ISR(&mux);
                attachInterrupt(digitalPinToInterrupt(ACTION_BTN), actionButtonInterrupt, FALLING);
                flagActionInterrupt = false;
                portEXIT_CRITICAL_ISR(&mux);

                // if (stateMachine == SEAL_TEST || stateMachine == PRES_TEST || stateMachine == CURR_TEST)
                // {
                stateMachine = ENDO_TEST;
                flagAlreadyPrint = false;
                stateString = "FIN  TEST";
                // }
                // else if (stateMachine == ENDO_TEST)
                // {
                //     stateMachine = MAIN_MENU;
                //     flagAlreadyPrint = false;
                //     stateString = "MAIN MENU";
                // }

                // Envio de datos hacia el micro control
                sendSerialData();
                debugText(stateString);
            }
        }
    }
}

/* ***************************************************************************
 * **** Ejecucion de la rutina de control de LEDs ****************************
 * ***************************************************************************/
void task_Indicators(void *pvParameters)
{

    while (true)
    {
        switch (stateMachine)
        {
        case SEAL_TEST:
            stateMachine = PRES_TEST;
            flagAlreadyPrint = false;
            stateString = "PRES TEST";
            break;

        case PRES_TEST:
            stateMachine = CURR_TEST;
            flagAlreadyPrint = false;
            stateString = "CURR TEST";
            break;

        case CURR_TEST:
            stateMachine = SEAL_TEST;
            flagAlreadyPrint = false;
            stateString = "SEAL TEST";
            break;

        case ENDO_TEST:
            stateMachine = MAIN_MENU;
            flagAlreadyPrint = false;
            stateString = "MAIN MENU";
            break;

        default:
            stateMachine = MAIN_MENU;
            flagAlreadyPrint = false;
            stateString = "MAIN MENU";
            break;
        }
    }
}

/** ****************************************************************************
 ** ************ END OF THE CODE ***********************************************
 ** ****************************************************************************/
