/*$$$LICENCE_NORDIC_STANDARD<2022>$$$*/

#include <nrfx_example.h>
#include <nrfx_egu.h>

#define NRFX_LOG_MODULE                 EXAMPLE
#define NRFX_EXAMPLE_CONFIG_LOG_ENABLED 1
#define NRFX_EXAMPLE_CONFIG_LOG_LEVEL   3
#include <nrfx_log.h>

/**
 * @defgroup nrfx_egu_example Basic EGU example
 * @{
 * @ingroup nrfx_egu_examples
 *
 * @brief Example showing basic functionality of nrfx_egu driver.
 *
 * @details Application initializes nrfx_egu driver instance and triggers
 *          three subsequent EGU channels. Then @ref egu_handler() is executed once
 *          for each triggered channel with relevant log message.
 */

/** @brief EGU instance used in the example. */
static nrfx_egu_t egu_inst = NRFX_EGU_INSTANCE(NRF_EGU_INST_GET(EGU_INST_IDX));

#if !defined(__ZEPHYR__)
/* Define an IRQ handler named nrfx_egu_<EGU_INST_IDX>_irq_handler. */
NRFX_INSTANCE_IRQ_HANDLER_DEFINE(egu, EGU_INST_IDX, &egu_inst);
#endif

/**
 * @brief Function for handling EGU driver events.
 *
 * @param[in] event_idx Index of the event that generated the interrupt.
 * @param[in] p_context Context passed to the event handler. Set on initialization.
 */
static void egu_handler(uint8_t event_idx, void * p_context)
{
    char * p_msg = p_context;
    NRFX_LOG_INFO("EGU handler triggered on channel: %u. "
                  "Context passed to the handler: >%s<",
                  event_idx, p_msg);
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
    IRQ_CONNECT(NRFX_IRQ_NUMBER_GET(NRF_EGU_INST_GET(EGU_INST_IDX)), IRQ_PRIO_LOWEST,
                nrfx_egu_irq_handler, &egu_inst, 0);
#endif

    NRFX_EXAMPLE_LOG_INIT();

    NRFX_LOG_INFO("Starting nrfx_egu example");
    NRFX_EXAMPLE_LOG_PROCESS();

    void * p_context = "Some context";
    status = nrfx_egu_init(&egu_inst, NRFX_EGU_DEFAULT_CONFIG_IRQ_PRIORITY, egu_handler, p_context);
    NRFX_ASSERT(status == 0);

    uint32_t ch0_idx = 0;
    uint32_t ch1_idx = 1;
    uint32_t ch2_idx = 2;

    uint32_t channels_to_trigger = (1 << ch0_idx) | (1 << ch1_idx) | (1 << ch2_idx);
    nrfx_egu_int_enable(&egu_inst, channels_to_trigger);

    nrfx_egu_trigger(&egu_inst, ch0_idx);
    nrfx_egu_trigger(&egu_inst, ch1_idx);
    nrfx_egu_trigger(&egu_inst, ch2_idx);

    while (1)
    {
        NRFX_EXAMPLE_LOG_PROCESS();
    }
}

/** @} */
