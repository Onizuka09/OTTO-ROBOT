init_project: 
	pio init --board esp32doit-devkit-v1
generate_commands_json:
	pio run -t compiledb
build:
	pio run
flash:
	pio run -e esp32_usb -t upload
test: 
	pio test -e esp32_usb
OTAFlash:
	pio run -e esp32_ota -t upload
