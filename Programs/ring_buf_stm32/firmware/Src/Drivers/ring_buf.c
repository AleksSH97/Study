/*
 * ring_buf.c
 *
 *  Created on: Jan 16, 2023
 *      Author: АлексанDOOR
 */

#include "ring_buf.h"

void ring_buf_init()
{
    lwrb_init(&data_uart.lwrb_msg, data_uart.buff_data, sizeof(data_uart.buff_data));
    lwrb_free(&data_uart.lwrb_msg);
    lwrb_set_evt_fn(&data_uart.lwrb_msg, ring_buf_evt_callback);
}

bool ring_buf_write(struct uart *self, const void *data, size_t num_byte_to_write)
{
    lwrb_write(&self->lwrb_msg, data, num_byte_to_write);
    // TODO think about false statement

    return true;
}

bool ring_buf_read(struct uart *self, void *data, size_t num_byte_to_write)
{
    lwrb_read(&self->lwrb_msg, data, num_byte_to_write);
    // TODO think about false statement

    return true;
}

void ring_buf_free(struct uart *self)
{
    lwrb_free(&self->lwrb_msg);
}

bool ring_buf_ready(struct uart *self)
{
    return lwrb_is_ready(&self->lwrb_msg);
}

void ring_buf_peek(const struct uart *self, size_t skip_count, void *data, size_t byte_to_peek)
{
    lwrb_peek(&self->lwrb_msg, skip_count, data, byte_to_peek);
}

void ring_buf_evt_callback(struct uart *self, lwrb_evt_type_t evt, size_t bp)
{
    UNUSED(bp);

    switch(evt) {
        case LWRB_EVT_READ:
            break;
        case LWRB_EVT_WRITE:
            break;
        case LWRB_EVT_RESET:
            break;
    }
}
