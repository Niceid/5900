
#ifndef _FCC_TEST_
#define _FCC_TEST_

void set_test_fcc_info(u8 *buf);
void fcc_uart_init();
void test_fcc_default_info(void);
void __set_fcc_default_info(u8 trx_mode,u8 fre,u8 p_type,u8 d_type,u8 tx_power,u8 hop_mode);
void test_ble_rf_default_info(void);
u8 fcc_uart_isr_callback(u8 uto_buf);
void fcc_uart_write(char a);
#endif
