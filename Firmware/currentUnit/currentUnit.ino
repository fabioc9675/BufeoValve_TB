/*

  one_device.ino - example using i2c_adc_ads7828 library

  Library:: i2c_adc_ads7828
  Author:: Doc Walker <4-20ma@wvfans.net>

  Copyright:: 2009-2016 Doc Walker

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

*/

#include "i2c_adc_ads7828.h"

#define SIGNAL 26

// device 0
// Address: A1=0, A0=1
// Command: SD=1, PD1=1, PD0=1
ADS7828 device(1, SINGLE_ENDED | REFERENCE_ON | ADC_ON, 0xFF);
ADS7828 *adc = &device;
ADS7828Channel *ch0 = adc->channel(0);
ADS7828Channel *ch1 = adc->channel(1);
ADS7828Channel *ch2 = adc->channel(2);
ADS7828Channel *ch3 = adc->channel(3);
ADS7828Channel *ch4 = adc->channel(4);
ADS7828Channel *ch5 = adc->channel(5);
ADS7828Channel *ch6 = adc->channel(6);
ADS7828Channel *ch7 = adc->channel(7);

int canal = 0;

void setup()
{
    // enable serial monitor
    Serial.begin(115200);

    // enable I2C communication
    ADS7828::begin();

    // adjust scaling on an individual channel basis
    ch0->minScale = 0;
    ch0->maxScale = 4095;
    ch1->minScale = 0;
    ch1->maxScale = 16383;
    ch2->minScale = 0;
    ch2->maxScale = 16383;
    ch3->minScale = 0;
    ch3->maxScale = 4095;
    ch4->minScale = 0;
    ch4->maxScale = 4095;
    ch5->minScale = 0;
    ch5->maxScale = 4095;
    ch6->minScale = 0;
    ch6->maxScale = 4095;
    ch7->minScale = 0;
    ch7->maxScale = 4095;

    pinMode(SIGNAL, OUTPUT);
}

void loop()
{
    // update all registered ADS7828 devices/unmasked channels
    ADS7828::updateAll();

    // output moving average values to console
    // Serial.print("\n ch0: ");
    // Serial.print(ch0->value(), DEC);
    Serial.print("\n");
    Serial.print(ch2->value(), DEC);
    // Serial.print("\n ch2: ");
    // Serial.print(ch2->value(), DEC);
    // Serial.print("\n ch3: ");
    // Serial.print(ch3->value(), DEC);
    // Serial.print("\n ch4: ");
    // Serial.print(ch4->value(), DEC);
    // Serial.print("\n ch5: ");
    // Serial.print(ch5->value(), DEC);
    // Serial.print("\n ch6: ");
    // Serial.print(ch6->value(), DEC);
    // Serial.print("\n ch7: ");
    // Serial.print(ch7->value(), DEC);
    // Serial.print("\n- - - - - - - - - - - - - - - - - - - - \n");

    canal = ch2->value();

    if (canal > 3450)
    {
        digitalWrite(SIGNAL, 1);
    }
    else
    {
        digitalWrite(SIGNAL, 0);
    }

    // delay
    delay(100);
}
