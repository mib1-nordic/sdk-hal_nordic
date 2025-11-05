/*$$$LICENCE_NORDIC_STANDARD<2022>$$$*/

#include <nrfx_example.h>
#include <nrfx_temp.h>

#define NRFX_LOG_MODULE                 EXAMPLE
#define NRFX_EXAMPLE_CONFIG_LOG_ENABLED 1
#define NRFX_EXAMPLE_CONFIG_LOG_LEVEL   3
#include <nrfx_log.h>

/**
 * @defgroup nrfx_temp_blocking_example Blocking TEMP example
 * @{
 * @ingroup nrfx_temp_examples
 *
 * @brief Example shows the functionality of nrfx_temp driver operating in the blocking mode.
 *
 * @details Application initializes nrfx_temp driver and starts operating in the blocking mode.
 *          The temperature measurement and relevant message logging are performed, until a number
 *          of measurements taken is equal to the value specified by the user.
 */

/** @brief Symbol specifying the number of temperature measurements to take. */
#define TEMP_ITERATIONS 5UL

/**
 * @brief Function for application main entry.
 *
 * @return Nothing.
 */
int main(void)
{
    nrfx_err_t status;
    (void)status;

    uint32_t i = 0;

    NRFX_EXAMPLE_LOG_INIT();

    NRFX_LOG_INFO("Starting nrfx_temp blocking example.");
    NRFX_EXAMPLE_LOG_PROCESS();

    nrfx_temp_config_t config = NRFX_TEMP_DEFAULT_CONFIG;
    status = nrfx_temp_init(&config, NULL);
    NRFX_ASSERT(status == 0);

    while (1)
    {
        if (i < TEMP_ITERATIONS)
        {
            status = nrfx_temp_measure();
            NRFX_ASSERT(status == 0);

            int32_t temperature = nrfx_temp_result_get();
            int32_t celsius_temperature = nrfx_temp_calculate(temperature);

            int32_t whole_celsius = celsius_temperature / 100;
            uint8_t fraction_celsius = NRFX_ABS(celsius_temperature % 100);

            NRFX_LOG_INFO("Measured temperature: %d.%02u [C]", whole_celsius, fraction_celsius);

            i++;
        }
        else if (i == TEMP_ITERATIONS)
        {
            nrfx_temp_uninit();
            i++;
        }

        NRFX_EXAMPLE_LOG_PROCESS();
    }
}

/** @} */
