/* ========================================
 * 
 * Host Uart for HiveBio Ministat
 * 
 * Copyright C. Harrison
 * BSD 2-clause license http://opensource.org/licenses/BSD-2-Clause
 *
 */
#include "ros.h"

#include "Uarts.h"
extern "C" {
  #include "project.h"
}

void Uart::begin(unsigned long baud) {
  // compute clock divider D = HFCLK/(baud * oversampling_factor)
  uint32_t denom = baud * Host_UART_UART_OVS_FACTOR;
  uint16_t divider = (CYDEV_BCLK__HFCLK__HZ + (denom/2))/denom;
  Host_UART_Clock_SetFractionalDividerRegister(divider, 0);
  Host_UART_Start();
};

int Uart::read(void) {
  int32_t data = -1;
  if (Host_UART_SpiUartGetRxBufferSize() != 0) {
    // data is ready
    data = (int32_t)Host_UART_SpiUartReadRxData();
  }
  // clear error flags
  if (Host_UART_CHECK_INTR_RX(Host_UART_INTR_RX_ERR)) {
    // TODO: test for meaningful error (i.e. not Buffer empty) & return -1
    Host_UART_ClearRxInterruptSource(Host_UART_INTR_RX_ERR);
  }
  return data;
};

size_t Uart::write(uint8_t data) {
  Host_UART_SpiUartWriteTxData((uint32_t)data);
  return 1;
};

Uart Uart0;

