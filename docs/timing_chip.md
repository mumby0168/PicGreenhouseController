# Timing Chip (DS1302)

##Description
This chip provides a capability to read and write a time and then have it tracked by the chip internally when operating a system, in this case the PIC.

##Features
* Serial interface (read one bit at a time I/O Pin)
* 3 pins (RST->Reset, I/O->Read/Write, SCLK->Serial Clock)

##Writing & Reading Options
The data within the chip is stored in register which each contain different peices of data see the table below for the register addresses and storage mappings:

When writing or reading this can be done on a single register or done using a burst mechnism which is supported by the timing chip.
