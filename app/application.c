#include <application.h>

#define SENSOR_UPDATE_INTERVAL_SECONDS 120
#define FIRST_REPORT_SECONDS 60
#define REGULAR_REPORT_SECONDS (15 * 60)
#define CALIBRATION_DELAY_SECONDS (10 * 60)
#define OOB_CO2_CONCENTRATION 100
#define APPLICATION_TASK_ID 0

bc_led_t led;
bc_button_t button;
bc_module_sigfox_t sigfox_module;
bc_tag_temperature_t temperature_tag_internal;
bc_tag_temperature_t temperature_tag;
bc_tag_barometer_t barometer_tag;
bc_tag_humidity_t humidity_tag;

BC_DATA_STREAM_FLOAT_BUFFER(stream_buffer_temperature_tag, 8)
bc_data_stream_t stream_temperature_tag;
BC_DATA_STREAM_FLOAT_BUFFER(stream_buffer_barometer_tag, 8)
bc_data_stream_t stream_barometer_tag;
BC_DATA_STREAM_FLOAT_BUFFER(stream_buffer_humidity_tag, 8)
bc_data_stream_t stream_humidity_tag;
BC_DATA_STREAM_FLOAT_BUFFER(stream_buffer_co2_concentration, 8)
bc_data_stream_t stream_co2_concentration;

void calibration_task(void *param)
{
    (void) param;

    bc_led_set_mode(&led, BC_LED_MODE_OFF);

    bc_module_co2_calibration(BC_MODULE_CO2_CALIBRATION_BACKGROUND_FILTERED);

    bc_scheduler_unregister(bc_scheduler_get_current_task_id());
}

void sigfox_module_event_handler(bc_module_sigfox_t *self, bc_module_sigfox_event_t event, void *event_param)
{
    (void) self;
    (void) event_param;

    if (event == BC_MODULE_SIGFOX_EVENT_ERROR)
    {
        bc_led_set_mode(&led, BC_LED_MODE_BLINK);
    }
    if (event == BC_MODULE_SIGFOX_EVENT_SEND_RF_FRAME_START)
    {
        bc_led_set_mode(&led, BC_LED_MODE_ON);
    }
    else if (event == BC_MODULE_SIGFOX_EVENT_SEND_RF_FRAME_DONE)
    {
        bc_led_set_mode(&led, BC_LED_MODE_OFF);
    }
}

void co2_module_event_handler(bc_module_co2_event_t event, void *event_param)
{
    (void) event;
    (void) event_param;

    bc_data_stream_t *stream = (bc_data_stream_t *) event_param;

    float value;

    if (bc_module_co2_get_concentration(&value))
    {
        float average;

        if (bc_data_stream_get_average(stream, &average))
        {
            if (abs(value - average) > OOB_CO2_CONCENTRATION)
            {
                float last_value;

                bc_data_stream_get_last(stream, &last_value);

                if (abs(last_value - average) > OOB_CO2_CONCENTRATION)
                {
                    bc_scheduler_plan_now(APPLICATION_TASK_ID);
                }
            }
        }

        bc_data_stream_feed(stream, &value);
    }
    else
    {
        bc_data_stream_reset(stream);
    }
}

void temperature_tag_event_handler(bc_tag_temperature_t *self, bc_tag_temperature_event_t event, void *event_param)
{
    (void) event;

    bc_data_stream_t *stream = (bc_data_stream_t *) event_param;

    float value;

    if (bc_tag_temperature_get_temperature_celsius(self, &value))
    {
        bc_data_stream_feed(stream, &value);
    }
    else
    {
        bc_data_stream_reset(stream);
    }

}

void humidity_tag_event_handler(bc_tag_humidity_t *self, bc_tag_humidity_event_t event, void *event_param)
{
    (void) event;

    bc_data_stream_t *stream = (bc_data_stream_t *) event_param;

    float value;

    if (bc_tag_humidity_get_humidity_percentage(self, &value))
    {
        bc_data_stream_feed(stream, &value);
    }
    else
    {
        bc_data_stream_reset(stream);
    }
}

void barometer_tag_event_handler(bc_tag_barometer_t *self, bc_tag_barometer_event_t event, void *event_param)
{
    (void) event;

    bc_data_stream_t *stream = (bc_data_stream_t *) event_param;

    float value;

    if (bc_tag_barometer_get_pressure_pascal(self, &value))
    {
        bc_data_stream_feed(stream, &value);
    }
    else
    {
        bc_data_stream_reset(stream);
    }
}

void button_event_handler(bc_button_t *self, bc_button_event_t event, void *event_param)
{
    (void) self;
    (void) event_param;

    if (event == BC_BUTTON_EVENT_CLICK)
    {
        bc_scheduler_plan_now(0);
    }
    else if (event == BC_BUTTON_EVENT_HOLD)
    {
        bc_led_set_mode(&led, BC_LED_MODE_BLINK);

        bc_scheduler_register(calibration_task, NULL, bc_tick_get() + CALIBRATION_DELAY_SECONDS * 1000);
    }
}

void application_init(void)
{
    bc_led_init(&led, BC_GPIO_LED, false, false);

    bc_module_battery_init(BC_MODULE_BATTERY_FORMAT_STANDARD);

    bc_module_sigfox_init(&sigfox_module, BC_MODULE_SIGFOX_REVISION_R2);
    bc_module_sigfox_set_event_handler(&sigfox_module, sigfox_module_event_handler, NULL);

    bc_data_stream_init(&stream_temperature_tag, BC_DATA_STREAM_TYPE_FLOAT, stream_buffer_temperature_tag, sizeof(stream_buffer_temperature_tag));
    bc_data_stream_init(&stream_barometer_tag, BC_DATA_STREAM_TYPE_FLOAT, stream_buffer_barometer_tag, sizeof(stream_buffer_barometer_tag));
    bc_data_stream_init(&stream_humidity_tag, BC_DATA_STREAM_TYPE_FLOAT, stream_buffer_humidity_tag, sizeof(stream_buffer_humidity_tag));
    bc_data_stream_init(&stream_co2_concentration, BC_DATA_STREAM_TYPE_FLOAT, stream_buffer_co2_concentration, sizeof(stream_buffer_co2_concentration));

    bc_module_co2_init();
    bc_module_co2_set_update_interval(SENSOR_UPDATE_INTERVAL_SECONDS * 1000);
    bc_module_co2_set_event_handler(co2_module_event_handler, &stream_co2_concentration);

    bc_tag_temperature_init(&temperature_tag_internal, BC_I2C_I2C0, BC_TAG_TEMPERATURE_I2C_ADDRESS_ALTERNATE);
    bc_tag_temperature_set_update_interval(&temperature_tag_internal, SENSOR_UPDATE_INTERVAL_SECONDS * 1000);
    bc_tag_temperature_set_event_handler(&temperature_tag_internal, NULL, NULL);

    bc_tag_temperature_init(&temperature_tag, BC_I2C_I2C0, BC_TAG_TEMPERATURE_I2C_ADDRESS_DEFAULT);
    bc_tag_temperature_set_update_interval(&temperature_tag, SENSOR_UPDATE_INTERVAL_SECONDS * 1000);
    bc_tag_temperature_set_event_handler(&temperature_tag, temperature_tag_event_handler, &stream_temperature_tag);

    bc_tag_humidity_init(&humidity_tag, BC_TAG_HUMIDITY_REVISION_R2, BC_I2C_I2C0, BC_TAG_HUMIDITY_I2C_ADDRESS_DEFAULT);
    bc_tag_humidity_set_update_interval(&humidity_tag, SENSOR_UPDATE_INTERVAL_SECONDS * 1000);
    bc_tag_humidity_set_event_handler(&humidity_tag, humidity_tag_event_handler, &stream_humidity_tag);

    bc_tag_barometer_init(&barometer_tag, BC_I2C_I2C0);
    bc_tag_barometer_set_update_interval(&barometer_tag, SENSOR_UPDATE_INTERVAL_SECONDS * 1000);
    bc_tag_barometer_set_event_handler(&barometer_tag, barometer_tag_event_handler, &stream_barometer_tag);

    bc_button_init(&button, BC_GPIO_BUTTON, BC_GPIO_PULL_DOWN, false);
    bc_button_set_hold_time(&button, 10000);
    bc_button_set_event_handler(&button, button_event_handler, NULL);
}

void application_task(void *param)
{
    (void) param;

    uint8_t header = 0;
    int16_t temperature = 0;
    uint8_t humidity = 0;
    uint16_t pressure = 0;
    uint16_t co2_concentration = 0;
    uint16_t co2_concentration_median = 0;
    uint16_t co2_concentration_raw = 0;

    float avarage;

    if (bc_data_stream_get_average(&stream_temperature_tag, &avarage))
    {
        header |= 0x01; temperature = avarage * 2;
    }

    if (bc_data_stream_get_average(&stream_humidity_tag, &avarage))
    {
        header |= 0x02; humidity = avarage * 2;
    }

    if (bc_data_stream_get_average(&stream_barometer_tag, &avarage))
    {
        header |= 0x04; pressure = avarage / 2;
    }

    if (bc_data_stream_get_average(&stream_co2_concentration, &avarage))
    {
        header |= 0x08; co2_concentration = avarage;
    }

    float median;

    if (bc_data_stream_get_median(&stream_co2_concentration, &median))
    {
        header |= 0x10; co2_concentration_median = median;
    }

    float raw;

    if (bc_data_stream_get_last(&stream_co2_concentration, &raw))
    {
        header |= 0x20; co2_concentration_raw = raw;
    }

    uint8_t buffer[12];

    buffer[0] = header;
    buffer[1] = temperature;
    buffer[2] = temperature >> 8;
    buffer[3] = humidity;
    buffer[4] = pressure;
    buffer[5] = pressure >> 8;
    buffer[6] = co2_concentration;
    buffer[7] = co2_concentration >> 8;
    buffer[8] = co2_concentration_median;
    buffer[9] = co2_concentration_median >> 8;
    buffer[10] = co2_concentration_raw;
    buffer[11] = co2_concentration_raw >> 8;

    if (bc_module_sigfox_send_rf_frame(&sigfox_module, buffer, sizeof(buffer)))
    {
        bc_scheduler_plan_current_relative(REGULAR_REPORT_SECONDS * 1000);
    }
    else
    {
        bc_scheduler_plan_current_relative(1000);
    }
}
