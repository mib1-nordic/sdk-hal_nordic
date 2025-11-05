/*$$$LICENCE_NORDIC_STANDARD<2022>$$$*/

#include <nrfx_example.h>
#include <nrfx_rng.h>

#define NRFX_LOG_MODULE                 EXAMPLE
#define NRFX_EXAMPLE_CONFIG_LOG_ENABLED 1
#define NRFX_EXAMPLE_CONFIG_LOG_LEVEL   3
#include <nrfx_log.h>

/**
 * @defgroup nrfx_rng_example Basic RNG example
 * @{
 * @ingroup nrfx_rng_examples
 *
 * @brief Example showing basic functionality of nrfx_rng driver.
 *
 * @details Application initializes nrfx_rng driver. The @ref rng_handler() is executed with relevant
 *          log message, until a number of generated numbers is equal to the value specified by the user.
 */

/** @brief Symbol specifying number of random values to generate. */
#define RNG_ITERATIONS 5UL

/**
 * @brief Function for handling RNG driver events.
 *
 * @param[in] rng_data Data passed to the event handler.
 */
static void rng_handler(uint8_t rng_data)
{
    static uint32_t i = 0;

    if (i < RNG_ITERATIONS)
    {
        NRFX_LOG_INFO("Generated value: %d", rng_data);
        i++;
    }
    else
    {
        nrfx_rng_stop();
    }
}

/**
 * @brief Function for application main entry.
 *
 * @return Nothing.
 */
int main(void)
{
    nrfx_err_t status;
    (void)status;

#if defined(__ZEPHYR__)
    IRQ_CONNECT(NRFX_IRQ_NUMBER_GET(NRF_RNG), IRQ_PRIO_LOWEST, nrfx_rng_irq_handler, 0, 0);
#endif

    NRFX_EXAMPLE_LOG_INIT();

    NRFX_LOG_INFO("Starting nrfx_rng example:");
    NRFX_EXAMPLE_LOG_PROCESS();

    nrfx_rng_config_t config = NRFX_RNG_DEFAULT_CONFIG;
    status = nrfx_rng_init(&config, rng_handler);
    NRFX_ASSERT(status == 0);

    nrfx_rng_start();

    while (1)
    {
        NRFX_EXAMPLE_LOG_PROCESS();
    }
}

/** @} */
