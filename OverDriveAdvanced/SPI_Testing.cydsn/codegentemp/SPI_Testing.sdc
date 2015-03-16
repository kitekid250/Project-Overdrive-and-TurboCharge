# THIS FILE IS AUTOMATICALLY GENERATED
# Project: C:\Users\prd005\Documents\GitHub\integrated-robotics-lab-core\MotorDriver\OverDriveAdvanced\SPI_Testing.cydsn\SPI_Testing.cyprj
# Date: Fri, 31 Oct 2014 16:03:35 GMT
#set_units -time ns
create_clock -name {UART_SCBCLK(FFB)} -period 708.33333333333326 -waveform {0 354.166666666667} [list [get_pins {ClockBlock/ff_div_2}]]
create_clock -name {SPI_SCBCLK(FFB)} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/ff_div_3}]]
create_clock -name {CyHFCLK} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/hfclk}]]
create_generated_clock -name {UART_SCBCLK} -source [get_pins {ClockBlock/hfclk}] -edges {1 17 35} -nominal_period 708.33333333333326 [list]
create_generated_clock -name {SPI_SCBCLK} -source [get_pins {ClockBlock/hfclk}] -edges {1 2 3} [list]
create_clock -name {CyIMO} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CySYSCLK} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/sysclk}]]
create_clock -name {CyILO} -period 31250 -waveform {0 15625} [list [get_pins {ClockBlock/ilo}]]
create_clock -name {CyLFCLK} -period 31250 -waveform {0 15625} [list [get_pins {ClockBlock/lfclk}]]


# Component constraints for C:\Users\prd005\Documents\GitHub\integrated-robotics-lab-core\MotorDriver\OverDriveAdvanced\SPI_Testing.cydsn\TopDesign\TopDesign.cysch
# Project: C:\Users\prd005\Documents\GitHub\integrated-robotics-lab-core\MotorDriver\OverDriveAdvanced\SPI_Testing.cydsn\SPI_Testing.cyprj
# Date: Fri, 31 Oct 2014 16:03:28 GMT
