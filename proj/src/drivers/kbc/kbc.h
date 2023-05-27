#ifndef __KBC_H
#define __KBC_H

#include <stdint.h>

#include "i8042.h"

/* Functions of KBC, common to keyboard and mouse */

/**
 * @brief get keyboard status
 * @param st pointer to the variable where the status will be stored
 * @return int  1 if an error occurs whiile reading the status 0 otherwise
 * 
 */
int (kbc_get_status)(uint8_t* st);


int (kbc_read_data)(uint8_t* data);
int (kbc_write)(uint8_t port, uint8_t cmd);
int (kbc_write_cmd)(uint8_t cmd);
int (kbc_write_arg)(uint8_t arg);
int (obf_full)();
int (ibf_empty)();
int (buffer_full)(uint8_t flag);
int (kbc_init)(uint8_t* irq_keyboard, uint8_t* irq_mouse);
int (kbc_disable)();

#endif /* __KBC_H */
