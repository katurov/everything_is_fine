# Is everything fine

The idea is to make a small screen witch will inform me about "Everything is good", using **MCU I already have**. So I took [TTGO](https://github.com/Xinyuan-LilyGO/TTGO-T-Display) and decided to make "tables" how it is going: lets it going alright. So now it looks like so:

## How it looks like

![this is how it looks](https://github.com/katurov/everything_is_fine/blob/main/IMG_4313.jpeg)

Here is a rectangular model of net:
* Is here a WiFi itself? Is is my laptop slow or do I have problem with WiFi?
* Ok, say, WiFi is ok, isn't my ISP good?
* Ok, ISP is ok, is whole Internet fine?
* It is everything fine with Internet, are my sites good?

Just a single look on these "rectangles" on the screen and I got an answer. If red - it is time to take a coffee, if yellow - it is time to take a closer look on it. Green meens it is boring.

All item are present on the screen, if something going wrong, the rectangle of this item changes its color.

Last item is the battery: MCU have connected battery, so I show here is it ok or not (don't know what for).

## The interface

Human' native: if it is not green, you should take care

Buttons:
* left one puts it online
* right one puts it offline (powered off)

## Small problem

As all controllers ESP32 cannot deal with SSL in "normal PC"way, so we have to put SSL certificates directly on MCU and use it on every connection. Sometimes certificates will gone outdated, so we will need to update 'em in MCU.

## Materials

1. TTGO ESP32

## Config

Open `config.h` and fillup variables. What you have to know:
* here is place for pwo pings and two sites
* you can use one and three -- why not
* `PINGHOSTS` - points how many host for ping
* `WEBSETIES` - how many hosts for web-check
* `WEBSETIES` + `PINGHOSTS` <= 4
* `hosts` contains IPs of hosts to ping
* `websites` contains Host's names and host's IPs to checks

Struct of `hosts`:
* `name` - what to show on "rectangle"
* `host_ip` - IP in format `IPAddress(89, 216, 107, 1)`
* `avg_delay` - this is the value in ms, if average time for ping answer more than it, "rectangle" will colored YELLOW
* `position` - where to draw the ractangle

Struct of `websites`
* `name` - what to show on "rectangle"
* `host` - hostname in DNS terms, like 'google.com'
* `get_string` - I'm lazy enough so here have to be a string 'GET https://`host`  HTTP/1.0'
* `hst_string` - I'm lazy enough so here have to be a string 'Host: `host`'
* `crt` - for SSL we have to verify host's certificate, so we have to have a public key (see below hot to get it)
* `position` - where to draw the ractangle

Do not forget to open User_Setup_Select.h of TFT_eSPI and:
* comment  ` #include <User_Setup.h>`
* uncomment `#include <User_Setups/Setup25_TTGO_T_Display.h>`

