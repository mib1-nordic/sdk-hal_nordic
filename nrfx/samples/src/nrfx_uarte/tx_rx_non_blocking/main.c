/*$$$LICENCE_NORDIC_STANDARD<2022>$$$*/

#include <nrfx_example.h>
#include <nrfx_uarte.h>

#define NRFX_LOG_MODULE                 EXAMPLE
#define NRFX_EXAMPLE_CONFIG_LOG_ENABLED 1
#define NRFX_EXAMPLE_CONFIG_LOG_LEVEL   3
#include <nrfx_log.h>

/**
 * @defgroup nrfx_uarte_non_blocking_example Non-blocking UARTE example
 * @{
 * @ingroup nrfx_uarte_examples
 *
 * @brief Example showing the functionality of nrfx_uarte driver operating in the
 *        non-blocking mode.
 *
 * @details Application initializes nrfx_uarte driver, then sends the specified message
 *          ( @ref MSG_TO_SEND ) using UARTE instance. The same instance of UARTE is configured
 *          to receive this message. To perform this operation different buffers are used and data
 *          is transferred as follows:
 *          @ref m_tx_buffer -> @ref m_rx_buffer
 *          The @ref uarte_handler() is executed with relevant log messages.
 */

/** @brief Symbol specifying message to be sent via UARTE data transfer.*/
#define MSG_TO_SEND "Nordic Semiconductor"

/** @brief UARTE instance used in the example. */
static nrfx_uarte_t uarte_inst = NRFX_UARTE_INSTANCE(NRF_UARTE_INST_GET(UARTE_INST_IDX));

#if !defined(__ZEPHYR__)
/* Define an IRQ handler named nrfx_timer_<UARTE_INST_IDX>_irq_handler. */
NRFX_INSTANCE_IRQ_HANDLER_DEFINE(uarte, UARTE_INST_IDX, &uarte_inst);
#endif

/** @brief UARTE transmit buffer initialized with @ref MSG_TO_SEND. */
static uint8_t m_tx_buffer[] = MSG_TO_SEND;

/** @brief UARTE receive buffer defined with the size to store specified message ( @ref MSG_TO_SEND ). */
static uint8_t m_rx_buffer[sizeof(MSG_TO_SEND)];

/**
 * @brief Function for handling UARTE driver events.
 *
 * @param[in] p_event   Pointer to event structure. Event is allocated on the stack so it is available
 *                      only within the context of the event handler.
 * @param[in] p_context Context passed to the interrupt handler, set on initialization. In this example
 *                      p_context is used to pass the address of the UARTE instance that calls this handler.
 */
static void uarte_handler(nrfx_uarte_event_t const * p_event, void * p_context)
{
    if (p_event->type == NRFX_UARTE_EVT_TX_DONE)
    {
        NRFX_LOG_INFO("--> UARTE event: TX done");
        NRFX_LOG_INFO("Content of TX buffer: %s", m_tx_buffer);
        NRFX_LOG_INFO("Content of RX buffer: %s", m_rx_buffer);
    }
    else
    {
        NRFX_LOG_INFO("UARTE event: %d", p_event->type);
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
    IRQ_CONNECT(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(UARTE_INST_IDX)), IRQ_PRIO_LOWEST,
                nrfx_uarte_irq_handler, &uarte_inst, 0);
#endif

    NRFX_EXAMPLE_LOG_INIT();

    NRFX_LOG_INFO("Starting nrfx_uarte non-blocking example.");
    NRFX_EXAMPLE_LOG_PROCESS();

    nrfx_uarte_config_t uarte_config = NRFX_UARTE_DEFAULT_CONFIG(UARTE_TX_PIN, UARTE_RX_PIN);
    status = nrfx_uarte_init(&uarte_inst, &uarte_config, uarte_handler);
    NRFX_ASSERT(status == 0);

    NRFX_LOG_INFO("Content of TX buffer: %s", m_tx_buffer);
    NRFX_LOG_INFO("Content of RX buffer: %s", m_rx_buffer);
    NRFX_EXAMPLE_LOG_PROCESS();

    status = nrfx_uarte_rx_buffer_set(&uarte_inst, m_rx_buffer, NRFX_ARRAY_SIZE(m_rx_buffer) - 1);
    NRFX_ASSERT(status == 0);

    status = nrfx_uarte_rx_enable(&uarte_inst, NRFX_UARTE_RX_ENABLE_STOP_ON_END);
    NRFX_ASSERT(status == 0);

    status = nrfx_uarte_tx(&uarte_inst, m_tx_buffer, sizeof(m_tx_buffer), 0);
    NRFX_ASSERT(status == 0);

    while (nrfx_uarte_tx_in_progress(&uarte_inst))
    {}

    nrfx_uarte_uninit(&uarte_inst);

    while (1)
    {
        NRFX_EXAMPLE_LOG_PROCESS();
    }
}

/** @} */
