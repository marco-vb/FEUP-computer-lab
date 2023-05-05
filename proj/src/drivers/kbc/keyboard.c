#include <lcom/lcf.h>

#include <stdint.h>
#include <stdbool.h>

#include "kbc.h"
#include "keyboard.h"

static int hook_keyboard = 1;
keyboard_packet_t keyboard_packet;

int (keyboard_subscribe_int)(uint8_t* bit_no) {
    *bit_no = BIT(hook_keyboard);
    return sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_keyboard);
}

int (keyboard_unsubscribe_int)() {
    return sys_irqrmpolicy(&hook_keyboard);
}

void (k_read_byte)(uint8_t b) {
    keyboard_packet.data[keyboard_packet.byte] = b;
    if (b == TWO_BYTES) {
        keyboard_packet.complete = false;
        keyboard_packet.byte++;
    }
    else {
        keyboard_packet.complete = true;
        keyboard_packet.byte = 0;
    }
}

void (keyboard_ih)() {
    uint8_t st, byte;
    if (kbc_get_status(&st) != F_OK) {
        printf("Error getting keyboard status.\n");
        return;
    }

    if (st & KBC_ERROR & MOUSE_DATA) {
        return;
    }

    if (st & KBC_OUT_FULL) {
        kbc_read_data(&byte);
        k_read_byte(byte);
    }
}

int (keyboard_restore_int)() {
    uint8_t cmd;

    kbc_write_cmd(KBC_READ_CMD);
    kbc_read_data(&cmd);

    cmd |= KBC_INT;

    kbc_write_cmd(KBC_WRITE_CMD);
    kbc_write_arg(cmd);

    return 0;
}
