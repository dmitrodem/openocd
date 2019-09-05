interface ftdi
ftdi_vid_pid 0x0403 0x6010

ftdi_layout_init 0x0088 0x009b
jtag newtap cmodA7 tap -irlen 6 -expected-id 0x0362d093
target create leon3.cpu leon3 -chain-position cmodA7.tap -endian big
gdb_target_description enable
transport select jtag
adapter_khz 30000
