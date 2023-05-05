#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

#include "drivers/timer/timer.h"
#include "drivers/kbc/kbc.h"
#include "drivers/kbc/keyboard.h"
#include "drivers/kbc/mouse.h"
#include "drivers/graphics/graphics.h"

extern mouse_packet_t mouse_packet;
extern keyboard_packet_t keyboard_packet;


int main(int argc, char* argv[]) {
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");

    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need it]
    lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    lcf_log_output("/home/lcom/labs/lab2/output.txt");

    // handles control over to LCF
    // [LCF handles command line arguments and invokes the right function]
    if (lcf_start(argc, argv))
        return 1;

    // LCF clean up tasks
    // [must be the last statement before return]
    lcf_cleanup();

    return 0;
}

int (proj_main_loop)(int argc, char* argv[]) {
    int ipc_status, r;
    message msg;
    uint8_t irq_mouse, irq_timer, irq_keyboard;

    if (video_mode() != OK) {
        printf("Error setting video mode.\n");
        return 1;
    }

    if (mouse_subscribe_int(&irq_mouse)) {
        printf("Error subscribing mouse interrupts.\n");
        return 1;
    }

    if (timer_subscribe_int(&irq_timer)) {
        printf("Error subscribing timer interrupts.\n");
        return 1;
    }

    if (keyboard_subscribe_int(&irq_keyboard)) {
        printf("Error subscribing keyboard interrupts.\n");
        return 1;
    }

    if (mouse_enable_dr() != OK) {
        printf("Error enabling data reporting.\n");
        return 1;
    }

    do {
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("Error receiving message: %d\n", r);
            continue;
        }

        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE:
                if (msg.m_notify.interrupts & irq_mouse) {
                    mouse_ih();
                    if (mouse_packet.complete) {
                        video_draw_rectangle(50, 50, 200, 200, 0x0000FF);
                    }
                }

                if (msg.m_notify.interrupts & irq_timer) {
                    timer_ih();
                }

                if (msg.m_notify.interrupts & irq_keyboard) {
                    keyboard_ih();
                    if (keyboard_packet.complete) {
                        video_draw_rectangle(50, 50, 200, 200, 0x00FF00);
                    }
                }

                break;
            default:
                break;
            }
        }
    } while (keyboard_packet.data[0] != ESC_BREAK);

    if (mouse_disable_dr() != OK) {
        printf("Error disabling data reporting.\n");
        return 1;
    }

    if (mouse_unsubscribe_int()) {
        printf("Error unsubscribing mouse interrupts.\n");
        return 1;
    }

    if (timer_unsubscribe_int()) {
        printf("Error unsubscribing timer interrupts.\n");
        return 1;
    }

    if (keyboard_unsubscribe_int()) {
        printf("Error unsubscribing keyboard interrupts.\n");
        return 1;
    }

    if (text_mode() != OK) {
        printf("Error exiting video mode.\n");
        return 1;
    }

    return 0;
}
