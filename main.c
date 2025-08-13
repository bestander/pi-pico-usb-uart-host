#include <stdio.h>
#include "pico/stdlib.h"
#include "tusb.h"

#include "hardware/gpio.h"
#include "i2c_slave.h"
#include <string.h>

#define I2C_SLAVE_ADDR 0x20
#define I2C_SDA_PIN 6
#define I2C_SCL_PIN 7
#define LED_PIN 25


uint8_t i2c_rx_buf[32];
uint8_t i2c_tx_buf[32];
volatile uint32_t ping_count = 0;
volatile size_t i2c_rx_len = 0;

void blink_led() {
    gpio_put(LED_PIN, 1);
    sleep_ms(100);
    gpio_put(LED_PIN, 0);
}

// I2C slave event handler
void i2c_slave_event_handler(i2c_inst_t *i2c, i2c_slave_event_t event) {
    switch (event) {
        case I2C_SLAVE_RECEIVE: {
            // Read all available bytes from master
            size_t rx_avail = i2c_get_read_available(i2c);
            if (rx_avail > 0) {
                    i2c_read_raw_blocking(i2c, i2c_rx_buf, rx_avail);
                i2c_rx_buf[i2c_rx_len] = 0;
                blink_led();
            }
            break;
        }
        case I2C_SLAVE_REQUEST: {
            // Master is requesting data, send last pressed key
            size_t tx_len = strlen((char *)i2c_tx_buf);
            if (tx_len > 0) {
                i2c_write_raw_blocking(i2c, i2c_tx_buf, tx_len);
                blink_led();
            }
            break;
        }
        case I2C_SLAVE_FINISH:
            // Optionally handle stop/restart
            break;
    }
}

// No longer needed: I2C events are handled by i2c_slave_event_handler

void usb_host_init(void) {
    tusb_init();
    printf("TinyUSB Host Initialized\n");
}

void usb_host_task(void) {
    tuh_task();
}

void tuh_mount_cb(uint8_t dev_addr) {
    printf("USB Device Connected: Address = %u\n", dev_addr);
    tuh_hid_receive_report(dev_addr, 0); // Request HID reports
}

void tuh_umount_cb(uint8_t dev_addr) {
    printf("USB Device Disconnected: Address = %u\n", dev_addr);
}

void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len) {
    (void)instance;

    // Modifier keys (byte 0)
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
            case 0x34: snprintf(key_text, sizeof(key_text), "%c", shift ? '"' : '\'' ); key_found = true; break;
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
            strncpy((char *)i2c_tx_buf, key_text, sizeof(i2c_tx_buf));
            i2c_tx_buf[sizeof(i2c_tx_buf)-1] = 0;
            break;
        }
    }
    printf("%s\n", key_text);
    tuh_hid_receive_report(dev_addr, instance); // Request next report
}

int main() {
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0);

    // Initialize USB Host
    printf("Initializing USB Host...\n");
    usb_host_init();

    // Initialize I2C0 as slave
    i2c_init(i2c1, 100 * 1000); // 100kHz
    printf("Pico I2C Test initialized!!\n");
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);
    printf("Starting Pico I2C Test...\n");
    i2c_slave_init(i2c1, I2C_SLAVE_ADDR, i2c_slave_event_handler);
    printf("Waiting for I2C commands from ESP32...\n");

    while (1) {
        usb_host_task();
        sleep_ms(10);
    }
    return 0;
}