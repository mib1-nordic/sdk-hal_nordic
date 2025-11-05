/*$$$LICENCE_NORDIC_STANDARD<2022>$$$*/

#include <nrfx_example.h>
#include <nrfx_twim.h>
#include <nrfx_twis.h>

#define NRFX_LOG_MODULE                 EXAMPLE
#define NRFX_EXAMPLE_CONFIG_LOG_ENABLED 1
#define NRFX_EXAMPLE_CONFIG_LOG_LEVEL   3
#include <nrfx_log.h>

/**
 * @defgroup nrfx_twim_twis_blocking_example Blocking TWIM with TWIS example
 * @{
 * @ingroup nrfx_twim_twis_examples
 *
 * @brief Example showing the functionality of nrfx_twim and nrfx_twis drivers operating in the
 *        blocking mode.
 *
 * @details Application initializes nrfx_twim and nrfx_twis drivers, then sends the specified message
 *          ( @ref MSG_TO_SEND ) from master to slave and back using TWIM and TWIS peripherals.
 *          To perform this operation different buffers are used and data is transferred as follows:
 *          @ref m_tx_buffer_master -> @ref m_rx_buffer_slave -> @ref m_rx_buffer_master
 */

/** @brief Symbol specifying slave address on TWI bus. */
#define SLAVE_ADDR 0x0B

/** @brief Symbol specifying message to be sent via TWIM data transfer.*/
#define MSG_TO_SEND "Nordic Semiconductor"

/** @brief TWIM transmit buffer initialized with @ref MSG_TO_SEND. */
static uint8_t m_tx_buffer_master[] = MSG_TO_SEND;

/** @brief TWIS recieve buffer for a message from TWIM. */
static uint8_t m_rx_buffer_slave[sizeof(MSG_TO_SEND)];

/** @brief TWIM recieve buffer for a message from TWIS. */
static uint8_t m_rx_buffer_master[sizeof(MSG_TO_SEND)];

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

    NRFX_LOG_INFO("Starting nrfx_twim_twis blocking example.");
    NRFX_EXAMPLE_LOG_PROCESS();

    nrfx_twim_t twim_inst = NRFX_TWIM_INSTANCE(NRF_TWIM_INST_GET(TWIM_INST_IDX));
    nrfx_twis_t twis_inst = NRFX_TWIS_INSTANCE(NRF_TWIM_INST_GET(TWIS_INST_IDX));

    nrfx_twim_xfer_desc_t twim_xfer_desc = NRFX_TWIM_XFER_DESC_TX(SLAVE_ADDR,
                                                                  m_tx_buffer_master,
                                                                  sizeof(m_tx_buffer_master));
    nrfx_twim_config_t twim_config = NRFX_TWIM_DEFAULT_CONFIG(TWIM_TWIS_MASTER_SCL_PIN, TWIM_TWIS_MASTER_SDA_PIN);
    status = nrfx_twim_init(&twim_inst, &twim_config, NULL, &twim_xfer_desc);
    NRFX_ASSERT(status == 0);

    nrfx_twis_config_t twis_config = NRFX_TWIS_DEFAULT_CONFIG(TWIM_TWIS_SLAVE_SCL_PIN,
                                                              TWIM_TWIS_SLAVE_SDA_PIN,
                                                              SLAVE_ADDR);

    status = nrfx_twis_init(&twis_inst, &twis_config, NULL);
    NRFX_ASSERT(status == 0);

    NRFX_LOG_INFO("Message to send: %s", m_tx_buffer_master);
    NRFX_LOG_INFO("Content of slave RX buffer:  %s", m_rx_buffer_slave);
    NRFX_LOG_INFO("Content of master RX buffer: %s", m_rx_buffer_master);
    NRFX_EXAMPLE_LOG_PROCESS();

    nrfx_twis_enable(&twis_inst);
    nrfx_twim_enable(&twim_inst);

    status = nrfx_twis_rx_prepare(&twis_inst, m_rx_buffer_slave, sizeof(m_rx_buffer_slave));
    NRFX_ASSERT(status == 0);

    status = nrfx_twim_xfer(&twim_inst, &twim_xfer_desc, 0);
    NRFX_ASSERT(status == 0);

    twim_xfer_desc.type = NRFX_TWIM_XFER_RX;
    twim_xfer_desc.p_primary_buf = m_rx_buffer_master;
    twim_xfer_desc.primary_length = sizeof(m_rx_buffer_master);

    status = nrfx_twis_tx_prepare(&twis_inst, m_rx_buffer_slave, sizeof(m_rx_buffer_slave));
    NRFX_ASSERT(status == 0);

    status = nrfx_twim_xfer(&twim_inst, &twim_xfer_desc, 0);
    NRFX_ASSERT(status == 0);

    NRFX_LOG_INFO("m_tx_buffer_master -> m_rx_buffer_slave -> m_rx_buffer_master");
    NRFX_LOG_INFO("Content of slave RX buffer:  %s", m_rx_buffer_slave);
    NRFX_LOG_INFO("Content of master RX buffer: %s", m_rx_buffer_master);

    while (1)
    {
        NRFX_EXAMPLE_LOG_PROCESS();
    }
}

/** @} */
