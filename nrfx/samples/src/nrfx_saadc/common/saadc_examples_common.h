/*$$$LICENCE_NORDIC_STANDARD<2022>$$$*/

#ifndef SAADC_EXAMPLES_COMMON_H__
#define SAADC_EXAMPLES_COMMON_H__

#include <nrfx_gpiote.h>

/**
 * @defgroup nrfx_saadc_examples_common Common SAADC module
 * @{
 * @ingroup nrfx_saadc_examples
 *
 * @brief Module with common functionalities used in nrfx_saadc examples.
 */

/**
 * @brief SAADC channel configuration for the single-ended mode with 3 us sample acquisition time.
 *        The 3 us acquisition time will work correctly when the source resistance of @p _pin_p input
 *        analog pin is less than 10 kOhm.
 *
 * This configuration sets up single-ended SAADC channel with the following options:
 * - resistor ladder inactive
 * - gain: 1
 * - reference voltage: internal 0.6 V
 * - sample acquisition time: 3 us
 * - burst disabled
 *
 * @param[in] _pin_p Positive input analog pin.
 * @param[in] _index Channel index.
 *
 * @sa nrfx_saadc_channel_t
 */
 #define SAADC_CHANNEL_SE_ACQ_3US(_pin_p, _index)                      \
{                                                                      \
    .channel_config =                                                  \
    {                                                                  \
        NRFX_COND_CODE_1(NRF_SAADC_HAS_CH_CONFIG_RES,                  \
                         (.resistor_p = NRF_SAADC_RESISTOR_DISABLED,   \
                          .resistor_n = NRF_SAADC_RESISTOR_DISABLED,), \
                        ())                                            \
        .gain       = NRF_SAADC_GAIN1,                                 \
        .reference  = NRF_SAADC_REFERENCE_INTERNAL,                    \
        NRFX_COND_CODE_1(NRF_SAADC_HAS_ACQTIME_ENUM,                   \
                         (.acq_time = NRF_SAADC_ACQTIME_3US,),         \
                         (.acq_time = 3,))                             \
        .mode       = NRF_SAADC_MODE_SINGLE_ENDED,                     \
        .burst      = NRF_SAADC_BURST_DISABLED,                        \
    },                                                                 \
    .pin_p          = (nrfx_analog_input_t)_pin_p,                     \
    .pin_n          = NRFX_ANALOG_INPUT_DISABLED,                      \
    .channel_index  = _index,                                          \
}

/**
 * @brief Function for setting up a GPIOTE task that toggles a given pin.
 *
 * @param[in] p_gpiote Pointer to the GPIOTE driver instance structure.
 * @param[in] pin      The pin to toggle.
 */
void gpiote_pin_toggle_task_setup(nrfx_gpiote_t * p_gpiote, nrfx_gpiote_pin_t pin);

/**
 * @brief Function for setting up a pin to be toggled once specified event is triggered.
 *
 * @param[in] p_gpiote Pointer to the GPIOTE driver instance structure.
 * @param[in] pin      The pin to toggle.
 * @param[in] eep      Address of the event register. This event will trigger the @p pin to toggle.
 */
void pin_on_event_toggle_setup(nrfx_gpiote_t * p_gpiote, nrfx_gpiote_pin_t pin, uint32_t eep);

/** @} */

#endif // SAADC_EXAMPLES_COMMON_H__
