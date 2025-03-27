#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"

#define LED_PIN 19
#define P10K_PIN 26
#define CYCLE_DELAY_MS 250

#define ADC_PIN_BASE 26

// Initialize the GPIO for the LED
uint pico_led_init(void) {
    // PWM configuration
    gpio_set_function(LED_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(LED_PIN);
    pwm_set_wrap(slice_num, 1<<12);
    pwm_set_chan_level(slice_num, PWM_CHAN_A, 1<<11);
    pwm_set_enabled(slice_num, true);
    return slice_num;
}

void p10k_init(int pin) {
    adc_init();
    adc_gpio_init(pin);
    adc_select_input(pin-ADC_PIN_BASE);
}

int main() {
    stdio_init_all();
    p10k_init(P10K_PIN);
    uint slice_num = pico_led_init();
    printf("Measuring ADC0:\n");
    while (true) {
        uint16_t gp26_reading = adc_read();
        float conversion_factor = 3.3f / (1<<12);
        printf("Raw value: 0x%03x, volatge: %1.3fV\n", gp26_reading, gp26_reading * conversion_factor);
        //pwm_set_chan_level(slice_num, PWM_CHAN_A, gp26_reading);
        pwm_set_gpio_level(LED_PIN,gp26_reading);
        sleep_ms(CYCLE_DELAY_MS);
    }
}
