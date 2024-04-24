// -----------------------------------mbox.c -------------------------------------
#include "../header/mbox.h"

/* Mailbox Data Buffer (each element is 32-bit)*/
/*
 * The keyword attribute allows you to specify special attributes
 *
 * The aligned(N) attribute aligns the current data item on an address
 * which is a multiple of N, by inserting padding bytes before the data item
 *
 * __attribute__((aligned(16)) : allocate the variable on a 16-byte boundary.
 * *
 * We must ensure that our our buffer is located at a 16 byte aligned address,
 * so only the high 28 bits contain the address
 * (last 4 bits is ZERO due to 16 byte alignment)
 *
 */
volatile unsigned int __attribute__((aligned(16))) mBuf[36];

/**
 * Read from the mailbox
 */
uint32_t mailbox_read(unsigned char channel)
{
    // Receiving message is buffer_addr & channel number
    uint32_t res;
    // Make sure that the message is from the right channel
    do
    {
        // Make sure there is mail to receive
        do
        {
            asm volatile("nop");
        } while (MBOX0_STATUS & MBOX_EMPTY);
        // Get the message
        res = MBOX0_READ;
    } while ((res & 0xF) != channel);

    return res;
}

/**
 * Write to the mailbox
 */
void mailbox_send(uint32_t msg, unsigned char channel)
{
    // Sending message is buffer_addr & channel number
    //  Make sure you can send mail
    do
    {
        asm volatile("nop");
    } while (MBOX1_STATUS & MBOX_FULL);
    // send the message
    MBOX1_WRITE = msg;
}

/**
 * Make a mailbox call. Returns 0 on failure, non-zero on success
 */
int mbox_call(unsigned int buffer_addr, unsigned char channel)
{
    // Check Buffer Address
    uart_sendc('\n');
    uart_puts("Buffer Address: ");
    uart_hex(buffer_addr);
    uart_sendc('\n');

    // Prepare Data (address of Message Buffer)
    unsigned int msg = (buffer_addr & ~0xF) | (channel & 0xF);
    mailbox_send(msg, channel);

    /* now wait for the response */
    /* is it a response to our message (same address)? */
    if (msg == mailbox_read(channel))
    {
        /* is it a valid successful response (Response Code) ? */
        if (mBuf[1] == MBOX_RESPONSE)
            uart_puts("Got successful response \n\n");

        return (mBuf[1] == MBOX_RESPONSE);
    }

    return 0;
}

void get_board_info()
{

    // Send request for board revision
    mBuf[0] = 11 * 4;       // Length of the buffer
    mBuf[1] = MBOX_REQUEST; // Request code

    mBuf[2] = MBOX_TAG_REVISION; // Tag: Get board revision
    mBuf[3] = 4;                 // Buffer size
    mBuf[4] = 0;                 // Request/response code
    mBuf[5] = 0;                 // Value buffer

    mBuf[6] = MBOX_TAG_MAC_ADDRESS; // Tag: Get board MAC address
    mBuf[3] = 4;                    // Buffer size
    mBuf[4] = 0;                    // Request/response code
    mBuf[5] = 0;                    // Value buffer
    mBuf[10] = MBOX_TAG_LAST;       // End of tags

    // Call mailbox_call function
    if (mbox_call(ADDR(mBuf), MBOX_CH_PROP))
    {
        uart_puts("Board Revision: ");
        uart_hex(mBuf[5]);
        uart_puts("\n");
        uart_puts("Board MAC Address: ");
        uart_print_mac_address(mBuf[9]);
    }
    else
        (uart_puts("Board Revision is not found."));

    uart_puts("\n");
}

void uart_print_mac_address(uint32_t mac_address) {
    // Extract each byte of the MAC address
    uint8_t byte1 = (mac_address >> 24) & 0xFF;
    uint8_t byte2 = (mac_address >> 16) & 0xFF;
    uint8_t byte3 = (mac_address >> 8) & 0xFF;
    uint8_t byte4 = mac_address & 0xFF;

    // Print the MAC address in the desired format
    uart_hex_byte(byte1);
    uart_sendc(':');
    uart_hex_byte(byte2);
    uart_sendc(':');
    uart_hex_byte(byte3);
    uart_sendc(':');
    uart_hex_byte(byte4);
}










