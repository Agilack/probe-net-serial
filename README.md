# probe-net-serial
Ethernet to serial adapter. Allow remote control of a serial port (remote console, debug port, ...) 

# revision 0.1
  This revision was the first prototype. This branch is for some future tests
but can be considered as deprecated.

 * hw-netserial-r01: Hardware board

 * sw-bootloader: Allow flash/update using DHCP and TFTP (if SW0 is pushed on startup)

 * sw-ut_eth: Unit test for ethernet driver

 * sw-ut_eth_uip: Unit test of uIP with ethernet interface

 * sw-ut_hello-bl: Simple test of firmware over bootloader "Hello World"

 * sw-ut_telnet-bl: Telnet server over bootloader using network API

 * sw-ut_oled: Unit test for OLED 0.91' display