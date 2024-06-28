/*
 * File:   serialHMI.ino
 * Author: GIBIC UdeA
 *
 * Created on July 4, 2020, 13:41 PM
 */

/** ****************************************************************************
 ** ************ INCLUDES ******************************************************
 ** ****************************************************************************/
#include "serialHMI.h"

/** ****************************************************************************
 ** ************ DEFINES *******************************************************
 ** ****************************************************************************/

/** ****************************************************************************
 ** ************ EXTERN VARIABLES **********************************************
 ** ****************************************************************************/
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

// Variables para menu actualizado
int new_estan_test;
int new_presi_test;
int new_corri_test;

extern int newStateMachine;
extern int currentStateMachine;

float adcADQ[40];
float adcMean = 0;

/** ****************************************************************************
 ** ************ VARIABLES *****************************************************
 ** ****************************************************************************/

/** ****************************************************************************
 ** ************ FUNCTIONS *****************************************************
 ** ****************************************************************************/

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
            // if (Serial.available() > 5) {  // solo para pruebas
            String dataIn = Serial2.readStringUntil(';');
            // String dataIn = Serial.readStringUntil(';');  // solo para pruebas
            // Serial.println(dataIn);
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
            newStateMachine = dataIn2[0].toInt();
            new_estan_test = dataIn2[1].toInt();
            new_presi_test = dataIn2[2].toInt();
            new_corri_test = dataIn2[3].toInt();
            Serial2.flush();

            // Comparacion de los nuevos valores
            if (estan_test != new_estan_test)
            {
                // estan_test = new_estan_test;
                flagEstanTest = true;
            }
            if (presi_test != new_presi_test)
            {
                // presi_test = new_presi_test;
                flagPresiTest = true;
            }
            if (corri_test != new_corri_test)
            {
                // corri_test = new_corri_test;
                flagCorriTest = true;
            }

            // Serial.flush();  // solo para pruebas
            /*Serial.println("State = " + String(currentStateMachine));
              Serial.println(String(newFrecRespiratoria) + ',' + String(newI) + ',' +
              String(newE) + ',' + String(maxPresion) + ',' +
              String(alerBateria) + ',' + String(estabilidad) + ',' +
              String(newStateMachine) + ',' + String(newVentilationMode));*/
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

// Function to rupdate LCD each 200 ms
void task_sendSerialData(void *arg)
{
    while (1)
    {
        // Cambio de main menu a seal test
        if (currentStateMachine == MAIN_MENU && newStateMachine == SEAL_TEST)
        {
            currentStateMachine = newStateMachine;

            estan_test = BUSY_TEST;
            presi_test = BUSY_TEST;
            corri_test = BUSY_TEST;

            String dataToSend = String(estanquedad) + ',' + String(presion) + ',' + String(corriente) + ',' +
                                String(estan_test) + ',' + String(presi_test) + ',' + String(corri_test) + ';';

            // Serial.print(dataToSend);  // solo para pruebas
            Serial2.print(dataToSend);
        }
        else if (currentStateMachine != newStateMachine)
        {
            currentStateMachine = newStateMachine;

            switch (currentStateMachine)
            {
            case SEAL_TEST:
                estan_test = BUSY_TEST;
                break;
            case PRES_TEST:
                presi_test = BUSY_TEST;
                break;
            case CURR_TEST:
                corri_test = BUSY_TEST;
                break;
            default:
                break;
            }
            String dataToSend = String(estanquedad) + ',' + String(presion) + ',' + String(corriente) + ',' +
                                String(estan_test) + ',' + String(presi_test) + ',' + String(corri_test) + ';';

            // Serial.print(dataToSend);  // solo para pruebas
            Serial2.print(dataToSend);
        }

        // Ejecucion de prueba de estanquedad
        if (currentStateMachine == SEAL_TEST && newStateMachine != IDLE_TEST)
        {
            // Pausa antes de iniciar prueba estanquedad
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            // Inicio de prueba
            digitalWrite(EV_INSPIRA, HIGH);
            for (int i = 0; i < 40; i++)
            {
                adcADQ[i] = analogRead(ADC_PRESS_1);
                // Serial.println(adcADQ[i]);
                vTaskDelay(50);
            }
            adcMean = 0;
            for (int i = 0; i < 40; i++)
            {
                adcMean += adcADQ[i];
            }
            adcMean /= 40;
            Serial.print("Umbral Estanquedad = 1000, Promedio Estanquedad = ");
            Serial.println(adcMean);

            // Proceso de comparacion, falla o exito
            if (adcMean <= 1000)
            {
                estan_test = FAIL_TEST;
            }
            else
            {
                estan_test = PASS_TEST;
            }

            newStateMachine = IDLE_TEST;

            digitalWrite(EV_INSPIRA, LOW);

            String dataToSend = String(estanquedad) + ',' + String(presion) + ',' + String(corriente) + ',' +
                                String(estan_test) + ',' + String(presi_test) + ',' + String(corri_test) + ';';

            // Serial.print(dataToSend);  // solo para pruebas
            Serial2.print(dataToSend);
        }

        // Ejecucion de prueba de presion
        if (currentStateMachine == PRES_TEST && newStateMachine != IDLE_TEST)
        {
            // Pausa antes de iniciar prueba estanquedad
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            // Inicio de prueba
            digitalWrite(EV_ESPIRA, HIGH);
            digitalWrite(EV_INSPIRA, HIGH);
            for (int i = 0; i < 40; i++)
            {
                adcADQ[i] = analogRead(ADC_PRESS_1);
                // Serial.println(adcADQ[i]);
                vTaskDelay(50);
            }
            adcMean = 0;
            for (int i = 0; i < 40; i++)
            {
                adcMean += adcADQ[i];
            }
            adcMean /= 40;
            Serial.print("Umbral No Presion = 1000, Promedio Presion = ");
            Serial.println(adcMean);

            // Proceso de comparacion, falla o exito
            if (adcMean <= 1000)
            {
                presi_test = PASS_TEST;
            }
            else
            {
                presi_test = FAIL_TEST;
            }

            newStateMachine = IDLE_TEST;

            digitalWrite(EV_ESPIRA, LOW);
            digitalWrite(EV_INSPIRA, LOW);

            String dataToSend = String(estanquedad) + ',' + String(presion) + ',' + String(corriente) + ',' +
                                String(estan_test) + ',' + String(presi_test) + ',' + String(corri_test) + ';';

            // Serial.print(dataToSend);  // solo para pruebas
            Serial2.print(dataToSend);
        }

        // Ejecucion de prueba de corriente
        if (currentStateMachine == CURR_TEST && newStateMachine != IDLE_TEST)
        {
            // Pausa antes de iniciar prueba estanquedad
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            // Inicio de prueba
            digitalWrite(EV_ESPIRA, HIGH);

            corri_test = FAIL_TEST;
            for (int i = 0; i < 40; i++)
            {
                if (digitalRead(CURR_DETEC) == HIGH)
                {
                    corri_test = PASS_TEST;
                }
                vTaskDelay(50);
            }
            Serial.print("Umbral Corriente = 0.5, Promedio Corriente = ");
            Serial.println(corri_test);

            newStateMachine = IDLE_TEST;

            digitalWrite(EV_ESPIRA, LOW);

            String dataToSend = String(estanquedad) + ',' + String(presion) + ',' + String(corriente) + ',' +
                                String(estan_test) + ',' + String(presi_test) + ',' + String(corri_test) + ';';

            // Serial.print(dataToSend);  // solo para pruebas
            Serial2.print(dataToSend);
        }

        // flagToACBackUp = false;

        vTaskDelay(200 / portTICK_PERIOD_MS); // update each 200 ms
    }
    vTaskDelete(NULL);
}

/** ****************************************************************************
 ** ************ END OF THE CODE ***********************************************
 ** ****************************************************************************/
