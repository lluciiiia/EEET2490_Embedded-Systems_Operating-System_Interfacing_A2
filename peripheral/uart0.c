#include "../header/uart0.h"

/**
 * Set baud rate and characteristics and map to GPIO
 */
void uart_init()
{
	unsigned int r;

	/* Turn off UART0 */
	UART0_CR = 0x0;

	/* Setup GPIO pins 36 and 37 */

	/* Set GPIO36 and GPIO37 to be pl011 TX/RX which is ALT0	*/
	r = GPFSEL3;
	r &= ~((7 << 18) | (7 << 21));		// clear bits 23-18 (FSEL37, FSEL36)
	r |= (0b110 << 18) | (0b110 << 21); // Set value 0b110 (select ALT2: TXD0/RXD0)
	GPFSEL3 = r;

	/* enable GPIO 36, 37 */
#ifdef RPI3	   // RPI3
	GPPUD = 0; // No pull up/down control
	// Toogle clock to flush GPIO setup
	r = 150;
	while (r--)
	{
		asm volatile("nop");
	}								 // waiting 150 cycles
	GPPUDCLK1 = (1 << 4) | (1 << 5); // enable clock for GPIO 36, 37
	r = 150;
	while (r--)
	{
		asm volatile("nop");
	}			   // waiting 150 cycles
	GPPUDCLK1 = 0; // flush GPIO setup

#else // RPI4
	r = GPIO_PUP_PDN_CNTRL_REG2;
	r &= ~((3 << 8) | (3 << 10)); // No resistor is selected for GPIO 36, 37
	GPIO_PUP_PDN_CNTRL_REG2 = r;
#endif

	/* Mask all interrupts. */
	UART0_IMSC = 0;

	/* Clear pending interrupts. */
	UART0_ICR = 0x7FF;

	/* Set integer & fractional part of Baud rate
	Divider = UART_CLOCK/(16 * Baud)
	Default UART_CLOCK = 48MHz (old firmware it was 3MHz);
	Integer part register UART0_IBRD  = integer part of Divider
	Fraction part register UART0_FBRD = (Fractional part * 64) + 0.5 */

	// 19200 baud
	UART0_IBRD = 156;
	UART0_FBRD = 17;

	/* Set up the Line Control Register */
	/* Enable FIFO */
	/* Set length to 8 bit */
	/* Defaults for other bit are No parity, 1 stop bit */
	UART0_LCRH = UART0_LCRH_FEN | UART0_LCRH_WLEN_8BIT;

	/* Enable UART0, receive, and transmit */
	UART0_CR = 0x301; // enable Tx, Rx, FIFO
}

/**
 * Send a character
 */
void uart_sendc(char c)
{

	/* Check Flags Register */
	/* And wait until transmitter is not full */
	do
	{
		asm volatile("nop");
	} while (UART0_FR & UART0_FR_TXFF);

	/* Write our data byte out to the data register */
	UART0_DR = c;
}

/**
 * Receive a character
 */
char uart_getc()
{
	char c = 0;

	/* Check Flags Register */
	/* Wait until Receiver is not empty
	 * (at least one byte data in receive fifo)*/
	do
	{
		asm volatile("nop");
	} while (UART0_FR & UART0_FR_RXFE);

	/* read it and return */
	c = (unsigned char)(UART0_DR);

	/* convert carriage return to newline */
	return (c == '\r' ? '\n' : c);
}

/**
 * Display a string
 */
void uart_puts(char *s)
{
	while (*s)
	{
		/* convert newline to carriage return + newline */
		if (*s == '\n')
			uart_sendc('\r');
		uart_sendc(*s++);
	}
}

/**
 * Display a value in hexadecimal format
 */
void uart_hex(unsigned int num)
{
	uart_puts("0x");
	for (int pos = 28; pos >= 0; pos = pos - 4)
	{

		// Get highest 4-bit nibble
		char digit = (num >> pos) & 0xF;

		/* Convert to ASCII code */
		// 0-9 => '0'-'9', 10-15 => 'A'-'F'
		digit += (digit > 9) ? (-10 + 'A') : '0';
		uart_sendc(digit);
	}
}

/*
**
* Display a value in decimal format
*/
void uart_dec(int num)
{
	// A string to store the digit characters
	char str[33] = "";

	// Calculate the number of digits
	int len = 1;
	int temp = num;
	while (temp >= 10)
	{
		len++;
		temp = temp / 10;
	}

	// Store into the string and print out
	for (int i = 0; i < len; i++)
	{
		int digit = num % 10; // get last digit
		num = num / 10;		  // remove last digit from the number
		str[len - (i + 1)] = digit + '0';
	}
	str[len] = '\0';

	uart_puts(str);
}

void uart_hex_byte(uint8_t byte)
{
	// Print the high nibble
	uart_sendc(hex_to_ascii(byte >> 4));

	// Print the low nibble
	uart_sendc(hex_to_ascii(byte & 0x0F));
}

void set_baud_rate_command(char *arg)
{
	unsigned int baud_rate = atoi(arg); // Convert string to integer

	// Calculate the baud rate divisor
	float BAUDDIVs = UART0_CLOCK_FREQ / (16.0f * baud_rate);

	// Set the integer and fractional parts of the baud rate divisor
	UART0_IBRD = (int)BAUDDIVs;
	UART0_FBRD = (int)((BAUDDIVs - UART0_IBRD) * 64 + 0.5);

	UART0_CR &= ~UART0_CR_UARTEN; // Disable UART0
	UART0_CR = 0x0;				  // Clear the control register

	// Set baud rate and characteristics
	UART0_IBRD = (int)BAUDDIVs;
	UART0_FBRD = (int)(((BAUDDIVs - UART0_IBRD) * 64) + 0.5);

	UART0_LCRH = (UART0_LCRH & ~UART0_LCRH_BRK) | UART0_LCRH_WLEN_8BIT;
	UART0_CR = UART0_CR_UARTEN | UART0_CR_TXE | UART0_CR_RXE; // Enable UART0, Tx, Rx

	uart_puts("\nBaud Rate has been set to ");
	uart_dec(baud_rate);
	uart_puts("\nIBRD: ");
	uart_dec(UART0_IBRD);
	uart_puts("\nFBRD: ");
	uart_dec(UART0_FBRD);
	uart_puts("\n");
}

void set_data_bits_command(char *arg)
{
	// Get the number of data bits from the argument
	int data_bits = atoi(arg);

	// Update the Line Control Register (LCRH) accordingly
	switch (data_bits)
	{
	case 5:
		UART0_LCRH = UART0_LCRH_FEN | UART0_LCRH_WLEN_5BIT;
		break;
	case 6:
		UART0_LCRH = UART0_LCRH_FEN | UART0_LCRH_WLEN_6BIT;
		break;
	case 7:
		UART0_LCRH = UART0_LCRH_FEN | UART0_LCRH_WLEN_7BIT;
		break;
	case 8:
		UART0_LCRH = UART0_LCRH_FEN | UART0_LCRH_WLEN_8BIT;
		break;
	default:
		// Invalid number of data bits
		uart_puts("\nInvalid number of data bits.\n");
		return;
	}
	uart_puts("\nThe number of data bits has been set to ");
	uart_hex(UART0_LCRH);
	uart_puts("\n");
}

void set_stop_bits_command(char *arg)
{
	uart_puts("\nLDRH before setting stop bits: ");
	uart_hex(UART0_LCRH);
	// Get the number of stop bits from the argument
	int stop_bits = atoi(arg);

	UART0_LCRH &= ~UART0_LCRH_STP2;

	// Update the Line Control Register (LCRH) accordingly
	switch (stop_bits)
	{
	case 1:
		break;
	case 2:
		UART0_LCRH |= UART0_LCRH_STP2;
		break;
	default:
		// Invalid number of stop bits
		uart_puts("\nInvalid number of stop bits.\n");
		return;
	}
	uart_puts("\nLDRH after setting stop bits: ");
	uart_hex(UART0_LCRH);
	uart_puts("\n");
}

void set_parity_command(char *arg)
{
	uart_puts("\nLDRH before setting parity: ");
	uart_hex(UART0_LCRH);

	if (compare_string(arg, "none") == 0)
	{
		UART0_LCRH &= ~(UART0_LCRH_PEN | UART0_LCRH_EPS);
	}
	else if (compare_string(arg, "even") == 0)
	{
		UART0_LCRH |= UART0_LCRH_PEN | UART0_LCRH_EPS;
	}
	else if (compare_string(arg, "odd") == 0)
	{
		UART0_LCRH |= UART0_LCRH_PEN;
		UART0_LCRH &= ~UART0_LCRH_EPS;
	}
	else
	{
		uart_puts("\nInvalid parity type. Please use 'none', 'even', or 'odd'.\n");
		return;
	}

	uart_puts("\nLDRH after setting parity: ");
	uart_hex(UART0_LCRH);
	uart_puts("\nParity has been set to ");
	uart_puts(arg);
	uart_puts("\n");
}

void set_handshaking_command(char *arg)
{
}