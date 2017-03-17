# Particle IoT
Open source, Arduino hobby IoT project using Particle.io Photons,
Alexa, and iOS apps.

Refer to my blog at lisles.net for more information.

All this code is running in my RV and is a work-in-progress.

It may take me awhile to get it all posted in this Github repo.
I ask for your indulgence in allowing me time to get
all the pieces uploaded here.

This project is intended to help other hobbyist that want to create
their own Arduino IoT devices, but may not want to create their own
Alexa or iOS apps to control them. By using the published IoT library,
your photon code can automatically connect to the Alexa and iOS
applications.

## Devices
Particle.io Photons are used to due to their ease of use and great
support. I've created low cost printed circuit boards (PCBs) to make
it even easier to use. These have wireless built-in, and can be programmed
over-the-air (OTA) so all they need is power to get started.

I've provided a set of source code files and instructions
that can be used to simplify
programming IoT devices using Particle's $19 Photon.

I've also include the Eagle files and instructions for creating
general purpose IoT printed circuit boards. Using these files,
PCBs can be ordered from China for about $1.50 each.

## Alexa Smart Home Skill
An Alexa smart home skill can be used to voice control the devices.
This includes Amazon's Echo, Dot, or Tap, in addition to other
AVS implementations such as Astra running on iOS.

The Alexa smart home skill will automatically detect your devices
and allow control of them based on the names and events coded in
the Photon microcode.

Currently the skill needs to be created in your own account with your
own LWA credentials, but I intend to allow the use of a single,
published skill using particle.io OAuth in the future.

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
app such as this, but it's certainly worth a try.

## Release History
Refer to the (Releases page)[https://github.com/rlisle/ParticleIoT/releases]
for release history.