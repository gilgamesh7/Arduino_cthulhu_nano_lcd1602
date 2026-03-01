# Arduino_cthulhu_nano_lcd1602
Arduino project that displays  "ph'nglui mglw'nafh cthulhu r'lyeh wgah'nagl fhtagn !   "

# Setup
## Arduino IDE:
1. Tools → Board → Arduino AVR Boards → Arduino Nano
1. Tools → Processor → ATmega328P (Old Bootloader)
1. Tools → Port → select the USB port (on Mac it looks like /dev/cu.usbserial-*)

## Wiring

1️⃣ Prepare the Mini SD Card
- ⚠️ Critical Use 32GB or smaller.
- Format (Mac – your M1 Pro)
- Insert SD card
- Open Disk Utility
- Select the card
- Click Erase
- Format as:
  - MS-DOS (FAT)
  - Scheme: Master Boot Record (MBR)
- Erase

2️⃣ Copy the MP3 File
- Create a folder named exactly: <br>
`mp3`
- Rename your file exactly: <br>
`0001.mp3`
- Final structure: <br>
SD Card <br>
&nbsp;└── mp3 <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;└── 0001.mp3 <br>
No uppercase. No fancy names. The DFPlayer is traditional and strict.
- Eject safely.

3️⃣ Insert into DFPlayer
<br> Insert the SD card into the DFPlayer Mini slot.
<br> Contacts facing the PCB.

4️⃣ Wiring Overview
<br> 
| LCD | Nano |
| --- | ---- |
| VCC | 5V   |
| GND | GND  |
| SDA | A4   |
| SCL | A5   |


5️⃣ Wire DFPlayer to Arduino Nano
<br> 
| DFPlayer | Nano                                   |
| -------- | -------------------------------------- |
| VCC      | 5V                                     |
| GND      | GND                                    |
| TX       | D10                                    |
| RX       | D11 (through 1kΩ resistor recommended) |

6️⃣ Connect Speaker
| DFPlayer | Speaker |
| -------- | ------- |
| SPK_1    | +       |
| SPK_2    | –       |

<br> ⚠️ Do NOT connect speaker to Nano.

7️⃣ Connect the 4-Pin Toggle Switch
<img width="830" height="483" alt="image" src="https://github.com/user-attachments/assets/4942c8a0-7b00-4167-83d2-02e69b95aaef" />

8️⃣ Power Stability (Important)
- 3W speaker can draw current spikes.
- If Nano resets:
- Add:
  - 470µF capacitor across DFPlayer VCC & GND
 









