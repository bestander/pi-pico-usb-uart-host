#include <stdio.h>
#include "pico/stdlib.h"
#include "tusb.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include <string.h>

#define UART_ID uart1
#define BAUD_RATE 9600
#define UART_TX_PIN 8  // TX
#define UART_RX_PIN 9  // RX
#define LED_PIN 25

// Buffer for UART received data (optional, for future bidirectional use)
uint8_t uart_rx_buf[32];
volatile size_t uart_rx_len = 0;

void blink_led() {
    gpio_put(LED_PIN, 1);
    sleep_ms(100);
    gpio_put(LED_PIN, 0);
}

// Handle UART received data (optional, kept for debugging)
void uart_receive() {
    while (uart_is_readable(UART_ID) && uart_rx_len < sizeof(uart_rx_buf) - 1) {
        uart_rx_buf[uart_rx_len++] = uart_getc(UART_ID);
        printf("Received on Pico UART: %c (0x%02X)\n", uart_rx_buf[uart_rx_len - 1], uart_rx_buf[uart_rx_len - 1]);
    }
    uart_rx_buf[uart_rx_len] = 0;
    if (uart_rx_len > 0) {
        printf("Full message received: %s\n", uart_rx_buf);
        uart_rx_len = 0;
    }
}

void usb_host_init(void) {
    tusb_init();
    printf("TinyUSB Host Initialized\n");
}

void usb_host_task(void) {
    tuh_task();
}

void tuh_mount_cb(uint8_t dev_addr) {
    printf("USB Device Connected: Address = %u\n", dev_addr);
    tuh_hid_receive_report(dev_addr, 0);
}

void tuh_umount_cb(uint8_t dev_addr) {
    printf("USB Device Disconnected: Address = %u\n", dev_addr);
}

void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len) {
    (void)instance;

    uint8_t modifiers = report[0];
    char key_text[16] = "";
    bool key_found = false;
    for (int i = 2; i < 8; i++) {
        if (report[i] == 0) continue;
        bool shift = (modifiers & 0x22);
        switch (report[i]) {
            case 0x04: snprintf(key_text, sizeof(key_text), "%c", shift ? 'A' : 'a'); key_found = true; break;
            case 0x05: snprintf(key_text, sizeof(key_text), "%c", shift ? 'B' : 'b'); key_found = true; break;
            case 0x06: snprintf(key_text, sizeof(key_text), "%c", shift ? 'C' : 'c'); key_found = true; break;
            case 0x07: snprintf(key_text, sizeof(key_text), "%c", shift ? 'D' : 'd'); key_found = true; break;
            case 0x08: snprintf(key_text, sizeof(key_text), "%c", shift ? 'E' : 'e'); key_found = true; break;
            case 0x09: snprintf(key_text, sizeof(key_text), "%c", shift ? 'F' : 'f'); key_found = true; break;
            case 0x0A: snprintf(key_text, sizeof(key_text), "%c", shift ? 'G' : 'g'); key_found = true; break;
            case 0x0B: snprintf(key_text, sizeof(key_text), "%c", shift ? 'H' : 'h'); key_found = true; break;
            case 0x0C: snprintf(key_text, sizeof(key_text), "%c", shift ? 'I' : 'i'); key_found = true; break;
            case 0x0D: snprintf(key_text, sizeof(key_text), "%c", shift ? 'J' : 'j'); key_found = true; break;
            case 0x0E: snprintf(key_text, sizeof(key_text), "%c", shift ? 'K' : 'k'); key_found = true; break;
            case 0x0F: snprintf(key_text, sizeof(key_text), "%c", shift ? 'L' : 'l'); key_found = true; break;
            case 0x10: snprintf(key_text, sizeof(key_text), "%c", shift ? 'M' : 'm'); key_found = true; break;
            case 0x11: snprintf(key_text, sizeof(key_text), "%c", shift ? 'N' : 'n'); key_found = true; break;
            case 0x12: snprintf(key_text, sizeof(key_text), "%c", shift ? 'O' : 'o'); key_found = true; break;
            case 0x13: snprintf(key_text, sizeof(key_text), "%c", shift ? 'P' : 'p'); key_found = true; break;
            case 0x14: snprintf(key_text, sizeof(key_text), "%c", shift ? 'Q' : 'q'); key_found = true; break;
            case 0x15: snprintf(key_text, sizeof(key_text), "%c", shift ? 'R' : 'r'); key_found = true; break;
            case 0x16: snprintf(key_text, sizeof(key_text), "%c", shift ? 'S' : 's'); key_found = true; break;
            case 0x17: snprintf(key_text, sizeof(key_text), "%c", shift ? 'T' : 't'); key_found = true; break;
            case 0x18: snprintf(key_text, sizeof(key_text), "%c", shift ? 'U' : 'u'); key_found = true; break;
            case 0x19: snprintf(key_text, sizeof(key_text), "%c", shift ? 'V' : 'v'); key_found = true; break;
            case 0x1A: snprintf(key_text, sizeof(key_text), "%c", shift ? 'W' : 'w'); key_found = true; break;
            case 0x1B: snprintf(key_text, sizeof(key_text), "%c", shift ? 'X' : 'x'); key_found = true; break;
            case 0x1C: snprintf(key_text, sizeof(key_text), "%c", shift ? 'Y' : 'y'); key_found = true; break;
            case 0x1D: snprintf(key_text, sizeof(key_text), "%c", shift ? 'Z' : 'z'); key_found = true; break;
            case 0x1E: snprintf(key_text, sizeof(key_text), "%c", shift ? '!' : '1'); key_found = true; break;
            case 0x1F: snprintf(key_text, sizeof(key_text), "%c", shift ? '@' : '2'); key_found = true; break;
            case 0x20: snprintf(key_text, sizeof(key_text), "%c", shift ? '#' : '3'); key_found = true; break;
            case 0x21: snprintf(key_text, sizeof(key_text), "%c", shift ? '$' : '4'); key_found = true; break;
            case 0x22: snprintf(key_text, sizeof(key_text), "%c", shift ? '%' : '5'); key_found = true; break;
            case 0x23: snprintf(key_text, sizeof(key_text), "%c", shift ? '^' : '6'); key_found = true; break;
            case 0x24: snprintf(key_text, sizeof(key_text), "%c", shift ? '&' : '7'); key_found = true; break;
            case 0x25: snprintf(key_text, sizeof(key_text), "%c", shift ? '*' : '8'); key_found = true; break;
            case 0x26: snprintf(key_text, sizeof(key_text), "%c", shift ? '(' : '9'); key_found = true; break;
            case 0x27: snprintf(key_text, sizeof(key_text), "%c", shift ? ')' : '0'); key_found = true; break;
            case 0x28: snprintf(key_text, sizeof(key_text), "Enter"); key_found = true; break;
            case 0x29: snprintf(key_text, sizeof(key_text), "Esc"); key_found = true; break;
            case 0x2A: snprintf(key_text, sizeof(key_text), "Backspace"); key_found = true; break;
            case 0x2B: snprintf(key_text, sizeof(key_text), "Tab"); key_found = true; break;
            case 0x2C: snprintf(key_text, sizeof(key_text), "Space"); key_found = true; break;
            case 0x2D: snprintf(key_text, sizeof(key_text), "%c", shift ? '_' : '-'); key_found = true; break;
            case 0x2E: snprintf(key_text, sizeof(key_text), "%c", shift ? '+' : '='); key_found = true; break;
            case 0x2F: snprintf(key_text, sizeof(key_text), "%c", shift ? '{' : '['); key_found = true; break;
            case 0x30: snprintf(key_text, sizeof(key_text), "%c", shift ? '}' : ']'); key_found = true; break;
            case 0x31: snprintf(key_text, sizeof(key_text), "%c", shift ? '|' : '\\'); key_found = true; break;
            case 0x32: snprintf(key_text, sizeof(key_text), "%c", shift ? '~' : '#'); key_found = true; break;
            case 0x33: snprintf(key_text, sizeof(key_text), "%c", shift ? ':' : ';'); key_found = true; break;
            case 0x34: snprintf(key_text, sizeof(key_text), "%c", shift ? '"' : '\''); key_found = true; break;
            case 0x35: snprintf(key_text, sizeof(key_text), "%c", shift ? '~' : '`'); key_found = true; break;
            case 0x36: snprintf(key_text, sizeof(key_text), "%c", shift ? '<' : ','); key_found = true; break;
            case 0x37: snprintf(key_text, sizeof(key_text), "%c", shift ? '>' : '.'); key_found = true; break;
            case 0x38: snprintf(key_text, sizeof(key_text), "%c", shift ? '?' : '/'); key_found = true; break;
            case 0x39: snprintf(key_text, sizeof(key_text), "Caps Lock"); key_found = true; break;
            case 0x3A: snprintf(key_text, sizeof(key_text), "F1"); key_found = true; break;
            case 0x3B: snprintf(key_text, sizeof(key_text), "F2"); key_found = true; break;
            case 0x3C: snprintf(key_text, sizeof(key_text), "F3"); key_found = true; break;
            case 0x3D: snprintf(key_text, sizeof(key_text), "F4"); key_found = true; break;
            case 0x3E: snprintf(key_text, sizeof(key_text), "F5"); key_found = true; break;
            case 0x3F: snprintf(key_text, sizeof(key_text), "F6"); key_found = true; break;
            case 0x40: snprintf(key_text, sizeof(key_text), "F7"); key_found = true; break;
            case 0x41: snprintf(key_text, sizeof(key_text), "F8"); key_found = true; break;
            case 0x42: snprintf(key_text, sizeof(key_text), "F9"); key_found = true; break;
            case 0x43: snprintf(key_text, sizeof(key_text), "F10"); key_found = true; break;
            case 0x44: snprintf(key_text, sizeof(key_text), "F11"); key_found = true; break;
            case 0x45: snprintf(key_text, sizeof(key_text), "F12"); key_found = true; break;
            case 0x4F: snprintf(key_text, sizeof(key_text), "Right Arrow"); key_found = true; break;
            case 0x50: snprintf(key_text, sizeof(key_text), "Left Arrow"); key_found = true; break;
            case 0x51: snprintf(key_text, sizeof(key_text), "Down Arrow"); key_found = true; break;
            case 0x52: snprintf(key_text, sizeof(key_text), "Up Arrow"); key_found = true; break;
            default: snprintf(key_text, sizeof(key_text), "Unknown (0x%02x)", report[i]); key_found = true; break;
        }
        if (key_found) {
            printf("Sending to UART: %s\\n\n", key_text);
            uart_puts(UART_ID, key_text);
            uart_puts(UART_ID, "\n");
            blink_led();
            break;
        }
    }
    printf("%s\n", key_text);
    tuh_hid_receive_report(dev_addr, instance);
}

int main() {
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0);

    // Initialize UART
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    uart_set_hw_flow(UART_ID, false, false);
    uart_set_format(UART_ID, 8, 1, UART_PARITY_NONE);
    if (uart_is_enabled(UART_ID)) {
        printf("UART Initialized on GPIO %d (TX), %d (RX) at %d baud\n", UART_TX_PIN, UART_RX_PIN, BAUD_RATE);
    } else {
        printf("UART1 failed to initialize\n");
    }

    // Initialize USB Host
    printf("Initializing USB Host...\n");
    usb_host_init();

    printf("Waiting for USB keyboard input...\n");
    while (1) {
        usb_host_task();
        uart_receive();
        sleep_ms(10);
    }
    return 0;
}