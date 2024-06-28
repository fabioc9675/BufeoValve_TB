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
extern volatile uint8_t fl_toggle;
extern volatile uint8_t fl_buzzer;

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
            fl_buzzer = true;
            portEXIT_CRITICAL(&mux);
            contAction = 0;

            Serial.println("ACT Interrupt main");
            // Serial.println("I am on Cycling state");
            // digitalWrite(ACTION_LED, LOW);

            // sendSerialData();

            stateMachine = SEAL_TEST;
            flagAlreadyPrint = false;
            stateString = "Seal test";

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
                fl_buzzer = true;
                portEXIT_CRITICAL_ISR(&mux);

                switch (stateMachine)
                {
                case SEAL_TEST:
                    if (estan_test == PASS_TEST && presi_test == PASS_TEST && corri_test == PASS_TEST)
                    {
                        stateMachine = ENDO_TEST;
                        stateString = "Fin  test";
                    }
                    else
                    {
                        if (presi_test != PASS_TEST)
                        {
                            stateMachine = PRES_TEST;
                            stateString = "Pres test";
                        }
                        else if (corri_test != PASS_TEST)
                        {
                            stateMachine = CURR_TEST;
                            stateString = "Curr test";
                        }
                    }
                    flagAlreadyPrint = false;
                    break;

                case PRES_TEST:
                    if (estan_test == PASS_TEST && presi_test == PASS_TEST && corri_test == PASS_TEST)
                    {
                        stateMachine = ENDO_TEST;
                        stateString = "Fin  test";
                    }
                    else
                    {
                        if (corri_test != PASS_TEST)
                        {
                            stateMachine = CURR_TEST;
                            stateString = "Curr test";
                        }
                        else if (estan_test != PASS_TEST)
                        {
                            stateMachine = SEAL_TEST;
                            stateString = "Seal test";
                        }
                    }
                    flagAlreadyPrint = false;
                    break;

                case CURR_TEST:
                    if (estan_test == PASS_TEST && presi_test == PASS_TEST && corri_test == PASS_TEST)
                    {
                        stateMachine = ENDO_TEST;
                        stateString = "Fin  test";
                    }
                    else
                    {
                        if (estan_test != PASS_TEST)
                        {
                            stateMachine = SEAL_TEST;
                            stateString = "Seal test";
                        }
                        else if (presi_test != PASS_TEST)
                        {
                            stateMachine = PRES_TEST;
                            stateString = "Pres test";
                        }
                    }
                    flagAlreadyPrint = false;
                    break;

                case ENDO_TEST:
                    stateMachine = MAIN_MENU;
                    flagAlreadyPrint = false;
                    stateString = "Main menu";
                    break;

                default:
                    stateMachine = MAIN_MENU;
                    flagAlreadyPrint = false;
                    stateString = "Main menu";
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
                fl_buzzer = true;
                portEXIT_CRITICAL_ISR(&mux);

                // if (stateMachine == SEAL_TEST || stateMachine == PRES_TEST || stateMachine == CURR_TEST)
                // {
                stateMachine = ENDO_TEST;
                flagAlreadyPrint = false;
                stateString = "Fin  test";
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
        // Evaluacion de estados de las pruebas
        if (estan_test == WAIT_TEST && presi_test == WAIT_TEST && corri_test == WAIT_TEST)
        {
            digitalWrite(LUMINB, LOW);
            digitalWrite(LUMING, LOW);
            digitalWrite(LUMINR, LOW);
        }
        else if (estan_test == FAIL_TEST || presi_test == FAIL_TEST || corri_test == FAIL_TEST)
        {
            // digitalWrite(LUMINB, LOW);
            digitalWrite(LUMINR, HIGH);
            digitalWrite(LUMING, LOW);
        }
        else if (estan_test == PASS_TEST && presi_test == PASS_TEST && corri_test == PASS_TEST)
        {
            digitalWrite(LUMINB, LOW);
            digitalWrite(LUMINR, LOW);
            digitalWrite(LUMING, HIGH);
        }
        else if (estan_test != FAIL_TEST || presi_test != FAIL_TEST || corri_test != FAIL_TEST)
        {
            // digitalWrite(LUMINB, LOW);
            digitalWrite(LUMINR, LOW);
            // digitalWrite(LUMING, LOW);
        }

        // evaluacion del tipo de prueba
        if (!(stateMachine == MAIN_MENU || stateMachine == ENDO_TEST))
        {
            if (fl_toggle)
            {
                fl_toggle = false;
                digitalWrite(ACTION_LED, !digitalRead(ACTION_LED));
                if (estan_test == BUSY_TEST || presi_test == BUSY_TEST || corri_test == BUSY_TEST)
                {
                    digitalWrite(LUMINB, digitalRead(ACTION_LED));
                    digitalWrite(LUMING, LOW);
                }
            }
        }
        else
        {
            digitalWrite(ACTION_LED, HIGH);
        }

        // vTaskDelay(100/portTICK_PERIOD_MS);
    }
}

void task_Buzzer(void *pvParameters)
{
    digitalWrite(BUZZER_PIN, LOW);
    while (true)
    {
        if (fl_buzzer == true)
        {
            fl_buzzer = false;
            digitalWrite(BUZZER_PIN, HIGH);
            vTaskDelay(50 / portTICK_PERIOD_MS);
            digitalWrite(BUZZER_PIN, LOW);
        }
    }
}

/** ****************************************************************************
 ** ************ END OF THE CODE ***********************************************
 ** ****************************************************************************/
