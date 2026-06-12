## About

This project turns an ESP32-S3 development board into an infinite filesystem inspired by the [digital Library of Babel](https://libraryofbabel.info/).

## Usage

1. Buy an ESP32-S3 development board - ideally one shaped as a USB stick for maximum bewilderment. [This is the one I got](https://a.aliexpress.com/_EvdHrrY), though you don't necessarily need this exact one.
2. Get Visual Studio Code and set up PlatformIO. Refer to Google or YouTube if you don't know how.
3. Clone this repository **with submodules**. Again, if you don't know what that means, look it up.
4. Open the cloned folder in VScode, wait for it to set up the project.
5. While holding the "BOOT" button, plug the microcontroller into your PC.
6. Click the "→" icon in VScode to compile and flash the project. Once that's done, disconnect and reconnect the microcontroller.
7. Explore! You might have to copy files off of the MTP share before reading them, as most programs don't support reading directly from MTP.

## Finding specific files

1. Install [Bun](https://bun.sh/).
2. Compile `file-to-path.cpp` with:

   ```bash
   clang++ -std=c++17 -O3 file-to-path.cpp -o file-to-path
   ```

3. Navigate to the cloned folder and use the command `bun run file-to-path.js <path>`, where `<path>` is a path to the file you wish to find. Note that files larger than a couple hundred bytes will take longer to search.
4. Copy the path it returns and paste it into your file browser after `disk/`.
5. Find the `file`, copy it off of the drive, and verify that it is in fact the same file.

## Finding a file path from a file

1. Compile `path-to-file.cpp` with:

   ```bash
   clang++ -std=c++17 -O3 path-to-file.cpp -o path-to-file
   ```

2. Run the program and pass the output file name you want to create:

   ```bash
   echo "/AB/cd/..." | ./path-to-file output.bin
   ```

   Replace `"/AB/cd/..."` with the generated Babel path you want to convert back into a file. The program reads the path from standard input and writes the recovered bytes to `output.bin`.
3. Open the generated file with a hex editor or compare it against the original file you encoded.

## Credits
The hardware-facing bits of this project are loosely cobbled on top of RigoLigoRLC's work on [esp32s3-tusb-mtp](https://github.com/RigoLigoRLC/esp32s3-tusb-mtp) and their [fork of espressif-tinyusb-...