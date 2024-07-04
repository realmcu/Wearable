#ifndef _LOG_ADAPTER_H_
#define _LOG_ADAPTER_H_

// include here
// #include "REG_BASE.h" // not a must

#ifdef __cplusplus
extern "C" {
#endif

void log_init(void);
void log_print(char *fmt, ...);

#define DBG_DIRECT log_print
#define APP_PRINT_ERROR0 log_print
#define APP_PRINT_ERROR1 log_print
#define APP_PRINT_ERROR2 log_print
#define APP_PRINT_ERROR3 log_print
#define APP_PRINT_ERROR4 log_print
#define APP_PRINT_ERROR5 log_print
#define APP_PRINT_ERROR6 log_print
#define APP_PRINT_ERROR7 log_print
#define APP_PRINT_ERROR8 log_print

#define APP_PRINT_WARN0 log_print
#define APP_PRINT_WARN1 log_print
#define APP_PRINT_WARN2 log_print
#define APP_PRINT_WARN3 log_print
#define APP_PRINT_WARN4 log_print
#define APP_PRINT_WARN5 log_print
#define APP_PRINT_WARN6 log_print
#define APP_PRINT_WARN7 log_print
#define APP_PRINT_WARN8 log_print

#define APP_PRINT_INFO0 log_print
#define APP_PRINT_INFO1 log_print
#define APP_PRINT_INFO2 log_print
#define APP_PRINT_INFO3 log_print
#define APP_PRINT_INFO4 log_print
#define APP_PRINT_INFO5 log_print
#define APP_PRINT_INFO6 log_print
#define APP_PRINT_INFO7 log_print
#define APP_PRINT_INFO8 log_print

#define APP_PRINT_TRACE0 log_print
#define APP_PRINT_TRACE1 log_print
#define APP_PRINT_TRACE2 log_print
#define APP_PRINT_TRACE3 log_print
#define APP_PRINT_TRACE4 log_print
#define APP_PRINT_TRACE5 log_print
#define APP_PRINT_TRACE6 log_print
#define APP_PRINT_TRACE7 log_print
#define APP_PRINT_TRACE8 log_print

#ifdef __cplusplus
}
#endif

#endif /* _LOG_ADAPTER_H_ */
