name: Build-Bin
on: [push]
jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - uses: arduino/setup-arduino-cli@v1
      - run: |
          arduino-cli config init
          arduino-cli config add board_manager.additional_urls https://github.com/ambiot/ambd_arduino/raw/master/Arduino_package/package_realtek_amebad_index.json
          arduino-cli core update-index
          arduino-cli core install ameba:amebad
          arduino-cli compile --fqbn ameba:amebad:ameba_bw16 . --output-dir ./out
      - uses: actions/upload-artifact@v4
        with:
          name: BW16-Bin-Files
          path: ./out/
