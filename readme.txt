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

Other odds & ends

Mosfets used to control the motors NCE603583   
    Source PDF: http://www.ncepower.com/upload/cn/propdf/NCE603583.pdf
    Backup https://github.com/shades66/Maidesite-standing-desk/blob/main/NCE603583.pdf
    Image : https://github.com/shades66/Maidesite-standing-desk/blob/main/maidesite-handset-mosfets.jpg

Processor used HC32L130  32bit ARM® Cortex®-M0+ .   
     image : https://github.com/shades66/Maidesite-standing-desk/blob/main/maidesite-handset-cpu.jpg

Current sensor? 
     image: https://github.com/shades66/Maidesite-standing-desk/blob/main/maidesite-handset-currentsensororamp.jpg

    
