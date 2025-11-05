/*$$$LICENCE_NORDIC_STANDARD<2022>$$$*/

#ifndef NRFX_EXAMPLE_H__
#define NRFX_EXAMPLE_H__

#include <hal/nrf_gpio.h>

#if defined(__ZEPHYR__)
#include <zephyr/logging/log_ctrl.h>
#define NRFX_EXAMPLE_LOG_INIT()    LOG_INIT()
#define NRFX_EXAMPLE_LOG_PROCESS() while (LOG_PROCESS())
#else
#define NRFX_EXAMPLE_LOG_INIT()
#define NRFX_EXAMPLE_LOG_PROCESS()
#endif

#if defined(NRF52832_XXAA) || defined(NRF52832_XXAB)
    #include "boards/nrf52dk_nrf52832_config.h"
#elif defined(NRF52830_XXAA)
    #include "boards/nrf52833dk_nrf52833_config.h"
#elif defined(NRF52833_XXAA)
    #include "boards/nrf52833dk_nrf52833_config.h"
#elif defined(NRF52840_XXAA)
    #include "boards/nrf52840dk_nrf52840_config.h"
#elif defined(NRF53_SERIES)
    #include "boards/nrf5340dk_nrf5340_config.h"
#elif defined(NRF91_SERIES)
    #include "boards/nrf9160dk_nrf9160_config.h"
#elif defined(NRF54L15_XXAA)
    #include "boards/nrf54l15dk_nrf54l15_config.h"
#else
#error "Unknown device."
#endif

#endif // NRFX_EXAMPLE_H__
