/*
 * File:   serialCONT.ino
 * Author: GIBIC UdeA
 *
 * Created on July 4, 2020, 13:41 PM
 */

/** ****************************************************************************
 ** ************ INCLUDES ******************************************************
 ** ****************************************************************************/
#include "serialCONT.h"

/** ****************************************************************************
 ** ************ DEFINES *******************************************************
 ** ****************************************************************************/

/** ****************************************************************************
 ** ************ EXTERN VARIABLES **********************************************
 ** ****************************************************************************/

// variables de estado de ventilacion
extern byte stateMachine;

// banderas de cambio de valores
extern volatile uint8_t flagTest;
extern volatile uint8_t flagEstanquedad;
extern volatile uint8_t flagPresion;
extern volatile uint8_t flagCorriente;
extern volatile uint8_t flagEstanTest;
extern volatile uint8_t flagPresiTest;
extern volatile uint8_t flagCorriTest;

// Variables para iniciar impresion de datos en menu
extern volatile uint8_t flagAlreadyPrint;
extern volatile uint8_t fl_buzzer;

// Variables para menu actualizado
extern float estanquedad;
extern float presion;
extern float corriente;
extern int estan_test;
extern int presi_test;
extern int corri_test;

/** ****************************************************************************
 ** ************ VARIABLES *****************************************************
 ** ****************************************************************************/

/** ****************************************************************************
 ** ************ FUNCTIONS *****************************************************
 ** ****************************************************************************/
// envio de datos por serial para configuracion
void sendSerialData(void)
{
    String dataToSend = String(stateMachine) + ',' +
                        String(estan_test) + ',' +
                        String(presi_test) + ',' +
                        String(corri_test) + ';';
    Serial2.print(dataToSend);
    Serial.println(dataToSend);
}

// envio de datos por serial para Debug
void debugText(String msg)
{
    Serial.println(msg);
}

/* ***************************************************************************
 * **** Ejecucion de la rutina de comunicacion por serial ********************
 * ***************************************************************************/
// Function to receive data from serial communication
void task_Receive(void *pvParameters)
{
    // Clean Serial buffers
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    Serial.flush();
    Serial2.flush();

    while (1)
    {
        if (Serial2.available() > 5)
        {
            // if (Serial.available() > 5) { // solo para pruebas

            String dataIn = Serial2.readStringUntil(';');
            // String dataIn = Serial.readStringUntil(';'); // solo para pruebas
            int contComas = 0;
            for (int i = 0; i < dataIn.length(); i++)
            {
                if (dataIn[i] == ',')
                {
                    contComas++;
                }
            }
            String dataIn2[40];
            for (int i = 0; i < contComas + 1; i++)
            {
                dataIn2[i] = dataIn.substring(0, dataIn.indexOf(','));
                dataIn = dataIn.substring(dataIn.indexOf(',') + 1);
            }
            // cargue los datos aqui
            // para entero
            // contCiclos =dataIn2[0].toInt();
            // para float
            float new_estanquedad = dataIn2[0].toFloat();
            float new_presion = dataIn2[1].toFloat();
            float new_corriente = dataIn2[2].toFloat();
            float new_estan_test = dataIn2[3].toInt();
            float new_presi_test = dataIn2[4].toInt();
            float new_corri_test = dataIn2[5].toInt();

            // Comparacion de los nuevos valores
            if (estanquedad != new_estanquedad)
            {
                estanquedad = new_estanquedad;
                flagEstanquedad = true;
                flagAlreadyPrint = false;
            }
            if (presion != new_presion)
            {
                presion = new_presion;
                flagPresion = true;
                flagAlreadyPrint = false;
            }
            if (corriente != new_corriente)
            {
                corriente = new_corriente;
                flagCorriente = true;
                flagAlreadyPrint = false;
            }
            if (estan_test != new_estan_test)
            {
                estan_test = new_estan_test;
                flagEstanTest = false;
                flagAlreadyPrint = false;
                fl_buzzer = true;
            }
            if (presi_test != new_presi_test)
            {
                presi_test = new_presi_test;
                flagPresiTest = false;
                flagAlreadyPrint = false;
                fl_buzzer = true;
            }
            if (corri_test != new_corri_test)
            {
                corri_test = new_corri_test;
                flagCorriTest = false;
                flagAlreadyPrint = false;
                fl_buzzer = true;
            }
            Serial2.flush();
            // Serial.flush(); // solo para pruebas
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    //   vTaskDelete(NULL);
}

/** ****************************************************************************
 ** ************ END OF THE CODE ***********************************************
 ** ****************************************************************************/
