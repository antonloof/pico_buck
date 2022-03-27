#include <stdio.h>
#include "pico/stdlib.h"
#include "sync_buck_pwm.pio.h"
#include "hardware/pio.h"

typedef struct sync_pwm_conf
{
    int start_pin;
    PIO pio;
    int word_size_bits;
    int sm, offset;
    pio_sm_config sm_config;

} sync_pwm_conf;

sync_pwm_conf spwm_conf = {20, pio1, 8};

void init_sync_buck(sync_pwm_conf *conf);
void set_spwm_blocking(sync_pwm_conf *conf, int value);

int main()
{
    init_sync_buck(&spwm_conf);
    set_spwm_blocking(&spwm_conf, 150);

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    while (1)
    {
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        sleep_ms(250);
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
        sleep_ms(250);
    }
    return 0;
}

void set_spwm_blocking(sync_pwm_conf *conf, int value)
{
    pio_sm_put_blocking(conf->pio, conf->sm, value & ((1 << conf->word_size_bits) - 1));
}

void init_sync_buck(sync_pwm_conf *conf)
{
    pio_gpio_init(conf->pio, conf->start_pin);
    pio_gpio_init(conf->pio, conf->start_pin + 1);
    pio_sm_set_consecutive_pindirs(conf->pio, conf->sm, conf->start_pin, 2, true);

    conf->sm = pio_claim_unused_sm(conf->pio, true);
    conf->offset = pio_add_program(conf->pio, &spwm_program);
    conf->sm_config = spwm_program_get_default_config(conf->offset);

    sm_config_set_clkdiv(&conf->sm_config, 2); // other value
    sm_config_set_sideset_pins(&conf->sm_config, conf->start_pin);
    sm_config_set_out_shift(&conf->sm_config, true, true, 32);
    sm_config_set_in_shift(&conf->sm_config, true, false, 32);
    pio_sm_init(conf->pio, conf->sm, conf->offset, &conf->sm_config);
    pio_sm_set_enabled(conf->pio, conf->sm, true);

    pio_sm_put_blocking(conf->pio, conf->sm, 1 << conf->word_size_bits);
}