/*
 * log.h
 *
 *  Created on: 20 дек. 2022 г.
 *      Author: АлексанDOOR
 */

#ifndef LOG_H_
#define LOG_H_

#include <stdarg.h>
#include <stdio.h>

#include "lwprintf/lwprintf.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define    log_printf_crlf(fmt, ...)               do { log_printf((fmt), ## __VA_ARGS__); log_printf("\r\n"); } while (0)

void log_init(void);
int log_printf(const char *fmt, ...);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LOG_H_ */
