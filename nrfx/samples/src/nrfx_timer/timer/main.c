/*$$$LICENCE_NORDIC_STANDARD<2022>$$$*/

#include <nrfx_example.h>
#include <nrfx_timer.h>

#define NRFX_LOG_MODULE                 EXAMPLE
#define NRFX_EXAMPLE_CONFIG_LOG_ENABLED 1
#define NRFX_EXAMPLE_CONFIG_LOG_LEVEL   3
#include <nrfx_log.h>

/**
 * @defgroup nrfx_timer_example Timer example
 * @{
 * @ingroup nrfx_timer_examples
 *
 * @brief Example showing basic functionality of nrfx_timer driver in Timer Mode.
 *
 * @details Application initializes nrfx_timer driver. The @ref timer_handler() is executed after
 *          the specified time.
 */

/** @brief Symbol specifying time in milliseconds to wait for handler execution. */
#define TIME_TO_WAIT_MS 5000UL

/** @brief TIMER instance used in the example. */
static nrfx_timer_t timer_inst = NRFX_TIMER_INSTANCE(NRF_TIMER_INST_GET(TIMER_INST_IDX));

#if !defined(__ZEPHYR__)
/* Define an IRQ handler named nrfx_timer_<TIMER_INST_IDX>_irq_handler. */
NRFX_INSTANCE_IRQ_HANDLER_DEFINE(timer, TIMER_INST_IDX, &timer_inst);
#endif

/**
 * @brief Function for handling TIMER driver events.
 *
 * @param[in] event_type Timer event.
 * @param[in] p_context  General purpose parameter set during initialization of
 *                       the timer. This parameter can be used to pass
 *                       additional information to the handler function, for
 *                       example, the timer ID.
 */
static void timer_handler(nrf_timer_event_t event_type, void * p_context)
{
    if(event_type == NRF_TIMER_EVENT_COMPARE0)
    {
        char * p_msg = p_context;
        NRFX_LOG_INFO("Timer finished. Context passed to the handler: >%s<", p_msg);
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
    IRQ_CONNECT(NRFX_IRQ_NUMBER_GET(NRF_TIMER_INST_GET(TIMER_INST_IDX)), IRQ_PRIO_LOWEST,
                nrfx_timer_irq_handler, &timer_inst, 0);
#endif

    NRFX_EXAMPLE_LOG_INIT();

    NRFX_LOG_INFO("Starting nrfx_timer basic example:");
    NRFX_EXAMPLE_LOG_PROCESS();

    uint32_t base_frequency = NRF_TIMER_BASE_FREQUENCY_GET(timer_inst.p_reg);
    nrfx_timer_config_t config = NRFX_TIMER_DEFAULT_CONFIG(base_frequency);
    config.bit_width = NRF_TIMER_BIT_WIDTH_32;
    config.p_context = "Some context";

    status = nrfx_timer_init(&timer_inst, &config, timer_handler);
    NRFX_ASSERT(status == 0);

    nrfx_timer_clear(&timer_inst);

    /* Creating variable desired_ticks to store the output of nrfx_timer_ms_to_ticks function */
    uint32_t desired_ticks = nrfx_timer_ms_to_ticks(&timer_inst, TIME_TO_WAIT_MS);
    NRFX_LOG_INFO("Time to wait: %lu ms", TIME_TO_WAIT_MS);

    /*
     * Setting the timer channel NRF_TIMER_CC_CHANNEL0 in the extended compare mode to stop the timer and
     * trigger an interrupt if internal counter register is equal to desired_ticks.
     */
    nrfx_timer_extended_compare(&timer_inst, NRF_TIMER_CC_CHANNEL0, desired_ticks,
                                NRF_TIMER_SHORT_COMPARE0_STOP_MASK, true);

    nrfx_timer_enable(&timer_inst);
    NRFX_LOG_INFO("Timer status: %s", nrfx_timer_is_enabled(&timer_inst) ? "enabled" : "disabled");

    while (1)
    {
        NRFX_EXAMPLE_LOG_PROCESS();
    }
}

/** @} */
