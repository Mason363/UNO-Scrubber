import serial
import serial.tools.list_ports
import pyautogui
import time
import sys

# CONFIGURATION
baud_rate = 115200
pyautogui.PAUSE = 0

# Auto-Detect Arduino
arduino_port = None
ports = serial.tools.list_ports.comports()
for p in ports:
    if "usbmodem" in p.device:
        arduino_port = p.device
        break

if arduino_port is None:
    time.sleep(2)
    sys.exit()

try:
    ser = serial.Serial(arduino_port, baud_rate, timeout=1)
    time.sleep(2)
    while True:
        if ser.in_waiting > 0:
            if ser.in_waiting > 20:
                ser.reset_input_buffer()  # Anti-Lag logic
            try:
                line = ser.readline().decode('utf-8').strip()
                if line == 'E_LEFT':        pyautogui.press('left')
                elif line == 'E_RIGHT':     pyautogui.press('right')
                elif line == 'E_SHIFT_L':   pyautogui.hotkey('shift', 'left')
                elif line == 'E_SHIFT_R':   pyautogui.hotkey('shift', 'right')
                elif line == 'CMD_SPACE':   pyautogui.press('space')
                elif line == 'CMD_NEW':     pyautogui.hotkey('shift', 'n')
                elif line == 'CMD_S':       pyautogui.press('s')
                elif line == 'CMD_RIPDEL':  pyautogui.hotkey('option', 'backspace')
            except Exception:
                pass
except:
    sys.exit()
