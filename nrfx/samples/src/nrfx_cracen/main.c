/*$$$LICENCE_NORDIC_STANDARD<2025>$$$*/

#include <nrfx_example.h>
#include <nrfx_cracen.h>

#define NRFX_LOG_MODULE                 EXAMPLE
#define NRFX_EXAMPLE_CONFIG_LOG_ENABLED 1
#define NRFX_EXAMPLE_CONFIG_LOG_LEVEL   3
#include <nrfx_log.h>

/**
 * @defgroup nrfx_cracen_example Basic CRACEN example
 * @{
 * @ingroup nrfx_cracen_examples
 *
 * @brief Example showing basic functionality of nrfx_cracen driver.
 *
 * @details Application initializes nrfx_cracen driver and fills a buffer with random data.
 */

/** @brief Symbol specifying number of random values to generate. */
#define RANDOM_BYTES_COUNT 5UL

/**
 * @brief Function for application main entry.
 *
 * @return Nothing.
 */
int main(void)
{
    nrfx_err_t status;
    (void)status;

    NRFX_EXAMPLE_LOG_INIT();

    NRFX_LOG_INFO("Starting nrfx_cracen example:");
    NRFX_EXAMPLE_LOG_PROCESS();

    status = nrfx_cracen_ctr_drbg_init();
    NRFX_ASSERT(status == 0);

    uint8_t buffer[RANDOM_BYTES_COUNT];
    status = nrfx_cracen_ctr_drbg_random_get(buffer, RANDOM_BYTES_COUNT);
    NRFX_ASSERT(status == 0);

    for (size_t i = 0; i < RANDOM_BYTES_COUNT; i++)
    {
        NRFX_LOG_INFO("Generated value: %d", buffer[i]);
    }

    nrfx_cracen_ctr_drbg_uninit();
    NRFX_ASSERT(status == 0);

    while (1)
    {
        NRFX_EXAMPLE_LOG_PROCESS();
    }
}

/** @} */
