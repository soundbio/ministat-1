/* ========================================
 * 
 * Host USB CDC Uart for HiveBio Ministat on PSoC5
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
  // baud rate unused
  USBUART_Start(0, USBUART_DWR_VDDD_OPERATION);
  while(!USBUART_GetConfiguration());
  USBUART_CDC_Init();
  read_ptr = read_tail = read_buf;
};

int Uart::read(void) {
  int32_t data = -1;
  uint16_t count;
  if (read_ptr<read_tail) {
    data = *read_ptr++;
  } else if ((count = USBUART_GetCount()) != 0) {
    // data is ready
    if (count > sizeof(read_buf)) {
      count = sizeof(read_buf);
    }
    read_ptr = read_buf;
    read_tail = read_buf + USBUART_GetData(read_buf, count);
    if (read_ptr<read_tail) {
      data = *read_ptr++;
    }
    P1_6_Write(1);
  }
  return data;
};

size_t Uart::write(uint8_t data) {
  while(USBUART_CDCIsReady() == 0u);    /* Wait till component is ready to send more data */ 
  USBUART_PutChar(data);
  return 1;
};

Uart Uart0;

