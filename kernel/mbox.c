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
            uart_puts(">> Got successful response \n\n");

        return (mBuf[1] == MBOX_RESPONSE);
    }

    return 0;
}

void get_board_info()
{
    display_start("BOARD INFORMATION");

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

    char board_info[18];

    // Call mailbox_call function
    if (mbox_call(ADDR(mBuf), MBOX_CH_PROP))
    {
        uart_puts("Board Revision: ");
        // uart_print_revision(mBuf[5], board_info);
        uart_puts("\n");
        uart_puts("Board MAC Address: ");
        uart_print_mac_address(mBuf[5], mBuf[6], board_info);
    }
    else
        (uart_puts("Board Revision is not found."));

    display_end();
}

void uart_print_mac_address(unsigned int mBuf5, unsigned int mBuf6, char *board_info)
{
    unsigned char bytes[6] = {
        (unsigned char)(mBuf5 & 0xFF),
        (unsigned char)((mBuf5 >> 8) & 0xFF),
        (unsigned char)((mBuf5 >> 16) & 0xFF),
        (unsigned char)((mBuf5 >> 24) & 0xFF),
        (unsigned char)(mBuf6 & 0xFF),
        (unsigned char)((mBuf6 >> 8) & 0xFF)};

    const char hexChars[] = "0123456789ABCDEF";

    for (int i = 0; i < 6; i++)
    {
        board_info[i * 3] = hexChars[bytes[i] >> 4];
        board_info[i * 3 + 1] = hexChars[bytes[i] & 0x0F];
        board_info[i * 3 + 2] = (i < 5) ? ':' : '\0';
    }

    uart_puts(board_info);
    uart_puts("\n");
}

void uart_print_revision(unsigned int mBuf5, char *board_info)
{

    // for (int i = 0; i < 8; i++)
    // {
    //     unsigned char nibble = (mBuf5 >> (4 (7 - i))) & 0x0F;
    //     board_info[i] = nibble < 10 ? nibble + '0' : nibble - 10 + 'A';
    // }
    // board_info[8] = '\0';

    // // Initialize the index to 0
    // int index = 0;
    // // Loop until the end of the string
    // while (board_info[index] == '0')
    // {
    //     // Move to the next character
    //     index++;
    // }
    // // Shift the remaining characters to the left
    // for (int i = 0; i < strlen(board_info) - index; i++)
    // {
    //     board_info[i] = board_info[i + index];
    // }
    // // Add a null character at the end of the string
    // board_info[strlen(board_info) - index] = '\0';
}
