#include <stdio.h>
#include "pico/stdlib.h"
#include "tusb.h"

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
    if (len < 8) return; // Ensure minimum report length

    // Modifier keys (byte 0)
    uint8_t modifiers = report[0];
    if (modifiers & 0x01) printf("Left Ctrl ");
    if (modifiers & 0x02) printf("Left Shift ");
    if (modifiers & 0x04) printf("Left Alt ");
    if (modifiers & 0x08) printf("Left GUI ");
    if (modifiers & 0x10) printf("Right Ctrl ");
    if (modifiers & 0x20) printf("Right Shift ");
    if (modifiers & 0x40) printf("Right Alt ");
    if (modifiers & 0x80) printf("Right GUI ");

    // Key codes (bytes 2-7, skip byte 1 which is reserved)
    for (int i = 2; i < 8; i++) {
        if (report[i] == 0) continue; // No key
        bool shift = (modifiers & 0x22); // Left or Right Shift
        switch (report[i]) {
            case 0x04: printf("%c", shift ? 'A' : 'a'); break;
            case 0x05: printf("%c", shift ? 'B' : 'b'); break;
            case 0x06: printf("%c", shift ? 'C' : 'c'); break;
            case 0x07: printf("%c", shift ? 'D' : 'd'); break;
            case 0x08: printf("%c", shift ? 'E' : 'e'); break;
            case 0x09: printf("%c", shift ? 'F' : 'f'); break;
            case 0x0A: printf("%c", shift ? 'G' : 'g'); break;
            case 0x0B: printf("%c", shift ? 'H' : 'h'); break;
            case 0x0C: printf("%c", shift ? 'I' : 'i'); break;
            case 0x0D: printf("%c", shift ? 'J' : 'j'); break;
            case 0x0E: printf("%c", shift ? 'K' : 'k'); break;
            case 0x0F: printf("%c", shift ? 'L' : 'l'); break;
            case 0x10: printf("%c", shift ? 'M' : 'm'); break;
            case 0x11: printf("%c", shift ? 'N' : 'n'); break;
            case 0x12: printf("%c", shift ? 'O' : 'o'); break;
            case 0x13: printf("%c", shift ? 'P' : 'p'); break;
            case 0x14: printf("%c", shift ? 'Q' : 'q'); break;
            case 0x15: printf("%c", shift ? 'R' : 'r'); break;
            case 0x16: printf("%c", shift ? 'S' : 's'); break;
            case 0x17: printf("%c", shift ? 'T' : 't'); break;
            case 0x18: printf("%c", shift ? 'U' : 'u'); break;
            case 0x19: printf("%c", shift ? 'V' : 'v'); break;
            case 0x1A: printf("%c", shift ? 'W' : 'w'); break;
            case 0x1B: printf("%c", shift ? 'X' : 'x'); break;
            case 0x1C: printf("%c", shift ? 'Y' : 'y'); break;
            case 0x1D: printf("%c", shift ? 'Z' : 'z'); break;
            case 0x1E: printf("%c", shift ? '!' : '1'); break;
            case 0x1F: printf("%c", shift ? '@' : '2'); break;
            case 0x20: printf("%c", shift ? '#' : '3'); break;
            case 0x21: printf("%c", shift ? '$' : '4'); break;
            case 0x22: printf("%c", shift ? '%' : '5'); break;
            case 0x23: printf("%c", shift ? '^' : '6'); break;
            case 0x24: printf("%c", shift ? '&' : '7'); break;
            case 0x25: printf("%c", shift ? '*' : '8'); break;
            case 0x26: printf("%c", shift ? '(' : '9'); break;
            case 0x27: printf("%c", shift ? ')' : '0'); break;
            case 0x28: printf("Enter"); break;
            case 0x29: printf("Esc"); break;
            case 0x2A: printf("Backspace"); break;
            case 0x2B: printf("Tab"); break;
            case 0x2C: printf("Space"); break;
            case 0x2D: printf("%c", shift ? '_' : '-'); break;
            case 0x2E: printf("%c", shift ? '+' : '='); break;
            case 0x2F: printf("%c", shift ? '{' : '['); break;
            case 0x30: printf("%c", shift ? '}' : ']'); break;
            case 0x31: printf("%c", shift ? '|' : '\\'); break;
            case 0x32: printf("%c", shift ? '~' : '#'); break; // UK layout
            case 0x33: printf("%c", shift ? ':' : ';'); break;
            case 0x34: printf("%c", shift ? '"' : '\''); break;
            case 0x35: printf("%c", shift ? '~' : '`'); break; // US layout
            case 0x36: printf("%c", shift ? '<' : ','); break;
            case 0x37: printf("%c", shift ? '>' : '.'); break;
            case 0x38: printf("%c", shift ? '?' : '/'); break;
            case 0x39: printf("Caps Lock"); break;
            case 0x3A: printf("F1"); break;
            case 0x3B: printf("F2"); break;
            case 0x3C: printf("F3"); break;
            case 0x3D: printf("F4"); break;
            case 0x3E: printf("F5"); break;
            case 0x3F: printf("F6"); break;
            case 0x40: printf("F7"); break;
            case 0x41: printf("F8"); break;
            case 0x42: printf("F9"); break;
            case 0x43: printf("F10"); break;
            case 0x44: printf("F11"); break;
            case 0x45: printf("F12"); break;
            case 0x4F: printf("Right Arrow"); break;
            case 0x50: printf("Left Arrow"); break;
            case 0x51: printf("Down Arrow"); break;
            case 0x52: printf("Up Arrow"); break;
            default: printf("Unknown (0x%02x)", report[i]); break;
        }
    }
    printf("\n");
    tuh_hid_receive_report(dev_addr, instance); // Request next report
}

int main() {
    stdio_init_all();
    printf("Initializing USB Host...\n");
    usb_host_init();
    while (1) {
        usb_host_task();
    }
    return 0;
}