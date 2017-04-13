#include <application.h>

#define SENSOR_UPDATE_INTERVAL_SECONDS 30
#define FIRST_REPORT_SECONDS 60
#define REGULAR_REPORT_SECONDS (15 * 60)


struct
{
    struct { bool valid; float value; } temperature;
    struct { bool valid; float value; } humidity;
    struct { bool valid; float value; } pressure;
    struct { bool valid; float value; } co2_concentration;

} sensor;

bc_led_t led;
bc_module_sigfox_t sigfox_module;
bc_tag_temperature_t temperature_tag_internal;
bc_tag_temperature_t temperature_tag;
bc_tag_barometer_t barometer_tag;
bc_tag_humidity_t humidity_tag;

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

    sensor.co2_concentration.valid = bc_module_co2_get_concentration(&sensor.co2_concentration.value);
}

void temperature_tag_event_handler(bc_tag_temperature_t *self, bc_tag_temperature_event_t event, void *event_param)
{
    (void) event;
    (void) event_param;

    sensor.temperature.valid = bc_tag_temperature_get_temperature_celsius(self, &sensor.temperature.value);
}

void humidity_tag_event_handler(bc_tag_humidity_t *self, bc_tag_humidity_event_t event, void *event_param)
{
    (void) event;
    (void) event_param;

    sensor.humidity.valid = bc_tag_humidity_get_humidity_percentage(self, &sensor.humidity.value);
}

void barometer_tag_event_handler(bc_tag_barometer_t *self, bc_tag_barometer_event_t event, void *event_param)
{
    (void) event;
    (void) event_param;

    sensor.pressure.valid = bc_tag_barometer_get_pressure_pascal(self, &sensor.pressure.value);
}

void timer_task(void *param)
{
    (void) param;

	uint8_t header = 0;
    int16_t temperature = 0;
	uint8_t humidity = 0;
	uint16_t pressure = 0;
    uint16_t co2_concentration = 0;

	if (sensor.temperature.valid)
	{
		header |= 0x01; temperature = sensor.temperature.value * 2;
	}

	if (sensor.humidity.valid)
	{
		header |= 0x02; humidity = sensor.humidity.value * 2;
	}

	if (sensor.pressure.valid)
	{
        header |= 0x04; pressure = sensor.pressure.value;
	}

    if (sensor.co2_concentration.valid)
	{
        header |= 0x08; co2_concentration = sensor.co2_concentration.value;
	}

    uint8_t buffer[8];

    buffer[0] = header;
    buffer[1] = temperature;
    buffer[2] = temperature >> 8;
    buffer[3] = humidity;
    buffer[4] = pressure;
    buffer[5] = pressure >> 8;
    buffer[6] = co2_concentration;
    buffer[7] = co2_concentration >> 8;

	if (bc_module_sigfox_send_rf_frame(&sigfox_module, buffer, sizeof(buffer)))
	{
		bc_scheduler_plan_current_relative(15 * 60 * 1000);
	}
	else
	{
		bc_scheduler_plan_current_relative(1000);
	}
}

void application_init(void)
{
    memset(&sensor, 0, sizeof(sensor));

    bc_led_init(&led, BC_GPIO_LED, false, false);
    bc_led_set_mode(&led, BC_LED_MODE_ON);

    bc_module_battery_init(BC_MODULE_BATTERY_FORMAT_STANDARD);

    bc_module_sigfox_init(&sigfox_module, BC_MODULE_SIGFOX_REVISION_R2);
	bc_module_sigfox_set_event_handler(&sigfox_module, sigfox_module_event_handler, NULL);

    bc_module_co2_init();
    bc_module_co2_set_update_interval(SENSOR_UPDATE_INTERVAL_SECONDS * 1000);
    bc_module_co2_set_event_handler(co2_module_event_handler, NULL);

    bc_tag_temperature_init(&temperature_tag_internal, BC_I2C_I2C0, BC_TAG_TEMPERATURE_I2C_ADDRESS_ALTERNATE);
    bc_tag_temperature_set_update_interval(&temperature_tag_internal, SENSOR_UPDATE_INTERVAL_SECONDS * 1000);
    bc_tag_temperature_set_event_handler(&temperature_tag_internal, NULL, NULL);

    bc_tag_temperature_init(&temperature_tag, BC_I2C_I2C0, BC_TAG_TEMPERATURE_I2C_ADDRESS_DEFAULT);
    bc_tag_temperature_set_update_interval(&temperature_tag, SENSOR_UPDATE_INTERVAL_SECONDS * 1000);
    bc_tag_temperature_set_event_handler(&temperature_tag, temperature_tag_event_handler, NULL);

    bc_tag_humidity_init(&humidity_tag, BC_TAG_HUMIDITY_REVISION_R2, BC_I2C_I2C0, BC_TAG_HUMIDITY_I2C_ADDRESS_DEFAULT);
    bc_tag_humidity_set_update_interval(&humidity_tag, SENSOR_UPDATE_INTERVAL_SECONDS * 1000);
    bc_tag_humidity_set_event_handler(&humidity_tag, humidity_tag_event_handler, NULL);

    bc_tag_barometer_init(&barometer_tag, BC_I2C_I2C0);
    bc_tag_barometer_set_update_interval(&barometer_tag, SENSOR_UPDATE_INTERVAL_SECONDS * 1000);
    bc_tag_barometer_set_event_handler(&barometer_tag, barometer_tag_event_handler, NULL);

    bc_led_set_mode(&led, BC_LED_MODE_OFF);

    bc_scheduler_register(timer_task, NULL, FIRST_REPORT_SECONDS * 1000);
}
