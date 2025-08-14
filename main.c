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

    // Send raw report array to UART instead of resolving key_text here
    uart_write_blocking(UART_ID, report, len);
    blink_led();
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