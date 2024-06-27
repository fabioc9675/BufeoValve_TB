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
extern volatile uint8_t flagEtanquedad;
extern volatile uint8_t flagPresion;
extern volatile uint8_t flagCorriente;

// variables de estado de ventilacion
extern byte stateMachine;

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

// Ejecucion de la rutina de refrescamiento del LCD
void lcdPrintFirstLine(void)
{
    // lcd.setCursor(0, 0);
    // switch (lineaAlerta)
    // {
    // case MAIN_MENU:
    //     if (stateMachine == STANDBY_STATE)
    //     {
    //         lcd.print("  Banco de pruebas  ");
    //     }
    //     // else
    //     // {
    //     //     lcd.print("  GIBIC Neuma ");
    //     //     if (currentVentilationMode == 0)
    //     //     {
    //     //         lcd.print("P-CMV ");
    //     //     }
    //     //     else if (currentVentilationMode == 1)
    //     //     {
    //     //         lcd.print("A/C   ");
    //     //     }
    //     //     else
    //     //     {
    //     //         lcd.print("CPAP  ");
    //     //     }
    //     // }
    //     break;
    // case CONFIG_MENU:
    //     if (currentVentilationMode == 0)
    //     {
    //         lcd.print(" Configuracion P-CMV");
    //     }
    //     else if (currentVentilationMode == 1)
    //     {
    //         lcd.print("  Configuracion A/C ");
    //     }
    //     else
    //     {
    //         lcd.print(" Configuracion CPAP ");
    //     }
    //     break;
    // case CONFIG_ALARM:
    //     lcd.print("      Alarmas       ");
    //     break;
    // case VENT_MENU:
    //     lcd.print(" Modo Ventilatorio  ");
    //     if (insideMenuFlag == true && optionVentMenu == 0)
    //     {
    //         lcd.setCursor(0, 0);
    //         lcd.write(126);
    //     }
    //     break;
    // case SERVICE_MENU:
    //     lcd.print("  Chequeo de fugas  ");
    //     break;
    // case ALE_PRES_PIP:
    //     lcd.print("Presion PIP elevada ");
    //     break;
    // case ALE_PRES_DES:
    //     lcd.print("Desconexion Paciente");
    //     break;
    // case ALE_OBSTRUCCION:
    //     lcd.print("    Obstruccion     ");
    //     break;
    // case ALE_GENERAL:
    //     lcd.print("   Fallo general   ");
    //     break;
    // case ALE_PRES_PEEP:
    //     lcd.print("  Perdida de PEEP   ");
    //     break;
    // case BATTERY:
    //     lcd.print(" Fallo red electrica");
    //     break;
    // case ALE_BATTERY_10MIN:
    //     lcd.print("Bateria baja 10 Min");
    //     break;
    // case ALE_BATTERY_5MIN:
    //     lcd.print(" Bateria baja 5 Min ");
    //     break;
    // case ALE_FR_ALTA:
    //     lcd.print("      FR alta       ");
    //     break;
    // case ALE_VE_ALTO:
    //     lcd.print("    Vol/min alto    ");
    //     break;
    // case ALE_APNEA:
    //     lcd.print("       Apnea        ");
    //     break;
    // case CHECK_MENU:
    //     lcd.print("Comprobacion Inicial");
    //     break;
    // case CONFIRM_MENU:
    //     lcd.print(" Confirmar cambios  ");
    //     break;
    // case CPAP_MENU:
    //     lcd.print(" Configuracion CPAP ");
    //     break;
    // default:
    //     break;
    // }
}

/* ***************************************************************************
 * **** Ejecucion de la rutina de refrescado de Display ++********************
 * ***************************************************************************/
void task_display(void *pvParameters)
{
    String taskMessage = "LCD Task one running on core ";
    taskMessage = taskMessage + xPortGetCoreID();
    // Serial.println(taskMessage);

    lcd_setup(); // inicializacion de LCD
    lcd.setCursor(0, 0);
    lcd.print(taskMessage);

    while (true)
    {
        /* ****************************************************************
         * **** Actualizacion de valores en pantalla LCD ******************
         * ***************************************************************/
        // if (flagService == false)
        // {
        // if ((menuAnterior != menuImprimir) && (flagAlreadyPrint == false))
        // {
        //     lcd.clear();
        //     lineaAnterior = MODE_CHANGE;
        // lcd_show_comp();
        // }
        // else
        // {
        //     lcd_show_part();
        // }
        // }
        // delay de 100 ms en la escritura del LCD

        // lcd.setCursor(2, 3);
        // lcd.blink_on();
        // vTaskDelay(1000 / portTICK_PERIOD_MS);
        // lcd.blink_off();
        // vTaskDelay(2000 / portTICK_PERIOD_MS);

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

/** ****************************************************************************
 ** ************ END OF THE CODE ***********************************************
 ** ****************************************************************************/