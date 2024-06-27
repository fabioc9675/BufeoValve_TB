/*
 * File:   lcdConfig.ino
 * Author: GIBIC UdeA
 *
 * Created on June 26, 2024, 13:41 PM
 */

/** ****************************************************************************
 ** ************ INCLUDES ******************************************************
 ** ****************************************************************************/
#include "lcdConfig.h"

/** ****************************************************************************
 ** ************ DEFINES *******************************************************
 ** ****************************************************************************/

/** ****************************************************************************
 ** ************ EXTERN VARIABLES **********************************************
 ** ****************************************************************************/
// **********************************************************

// definicion de lcd class
extern LiquidCrystal_I2C lcd;

extern volatile uint8_t flagAlreadyPrint;

// banderas de cambio de valores
extern volatile uint8_t flagTest;
extern volatile uint8_t flagEstanquedad;
extern volatile uint8_t flagPresion;
extern volatile uint8_t flagCorriente;
extern volatile uint8_t flagEstanTest;
extern volatile uint8_t flagPresiTest;
extern volatile uint8_t flagCorriTest;

// Variables para menu actualizado
extern float estanquedad;
extern float presion;
extern float corriente;
extern int estan_test;
extern int presi_test;
extern int corri_test;

// variables de estado de ventilacion
extern byte stateMachine;
extern String stateString;

/** ****************************************************************************
 ** ************ VARIABLES *****************************************************
 ** ****************************************************************************/
// // variables contador para verificacion de alertas

/** ****************************************************************************
 ** ************ FUNCTIONS *****************************************************
 ** ****************************************************************************/
// Configuracion del LCD
void lcd_setup(void)
{
    lcd.begin(21, 22);
    lcd.backlight();
    lcd.clear();
}

/* ***************************************************************************
 * **** Ejecucion de la rutina de refrescado de Display ++********************
 * ***************************************************************************/
void task_display(void *pvParameters)
{
    String taskMessage = "   BUFEO TestBench  ";
    // taskMessage = taskMessage + xPortGetCoreID();
    // Serial.println(taskMessage);

    lcd_setup(); // inicializacion de LCD
    lcd.setCursor(0, 0);
    lcd.print(taskMessage);

    // impresion de interface
    lcd.setCursor(0, 1);
    lcd.print("SEAL:     |CUR:     ");
    lcd.setCursor(0, 2);
    lcd.print("PRES:     |RES:     ");
    lcd.setCursor(0, 3);
    lcd.print("PRUEBA:             ");

    while (true)
    {
        /* ****************************************************************
         * **** Actualizacion de valores en pantalla LCD ******************
         * ***************************************************************/

        if (flagAlreadyPrint == false)
        {
            flagAlreadyPrint = true;
            // Evaluacion del estado del test de estanquedad
            if (flagEstanTest == false)
            {
                flagEstanTest = true;
                lcd.setCursor(5, 1);
                switch (estan_test)
                {
                case WAIT_TEST:
                    lcd.print("WAIT");
                    break;
                case FAIL_TEST:
                    lcd.print("FAIL");
                    break;
                case BUSY_TEST:
                    lcd.print("BUSY");
                    break;
                case PASS_TEST:
                    lcd.print("PASS");
                    break;
                default:
                    break;
                }
            }

            // Evaluacion del estado del test de presion
            if (flagPresiTest == false)
            {
                flagPresiTest = true;
                lcd.setCursor(5, 2);
                switch (presi_test)
                {
                case WAIT_TEST:
                    lcd.print("WAIT");
                    break;
                case FAIL_TEST:
                    lcd.print("FAIL");
                    break;
                case BUSY_TEST:
                    lcd.print("BUSY");
                    break;
                case PASS_TEST:
                    lcd.print("PASS");
                    break;
                default:
                    break;
                }
            }

            // Evaluacion del estado del test de corriente
            if (flagCorriTest == false)
            {
                flagCorriTest = true;
                lcd.setCursor(15, 1);
                switch (corri_test)
                {
                case WAIT_TEST:
                    lcd.print("WAIT");
                    break;
                case FAIL_TEST:
                    lcd.print("FAIL");
                    break;
                case BUSY_TEST:
                    lcd.print("BUSY");
                    break;
                case PASS_TEST:
                    lcd.print("PASS");
                    break;
                default:
                    break;
                }
            }

            lcd.setCursor(8, 3);
            lcd.print(stateString);
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

/** ****************************************************************************
 ** ************ END OF THE CODE ***********************************************
 ** ****************************************************************************/