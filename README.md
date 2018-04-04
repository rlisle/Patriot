# Patriot
Patriot is an open source, Arduino hobby IoT project using 
Particle.io devices, Alexa, and iOS apps.

<img src="https://www.lucidchart.com/publicSegments/view/e2fb9ca3-c413-498b-b193-ab9dd6626c3a/image.png" alt="Patriot IoT Diagram" style="width: 640px;"/>

It is intended to help other hobbyists quickly create Internet-of-Things
projects that can be controlled using Voice (Alexa) and iOS devices
without having to recreate new Alexa skills or iOS apps.

Initially this project will consist of a published particle.io Photon
library (IoTlib) and a published Alexa Smart Home skill (Patriot).

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
support. I've created low cost printed circuit boards (PCBs) to make
it even easier to use. These have wireless built-in, and can be programmed
over-the-air (OTA) so all they need is power to get started.

I've provided a set of source code files and instructions
that can be used to simplify
programming IoT devices using Particle's $19 Photon.

I've also included the Eagle files and instructions for creating
general purpose IoT printed circuit boards. Using these files,
PCBs can be ordered from China for about $1.50 each.

Recently Particle announced that their P0 and P1 parts are now supported
in Seeed's PCB Fusion program.
This enables the creation of low cost custom PCBs using the smaller, lower 
cost P0 and P1 chips, and optional assembly. Wow!
I've not tried this yet, but it looks very promising and I will post
my designs once I do so.


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

Currently the apps must be compiled in Xcode with your Particle.io credentials
and downloaded to your iPhone or iPad manually, but I intend to allow the use
of a single, published app using particle.io OAuth in the future.
It remains to be seen whether Apple will allow certifying a hobbyist
app such as this, but I'm going to try.

And as mentioned above also, the Alexa app can be used to control your devices.

## Release History
Refer to the (Releases page)[https://github.com/rlisle/Patriot/releases]
for release history.
