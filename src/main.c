/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "hardware/pio.h"
#include "pico/stdlib.h"
// Our assembled program:
#include "hello.pio.h"

int main() {
  stdio_init_all();

  // Choose which PIO instance to use (there are two instances)
  PIO pio = pio0;

  // Our assembled program needs to be loaded into this PIO's instruction
  // memory. This SDK function will find a location (offset) in the
  // instruction memory where there is enough space for our program. We need
  // to remember this location!
  uint offset = pio_add_program(pio, &hello_program);

  // Find a free state machine on our chosen PIO (erroring if there are
  // none). Configure it to run our program, and start it, using the
  // helper function we included in our .pio file.
  uint sm1 = pio_claim_unused_sm(pio, true);
  hello_program_init(pio, sm1, offset, 2);

  // an another
  uint sm2 = pio_claim_unused_sm(pio, true);
  hello_program_init(pio, sm2, offset, 3);

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
