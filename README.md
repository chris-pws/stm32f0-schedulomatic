# stm32f0-schedulomatic

The stm32f0-schedulomatic is a pre-emptive task scheduler supporting mutual exclusion signaling and a callback framework designed to accommodate parallel DMA memory <-> peripheral transfers and resource sharing by multiple threads. Currently supports fixed frequency tasks, including UART and SPI transmission.

To-do: 
* Continuous ADC to memory DMA;
* SSD1322 display driver;
* Frame buffer class;
* Stack framed context switching.
