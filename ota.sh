source ~/zephyrproject/.venv/bin/activate
west flash --hex-file build/mcuboot/zephyr/zephyr.hex && west flash --hex-file build/EscapeRoom/zephyr/zephyr.signed.hex
