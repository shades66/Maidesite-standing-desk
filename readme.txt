# Maidesite-standing-desk
My experimentations with connecting a Maidesite PRO 2 standing desk controller to HomeAssist

The following links have a wealth of information around the serial protocol
https://docs.google.com/spreadsheets/d/1GKZfDFljVX4eQBMawq0-Rc8t0x8V6gjQ5BgAYngPYTo/edit?pli=1#gid=1438530487 found via https://github.com/phord/maraid-Jarvis
https://github.com/phord/Jarvis

Implemented features

* Button to trigger any of the 4 presets
* Button to move UP/DOWN
* Slider to move direct to a height
* Get current settings for height & presets
* Read preset & current height at startup
* Lowers automatically once the rooms presence detector thinks no-one is around.  There is a delay to allow movement if it does see no movement.  In my case the desk light will switch off once no movement is detected giving time to avoid the desk dropping at the sametime.
* Disables HA/ESPHome driven requests when no movement detected.  Just in case someone wants to play when no-one is around...

Todo
* Tidy up code (remove test stuff)
* Add buttons to create presets based on current height
* Look into automations to automatically lower after alarms
* Add visual indicator of desk Enabled/Disabled

My current circuit is made up of the following

RJ12 Cable  https://www.amazon.co.uk/dp/B07YC7DWN5?ref=ppx_yo2ov_dt_b_product_details&th=1   cut to supply around 10 inches of cable
USB cable  cut to take power from the USB socket on the controller (The 5v from the RJ12 doesn't seem to have enough to power the ESP32)
ESP32-S3 Board  (should be able to use any,  I use my own board in most of my HA stuff https://github.com/shades66/OddsAndEnds/tree/main/ESP32Boards/V2 )

# Connections are

RJ12 ----------------------------- ESP32
YELLOW --------------------------  GROUND
                      /--[ 3k ]-^
GREEN  ------[ 1.5k ] -----------  Serial IN
BLUE   -------------------------- Serial OUT

if that makes no sense.  GREEN is TX from the RJ12 connector to a potential divider via a 1.5k resistor to the ESP32 which is connected again to ground via a 3k resistor.
(I'll put an image up at some point to make it clearer)

USB ----------------------------- ESP32
RED/5v ---------------------------- 5v in
BLACK/GND ------------------------- GND


# RJ12 pinout
   with the tab at the bottom looking into the connector the pins are

   1   Unused on BT.   connects to PA13/RXD/SWDIO on processor.   not tried anything with this
   2   GND
   3   TDX - Out of handset    PA04 on processor
   4   5v  - low current,  not enough to drive a ESP32 
   5   RXD - Into the handset  PA03 on processor
   6   Unused on BT.   connects to PA14/TXD/SWCLK on processor.   not tried anything with this

see https://github.com/shades66/Maidesite-standing-desk/blob/main/maidesite-back-handset.jpg for this & motor pinouts

J3  - Connector to front keypad (not really looked at this part of the board)
  Assuming Pin 1 next to J3 silkscreen
  Pin 1  -  GND from L3/U3 Buck Converter area
  Pin 2  -  VCC 5v?  from L3/U3 Buck Converter area
  Pin 3  - 
  Pin 4  -
  Pin 5  - 
  Pin 6  -
  Pin 7  -

Other odds & ends

Motors :   JXET-18-50
          input:  DC24V 5A 120W
        https://www.reddit.com/media?url=https%3A%2F%2Fpreview.redd.it%2Frwye4gc3shk81.png%3Fwidth%3D4032%26format%3Dpng%26auto%3Dwebp%26s%3D7e5ffd4c4db3439ee0d53be7d9f2ef88d9a3b4a0

DdX components all over the board.  Appear to be BAV99 A7 SOT23 Hottech ESD Protection based on A7 marking and how they are connected
    See : https://www.sunrom.com/p/bav99-a7-sot23-hottech-esd-protection-of-io-pins
    
Mosfets used to control the motors NCE603583   
    Source PDF: http://www.ncepower.com/upload/cn/propdf/NCE603583.pdf
    Backup https://github.com/shades66/Maidesite-standing-desk/blob/main/NCE603583.pdf
    Image : https://github.com/shades66/Maidesite-standing-desk/blob/main/maidesite-handset-mosfets.jpg

Processor used HC32L130  32bit ARM® Cortex®-M0+ .    HC32L136J8TA-LQ48
     image : https://github.com/shades66/Maidesite-standing-desk/blob/main/maidesite-handset-cpu.jpg
     link: https://www.keil.arm.com/devices/hdsc-hc32l130j8ta/features/
        not found an english PDF but pinout & basics can be worked out on document linked here https://jlcpcb.com/partdetail/Xhsc-HC32L136J8TALQ48/C779473

      PA13 - Pin 1 Bluetooth RJ12 Connector
      PA14 - Pin 6 Bluetooth RJ12 connector
      PA04 - Serial TO Bluetooth RJ12 Connector
      PA03 - Serial FROM Bluetooth RJ12 Connector

      PA10 - Q9 (LeftLeg) - Ground ENABLE Gate  Motor Pin 1
      PA08 - Q10 (LeftLeg)  -Ground ENABLE Gate  Motor Pin 2
      PB14 - Q7 (RightLeg)  - Ground ENABLE Gate  Motor Pin 1
      PB13 - Q8 (RightLeg)  - Ground ENABLE Gate  Motor Pin 2

      PB10  - From LM358
      PB02  - FROM LM358

      PB04  - Child Lock Button

      PA15  -  +V GATE ENABLE
      PB03  -  +V GATE ENABLE
            - not worked out this part fully.    Seems we have 2 signals into Q6. Guessing a dual transistor/fet ? which allow the following 2 selections
                Q8 (RightLeg)  +  Q10 (LeftLeg)   +V Enable Gate  Motor Pin 2
                Q7 (RightLeg)  +  Q9 (LeftLeg)    +V Enable Gate  Motor Pin 1

      Unknown - Track going under processor near U4 silkscreen
              - This looks to go to R4/R8 between the mosfets.  Maybe a temperature sensor to detect overheating ?

      PB01  - Left Leg  Motor Signal 2
      PB00  - Right Leg  Motor Signal 1
      PA07  - Left Leg  Motor Signal 1
      PA06  - Right Leg  Motor Signal 2
            -   These pins connect to the sensors in the motors. Possible optical encoders as the sockets provide a GND & 5V connection too?


      + various others.  Mostly to the keypad so as others have found a mix of GPIO's for the UP/DOWN + other buttons as well as the serial connection to (and from?) the display.
        Also spitted some go to the power supply section.    Monitoring or control !??


Current sensor?  A LM358A by 3PEAK by the looks.    I guess measuring for load over the 2 large 150 resistors for the sensor if something is blocking the desk + maybe even for motor issues?
     image: https://github.com/shades66/Maidesite-standing-desk/blob/main/maidesite-handset-currentsensororamp.jpg

Power supplies.
      24V IN (I think) + Some filterting to the Mosfets for driving the motors

      Small Buck converter  (U1 & L1)  -  Appears to be for the 5v across the board
      Large Buck converter (U3 + L3)   -  To the button panel.  Power for USB Connector + components on the button board?
      Temperature sensor connects via Q1,  Maybe a voltage reference for accurate temperatore measurement?

