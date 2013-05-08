//
//  p20Embedded.c
//  P20 project
//
// This program recieves a number over USART or SPI and
// then encrypts that number using a simple function.
//
//  Created by Joel Clark on 07/05/2013.
//
//

#include <stdio.h>
#include <math.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void mcu_init();
void uart0_init(void);
void uart_transmit(unsigned char data);
void button_init(void);
void put_str(char *str);
int encrypt_number(char x);

volatile unsigned char data = 'c';
volatile unsigned char new_char = 0;

ISR(USART0_RX_vect) {
    data = UDR0;
    new_char = 1;
}

ISR(INT0_vect, ISR_BLOCK) {
    _delay_ms(100);
    
    if (!(PINB & _BV(PB2))) {
        uart_transmit('+');
    }
}

int main() {
    mcu_init();
    put_str("initialised...)\n");
    
    while (1) {
        if (new_char) {
            // encrypt the number
            data = encrypt_number((int)data);
            uart_transmit(data);
            new_char = 0;
        }
    }
    
    return 1;
}

int encrypt_number(char x) {
    x -= 48;
    x = (1664525*x + 1013904223) % (int)(pow(2, 32));
    return x += 48;
    
}

void put_str(char *str) {
	int i;
	for(i=0; str[i]; i++) uart_transmit(str[i]);
}

void uart0_init(void) {
	/* Configure 9600 baud, 8-bit, no parity and one stop bit */
	const int baud_rate = 9600;
	UBRR0H = (F_CPU/(baud_rate*16L)-1) >> 8;
	UBRR0L = (F_CPU/(baud_rate*16L)-1);
	UCSR0B = _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0);
	UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

void button_init(void) {
    // set pin direction
    DDRD &= _BV(PD2);
    PORTD |= _BV(PD2);
    
    // set up interrupts
    EICRA = 0x00;
    EICRA = _BV(ISC01);
    EIMSK = _BV(INT0);
}

void uart_transmit(unsigned char data) {
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & _BV(UDRE0)) );
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

void mcu_init() {
    
    uart0_init();
    button_init();
    sei();
    
}

