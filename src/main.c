/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "hardware/pio.h"
#include "pico/stdlib.h"
// Our assembled program:
#include "blink.pio.h"

static inline void blink_pio_init(PIO pio, uint sm, uint offset, uint pin) {
  pio_sm_config c = blink_program_get_default_config(offset);
  // Map the state machine's OUT pin group to one pin, namely the `pin`
  // parameter to this function.
  sm_config_set_out_pins(&c, pin, 1);
  // Set this pin's GPIO function (connect PIO to the pad)
  pio_gpio_init(pio, pin);
  // Set the pin direction to output at the PIO
  pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);
  // Load our configuration, and jump to the start of the program
  pio_sm_init(pio, sm, offset, &c);
  // Set the state machine running
  pio_sm_set_enabled(pio, sm, true);
}

int main() {
  stdio_init_all();

  // Choose which PIO instance to use (there are two instances)
  PIO pio = pio0;

  // Our assembled program needs to be loaded into this PIO's instruction
  // memory. This SDK function will find a location (offset) in the
  // instruction memory where there is enough space for our program. We need
  // to remember this location!
  uint offset = pio_add_program(pio, &blink_program);

  // Find a free state machine on our chosen PIO (erroring if there are
  // none). Configure it to run our program, and start it, using the
  // helper function we included in our .pio file.
  uint sm1 = pio_claim_unused_sm(pio, true);
  blink_pio_init(pio, sm1, offset, 2);

  // an another
  uint sm2 = pio_claim_unused_sm(pio, true);
  blink_pio_init(pio, sm2, offset, 3);

  // The state machine is now running. Any value we push to its TX FIFO will
  // appear on the LED pin.
  while (true) {
    // Blink
    pio_sm_put_blocking(pio, sm1, 1);
    pio_sm_put_blocking(pio, sm2, 0);
    sleep_ms(500);
    // Blonk
    pio_sm_put_blocking(pio, sm1, 0);
    pio_sm_put_blocking(pio, sm2, 1);
    sleep_ms(500);
  }
}
