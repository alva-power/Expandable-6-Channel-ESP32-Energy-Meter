
# PlatformIO Setup and Flashing Guide

This document outlines how to install PlatformIO and use it to flash your ESP32 device, assuming you already have a `platformio.ini` file configured for your project.

## PlatformIO Installation

PlatformIO can be installed in a few ways, the most common methods are described below:

### 1. PlatformIO IDE (VS Code Extension)

The recommended method is using the PlatformIO IDE extension for Visual Studio Code (VS Code).

1.  **Install VS Code:** Download and install VS Code from [https://code.visualstudio.com/](https://code.visualstudio.com/).
2.  **Install the PlatformIO IDE Extension:**
    *   Open VS Code.
    *   Go to the Extensions view (Ctrl+Shift+X or click the Extensions icon in the Activity Bar).
    *   Search for "PlatformIO IDE" and click "Install".
    *   VS Code may need to be restarted after installation.

## Flashing Your ESP32

This section assumes you have:

*   Installed PlatformIO correctly (either via VS Code or CLI).
*   Created a PlatformIO project and your `platformio.ini` is configured.
*   Connected your ESP32 to your computer via USB.
*   Installed any required serial drivers.
*   Opened the correct project in VS code.

**Flashing from PlatformIO IDE (VS Code):**

1.  **Open Your Project:** Open your project directory in VS Code.
2.  **Build Your Code:**
    *   Click the PlatformIO logo in the left Activity Bar.
    *   Click the "Build" icon (check mark) at the Bottom of the VSCODE.
    * This compiles your source code into an executable file.
3.  **Upload to Your ESP32:**
    *   Click the "Upload" icon (arrow pointing to device) to flash the code to your ESP32.
    * This process builds the project if not already done, and then uploads it to your board.
4.  **Monitor the output:**
    *   Click the "Monitor" icon (plug icon) to see the serial output from the ESP32.

**Flashing from PlatformIO Core (CLI):**

1.  **Navigate to Your Project:** Open your terminal or command prompt and navigate to your PlatformIO project directory where the `platformio.ini` is located.
2.  **Build and Upload:**
    *   Run the command:
        ```bash
        pio run
        ```
        This command compiles the code and uploads it to your board.
    * Alternatively, you can specify specific actions such as:
        *   Build: `pio run -t build`
        *   Upload: `pio run -t upload`
    * If your board upload fail, change in your `platformio.ini`:
        ```ini
        upload_port = COM3    # Change this to your USB PORT, you can usually find it in device manager.
        ```
3.  **Monitor Serial Output:**
    *   Run the command:
         ```bash
        pio device monitor
        ```
        This opens the serial monitor and shows the data from your ESP32.

## Important Notes:

*   **Serial Port:** PlatformIO usually automatically detects the serial port, but if you face errors you should add the `upload_port` option in your `platformio.ini` file to specify which port to use. See above. The same can be done with `monitor_port`.
*   **Troubleshooting:**
    *   If you encounter issues, verify your USB connection, serial drivers, and settings in your `platformio.ini`.
    *   Refer to the PlatformIO documentation for ESP32 if there are other questions.