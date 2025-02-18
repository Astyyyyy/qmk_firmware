/* 
Copyright 2023 NEO

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once


/* RGB matrix */
#ifdef RGB_MATRIX_ENABLE

#define I2C_DRIVER I2CD1
#define I2C1_SCL_PIN B8
#define I2C1_SDA_PIN B9
#define I2C1_SCL_PAL_MODE 1
#define I2C1_SDA_PAL_MODE 1
#define I2C1_TIMINGR_PRESC 0x00U
#define I2C1_TIMINGR_SCLDEL 0x03U
#define I2C1_TIMINGR_SDADEL 0x01U
#define I2C1_TIMINGR_SCLH 0x03U
#define I2C1_TIMINGR_SCLL 0x09U


// i2c setting
//#define USE_I2CV1
//#define STM32_I2C_USE_I2C1 1
//#define USE_I2C 1
//
//#define I2C1_SCL 8 //I2C1_SCL_PIN B6// //AT403
//#define I2C1_SDA 9 //I2C1_SDA_PIN B7//  //AT403 
//#define I2C1_SCL_PIN        B8
//#define I2C1_SDA_PIN        B9
//#define SPI_SCK_PIN 13  // PB13 for SPI2_SCK
//#define SPI_MISO_PIN 14 // PB14 for SPI2_MISO
//#define SPI_MOSI_PIN 15 // PB15 for SPI2_MOSI
//#define SPI_NSS_PIN 12  // PB12 for SPI2_NSS
//#define OPMODE_I2C 1   // Typically, 0 corresponds to standard I2C mode
//#define STD_DUTY_CYCLE 2  // Standard duty cycle for I2C
//#define I2C1_OPMODE OPMODE_I2C
//#define I2C1_OPMODE OPMODE_I2C
//#define I2C1_CLOCK_SPEED 400000  //AT403
//#define I2C1_DUTY_CYCLE FAST_DUTY_CYCLE_2  //AT403
//#define I2C1_DUTY_CYCLE I2C_DUTY_CYCLE_2  //AT403
//#define I2C1_DUTY_CYCLE STD_DUTY_CYCLE
//#define IS31FL3741_I2C_ADDRESS_1 IS31FL3741_I2C_ADDRESS_GND
// #    define RGB_MATRIX_DISABLE_KEYCODES //禁用默认的RGB_TOGGLE等键值来控制RGB矩阵，而使用其他键和代码来控制
#    define RGB_DISABLE_AFTER_TIMEOUT 0 // number of ticks to wait until disabling effects
#    define RGB_DISABLE_WHEN_USB_SUSPENDED  // turn off effects when suspended
#    define RGB_MATRIX_KEYPRESSES           // reacts to keypresses
#    define RGB_MATRIX_FRAMEBUFFER_EFFECTS
#    define RGB_MATRIX_LED_PROCESS_LIMIT  4
#    define RGB_MATRIX_LED_FLUSH_LIMIT    26
//// #    define RGB_MATRIX_MAXIMUM_BRIGHTNESS 128
#    define RGB_MATRIX_STARTUP_VAL        64
#    define DRIVER_ADDR_1                 0b0110000
#    define DRIVER_COUNT                  1
#    define RGB_MATRIX_LED_COUNT          68   //DRIVER_LED_TOTAL              //RGB_MATRIX_LED_COUNT 

#endif