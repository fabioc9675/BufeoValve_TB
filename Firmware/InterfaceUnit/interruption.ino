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
        if (stateMachine == MAIN_MENU && contAction > 500 && digitalRead(ACTION_BTN) == 0)
        {
            portENTER_CRITICAL(&mux);
            attachInterrupt(digitalPinToInterrupt(ACTION_BTN), actionButtonInterrupt, FALLING);
            flagActionInterrupt = false;
            portEXIT_CRITICAL(&mux);
            contAction = 0;

            Serial.println("ACT Interrupt main");
            // Serial.println("I am on Cycling state");
            digitalWrite(ACTION_LED, LOW);

            // sendSerialData();

            stateMachine = SEAL_TEST;
            flagAlreadyPrint = false;
            stateString = "SEAL TEST";

            Serial.println(stateString);
        }
        else if (stateMachine != MAIN_MENU)
        {
            // Serial.println(digitalRead(STANDBY));
            if (contAction < 3000 && digitalRead(ACTION_BTN) == 1)
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

                default:
                    stateMachine = MAIN_MENU;
                    flagAlreadyPrint = false;
                    stateString = "MAIN MENU";
                    break;
                }
                Serial.println(stateString);
            }
            else if (contAction > 3000)
            {
                contAction = 0;
                stateMachine = MAIN_MENU;
                digitalWrite(ACTION_LED, HIGH);
                portENTER_CRITICAL_ISR(&mux);
                attachInterrupt(digitalPinToInterrupt(ACTION_BTN), actionButtonInterrupt, FALLING);
                flagActionInterrupt = false;
                portEXIT_CRITICAL_ISR(&mux);

                stateMachine = MAIN_MENU;
                flagAlreadyPrint = false;
                stateString = "MAIN MENU";

                Serial.println(stateString);

                // sendSerialData();
                // Serial.println("I am on Standby state");
            }
        }
    }
}

/** ****************************************************************************
 ** ************ END OF THE CODE ***********************************************
 ** ****************************************************************************/
