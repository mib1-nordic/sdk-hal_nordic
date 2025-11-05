/*$$$LICENCE_NORDIC_STANDARD<2022>$$$*/

#include <saadc_examples_common.h>
#include <helpers/nrfx_gppi.h>
#include <nrfx_gpiote.h>

void gpiote_pin_toggle_task_setup(nrfx_gpiote_t * p_gpiote, nrfx_gpiote_pin_t pin)
{
    nrfx_err_t status;
    (void)status;

    uint8_t gpiote_channel;
    status = nrfx_gpiote_channel_alloc(p_gpiote, &gpiote_channel);
    NRFX_ASSERT(status == 0);

    static const nrfx_gpiote_output_config_t output_config =
    {
        .drive = NRF_GPIO_PIN_H0H1,
        .input_connect = NRF_GPIO_PIN_INPUT_DISCONNECT,
        .pull = NRF_GPIO_PIN_NOPULL,
    };

    const nrfx_gpiote_task_config_t task_config =
    {
        .task_ch = gpiote_channel,
        .polarity = NRF_GPIOTE_POLARITY_TOGGLE,
        .init_val = NRF_GPIOTE_INITIAL_VALUE_LOW,
    };

    status = nrfx_gpiote_output_configure(p_gpiote, pin, &output_config, &task_config);
    NRFX_ASSERT(status == 0);

    nrfx_gpiote_out_task_enable(p_gpiote, pin);
}

void pin_on_event_toggle_setup(nrfx_gpiote_t * p_gpiote, nrfx_gpiote_pin_t pin, uint32_t eep)
{
    nrfx_gppi_handle_t gppi_handle;
    int status;
    uint32_t tep;
    (void)status;

    gpiote_pin_toggle_task_setup(p_gpiote, pin);
    tep = nrfx_gpiote_out_task_address_get(p_gpiote, pin);

    status = nrfx_gppi_conn_alloc(eep, tep, &gppi_handle);
    NRFX_ASSERT(status == 0);

    nrfx_gppi_conn_enable(gppi_handle);
}
