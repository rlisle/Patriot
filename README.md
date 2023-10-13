# Patriot
Patriot is an open source, hobby IoT project using 
Particle.io Photon 2 devices, and optionally Siri, HomeKit, MQTT, and/or iOS apps.

<img src="https://www.lucidchart.com/publicSegments/view/e2fb9ca3-c413-498b-b193-ab9dd6626c3a/image.png" alt="Patriot IoT Diagram" style="width: 640px;"/>

It is intended to help other hobbyists quickly create Internet-of-Things
projects that can be controlled using Voice (Siri) and iOS devices
without having to recreate new HomeKit programming or iOS apps.

This project consists of published particle.io Photon 2
libraries (IoTlib and plugins).

Note that previously a published Alexa Smart Home skill (Patriot) was used.
The use of Alexa has been replaced with Siri and HomeKit. 
I apologize to those that want Alexa support, but it was getting to be too much work for me to support both iOS and Alexa by myself in my spare time.

The HomeKit "Home" app can be used with your old iPhones
to provide a touch display control panel.

Your Photon 2 based IoT projects can be created using any of the 
standard particle.io development tools (Web IDE,
Particle IDE, or Particle CLI).

The main objective is to allow the creation of networks of IoT
devices that can communicate with each other, and is:
1. Easily extensible: new devices can be added without having to modify existing devices.
2. Super simple to program
3. Reuses existing code, including the HomeKit apps, iOS apps, and Photon library code.

Refer to the older article [Alexa Controlled Photon Project without Alexa Coding](https://www.hackster.io/patriot-iot/alexa-controlled-photon-project-without-alexa-coding-f47d84)
on Hackster.io or my blog at lisles.net for some historical information.

## Devices
Particle.io Photon 2's are used due to their ease of use and great
support. I had originally created my own low cost printed circuit boards (PCBs), but since then ControlEverything (NCD.io) has released a series of Photon based boards that make it even easier to use.

These have wireless built-in, and can be programmed
over-the-air (OTA) so all they need is power to get started.

I've provided a set of source code files and instructions
that can be used to simplify
programming IoT devices using Particle's $19 Photon 2.

I've still publish the Eagle files and instructions for creating
general purpose IoT printed circuit boards.
PCBs can be ordered from China for about $1.50 each.
However, I've found that purchasing boards from NCD.io is quicker and the boards are much higher quality. The prices are very reasonable. For example, a simple Photon board with a power supply that works directly with my RV's 12v power and has screw terminals for connecting to the Photon's pins is only $15.

## Homebridge
HomeKit support is provided by Homebridge and the MQTTThingy plugin.

This allows Siri to control Patriot devices using your iPhone, Homepod, or Apple TV.

## Release History
Refer to the (Releases page)[https://github.com/rlisle/Patriot/releases]
for release history.
