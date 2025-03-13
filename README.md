# stm32f4-discovery_sdcard_spi
Stm32f4-Discovery - STM32F407
Add my SDCARD(SPI mode) + USB UART CH340G

u2tx     - PA2
u2rx     - PA3
spi2clk  - PB13
spi2miso - PB14
spi2mosi - PB15
spi2cs   - PB12

NO rtos, Uart console(115200 8n1)
spi(8bit) + FatFs + xprintf

-------------------------------------------------------------------------------
Board parameter:
 HSI = 16MHz (all hard modules -> 16 MHz)
 fask spi clk = 8 MHz

Test 1:
Create Bin file, size = 15Mb
write file size = 15Mb t_start = 383, t_stop = 133839 Speed write = 118260 b/s

Test 2:
Create Bin file, size = 15Mb
read file size = 15801700, t_start = 487, t_stop = 77619 Speed read = 205216 b/s
