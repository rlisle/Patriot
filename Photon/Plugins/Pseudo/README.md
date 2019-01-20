# PatriotPseudoDevice

A Patriot plugin to support adding Alexa commands without devices.

This library is used to support commands without hardware.
This can be useful for setting a mode not associates with any particular device.
For example, GuestMode is used to disable turning on the lamp in the guest bedroom that would otherwise be turned on in the morning when I wake up.

## Usage

Include this library in any Photon sketch to add a pseudo device.

```
#include <IoT.h>
#include <PatriotPseudoDevice.h>

IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("myPhoton");
    iot->begin();

    // Create your device
    PseudoDevice *guestMode = new PseudoDevice();

    // Add it to IoT
    iot->addDevice(guestMode);
}

void loop() {
    iot->loop();
}
```

## Documentation

Refer to the Patriot Github repository and documentation for more
information.


## Contributing

Here's how you can make changes to this library and eventually contribute those changes back.

To get started, [clone the library from GitHub to your local machine](https://help.github.com/articles/cloning-a-repository/).

Change the name of the library in `library.properties` to something different. You can add your name at then end.

Modify the sources in <src> and <examples> with the new behavior.

To compile an example, use `particle compile examples/usage` command in [Particle CLI](https://docs.particle.io/guide/tools-and-features/cli#update-your-device-remotely) or use our [Desktop IDE](https://docs.particle.io/guide/tools-and-features/dev/#compiling-code).

After your changes are done you can upload them with `particle library upload` or `Upload` command in the IDE. This will create a private (only visible by you) library that you can use in other projects. Do `particle library add IoT_myname` to add the library to a project on your machine or add the IoT_myname library to a project on the Web IDE or Desktop IDE.

At this point, you can create a [GitHub pull request](https://help.github.com/articles/about-pull-requests/) with your changes to the original library.

If you wish to make your library public, use `particle library publish` or `Publish` command.

## LICENSE
Copyright 2019 Ron Lisle

Refer to the included LICENSE file.
