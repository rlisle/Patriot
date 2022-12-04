# Patriot
Patriot is an open source, hobby IoT project using 
Particle.io Photon devices, and optionally Alexa, Siri, MQTT, and/or iOS apps.

<img src="https://www.lucidchart.com/publicSegments/view/e2fb9ca3-c413-498b-b193-ab9dd6626c3a/image.png" alt="Patriot IoT Diagram" style="width: 640px;"/>

It is intended to help other hobbyists quickly create Internet-of-Things
projects that can be controlled using Voice (Alexa) and iOS devices
without having to recreate new Alexa skills or iOS apps.

This project consists of a published particle.io Photon
libraries (IoTlib and plugins) and a published Alexa Smart Home skill (Patriot).

An iOS control panel app exists that allows your old iPhones
to be used as touch display control panels. This code resides in the
Patriot-iOS repository.

If you are using an Alexa device, then you can also use the Alexa
app Smart Home page to monitor and control your devices in addition
to or instead of using the Patriot-iOS app.

Your Photon based IoT projects can be created using any of the 
standard particle.io development tools (Web IDE,
Particle IDE, or Particle CLI).

The main objective is to allow the creation of networks of IoT
devices that can communicate with each other, and is:
1. Easily extensible: new devices can be added without having to modify existing devices.
2. Super simple to program
3. Reuses existing code, including the Alexa skills, iOS apps, and Photon library code.

Refer to [Alexa Controlled Photon Project without Alexa Coding](https://www.hackster.io/patriot-iot/alexa-controlled-photon-project-without-alexa-coding-f47d84)
on Hackster.io or my blog at lisles.net for more information.

## Devices
Particle.io Photons are used to due to their ease of use and great
support. I had originally created my own low cost printed circuit boards (PCBs), but since then ControlEverything (NCD.io) has released a series of Photon based boards that make it even easier to use.

These have wireless built-in, and can be programmed
over-the-air (OTA) so all they need is power to get started.

I've provided a set of source code files and instructions
that can be used to simplify
programming IoT devices using Particle's $19 Photon.

I've still publish the Eagle files and instructions for creating
general purpose IoT printed circuit boards.
PCBs can be ordered from China for about $1.50 each.
However, I've found that purchasing boards from NCD.io is quicker and the boards are much higher quality. The prices are very reasonable. For example, a simple Photon board with a power supply that works directly with my RV's 12v power and has screw terminals for connecting to the Photon's pins is only $15.

## Alexa Smart Home Skill
An Alexa smart home skill can be used to voice control the devices.
This includes Amazon's Echo, Dot, or Tap, in addition to other
AVS implementations such as using the Alexa app on iOS.

The Alexa smart home skill will automatically discover your Patriot devices
and allow control of them based on the names and events coded in
the Photon microcode.

The Alexa skill will prompt you to login to your Particle.io account 
when you install it. From that point on it will automatically 
detect devices that you compile with the IoTlib, using the
name and commands specified in the device's code.

## iOS Apps
The Alexa app can be used to control and automate your Patriot devices. This is a robust solution for iOS when you are away from your Alexa devices.

Alternatively, I released the source code for an iOS control panel app on [Github](https://github.com/rlisle/Patriot-iOS2).
This app can be used to control your Patriot devices from your iPhone or iPad.
For example, you can use your old iOS devices as control panels
to turn on and off devices that you implement using Photons.
Support for this app will be minimal though. I find myself just using Alexa for user interaction with devices.

## Release History
Refer to the (Releases page)[https://github.com/rlisle/Patriot/releases]
for release history.
