# Patriot
Patriot is an open source, Arduino hobby IoT project using 
Particle.io devices, Alexa, and iOS apps.

It is intended to help other hobbyists quickly create Internet-of-Things
projects that can be controlled using Voice (Alexa) and iOS devices
without having to recreate new Alexa skills or iOS apps.

Initially this project will consist of a published particle.io Photon
library (IoTlib) and a published Alexa Smart Home skill (Patriot).

An iOS control panel app exists that allows your old iPhones
to be used as touch display control panels. This code is being
cleaned up and will be open sourced and/or available on the app
store soon.

Your Photon based IoT projects can be created using any of the 
standard particle.io development tools (Web IDE,
Particle IDE, or Particle CLI).

The main objective is to allow the creation of networks of IoT
devices that can communicate with each other, and is:
1. Easily extensible: new devices can be added without having to modify existing devices.
2. Super simple to program
3. Reuses existing code, including the Alexa skills, iOS apps, and Photon library code.

Refer to my blog at lisles.net for more information.

## Devices
Particle.io Photons are used to due to their ease of use and great
support. I've created low cost printed circuit boards (PCBs) to make
it even easier to use. These have wireless built-in, and can be programmed
over-the-air (OTA) so all they need is power to get started.

I've provided a set of source code files and instructions
that can be used to simplify
programming IoT devices using Particle's $19 Photon.

I've also included the Eagle files and instructions for creating
general purpose IoT printed circuit boards. Using these files,
PCBs can be ordered from China for about $1.50 each.

## Alexa Smart Home Skill
An Alexa smart home skill can be used to voice control the devices.
This includes Amazon's Echo, Dot, or Tap, in addition to other
AVS implementations such as Astra running on iOS.

The Alexa smart home skill will automatically detect your devices
and allow control of them based on the names and events coded in
the Photon microcode.

The Alexa skill will prompt you to login to your Particle.io account 
when you install it. From that point on it will automatically 
detect devices that you compile with the IoTlib, using the
name and commands specified in the device's code.

## iOS Apps
An iOS app can be used to control devices from your iPhone or iPad.
For example, you can use your old iOS devices as control panels
to turn on and off devices that you implement using Photons.

In addition, I've created a separate iOS app that can run on older
iOS devices. I use this to provide control panels by mounting unused,
older iPhones directly to the wall in various places around my RV.

Currently the apps must be compiled in Xcode with your LWA credentials
and downloaded to your iPhone manually, but I intend to allow the use
of a single, published app using particle.io OAuth in the future.
It remains to be seen whether Apple will allow certifying a hobbyist
app such as this, but I'm going to try.

## Release History
Refer to the (Releases page)[https://github.com/rlisle/ParticleIoT/releases]
for release history.
