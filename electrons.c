// Copyright © 2014 Vernon Mauery <vernon@mauery.com>
// SPDX-License-Identifier: GPL-2.0-only

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <string.h>
#include "util.h"

//#include "usart.h"

#if 1
int main(void)
{
    uint8_t i, j;
	uint8_t d;
	uint8_t v[] = { 0, 0, 0, 0 };

	wdt_disable();                            // disable watchdogs
	sei();                                    // enable interrupts
	DDRC |= 0x0f;
//	usart_setup_static(b9600, data8, parity_none, stop1);
//	usart_puts_p(PSTR("hello world.\r\n"));

	v[0] = 0xff;
	while(1) {
		for (j=0; j<8; j++)
		{
			d = 0x0f;
			for (i=0; i<255; i++)
			{
				PORTC = 0x00;
				// we have four channels to modulate
				// each one starts at one value and then all just loop around
				if (i >= v[0])
					d &= ~(0x01);
				if (i >= v[1])
					d &= ~(0x02);
				if (i >= v[2])
					d &= ~(0x04);
				if (i >= v[3])
					d &= ~(0x08);
				PORTC = d;
			}
		}
		v[0] >>= 1;
		v[1] >>= 1;
		v[2] >>= 1;
		v[3] >>= 1;
		if (v[0] == 0x3f) v[1] = 0xff;
		if (v[1] == 0x3f) v[2] = 0xff;
		if (v[2] == 0x3f) v[3] = 0xff;
		if (v[3] == 0x3f) v[0] = 0xff;
    }
	return 0;
}
#else

int main(void)
{
    uint8_t i, j;
	uint8_t d;
	uint8_t v[] = { 0, 0, 0, 0 };

	wdt_disable();                            // disable watchdogs
	sei();                                    // enable interrupts
	DDRC |= 0x0f;
	usart_setup_static(b9600, data8, parity_none, stop1);
	usart_write((const uint8_t*)"hello world.\r\n", 14);

	v[0] = 0xff;
	while(1) {
		for (j=0; j<9; j++)
		{
			d = 0x0f;
			for (i=0; i<255; i++)
			{
				PORTC = 0x00;
				// we have four channels to modulate
				// each one starts at one value and then all just loop around
				if (i >= v[0])
					d &= ~(0x01);
				if (i >= v[1])
					d &= ~(0x02);
				if (i >= v[2])
					d &= ~(0x04);
				if (i >= v[3])
					d &= ~(0x08);
				PORTC = d;
			}
		}
		v[0] >>= 2;
		v[1] >>= 2;
		v[2] >>= 2;
		v[3] >>= 2;
		if (v[0] == 0x0f) v[1] = 0xff;
		if (v[1] == 0x0f) v[2] = 0xff;
		if (v[2] == 0x0f) v[3] = 0xff;
		if (v[3] == 0x0f) v[0] = 0xff;
		if (usart_have_rx())
			usart_write_byte(usart_read_byte());
    }
	return 0;
}

#endif
