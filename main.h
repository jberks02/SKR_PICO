/*
 * SKR PICO
 *
 * @version     1.0.0
 * @author      Justin Berkshire
 * @copyright   2021
 * @licence     MIT
 *
 */
#ifndef _PICOCPP_MAIN_HEADER_
#define _PICOCPP_MAIN_HEADER_

/*
 * C++ HEADERS
 */
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <bits/stdc++.h>

/*
 * PICO HEADERS
 */
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"
#include "pico/multicore.h"
#include "hardware/adc.h"
// #include "hardware/uart.h"

// MY HEADERS
#include "./structs/tmcRegisters.cpp"
#include "./structs/tcmReadResponse.cpp"
#include "./structs/Peripherals.cpp"
#include "./structs/StepMove.cpp"
#include "./modules/UartInterface.cpp"
#include "./modules/Stepper.cpp"
#include "./modules/SynchronousMovement.cpp"

#ifdef __cplusplus
extern "C"
{
#endif

    /*
 * Usual header code here
 */

#ifdef __cplusplus
}
#endif

#endif // _PICOCPP_MAIN_HEADER_