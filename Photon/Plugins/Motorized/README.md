# PatriotMotorized

A Patriot plugin to support motorized devices such as vents and curtains.

This library is used to support motorized devices that require timed open
and closed type operations.

## Usage

Include this library in any Photon sketch that needs to support a motorized device.
Refer to the more complex examples in the main Patriot IoT examples
 directory.

The example creates a single motorized device connected to pin D7.

A behavior is setup to control the device with an event named "vent".

Since Patriot supports automatic detection by the iOS app and Alexa,
either of these can be used to control the fan once this sketch is
loaded to a Photon. There may be a 15 minute delay before Alexa starts
to work, since a "Discover Devices" cycle is needed. You can manually
invoke a discovery using the Alexa app to eliminate the delay.

Note that the device is not controlled directly. It must be controlled
by defining a behavior. The behavior can be turned on or off using
Alexa or the iOS app, in addition to other input devices.

```
#include <IoT>
#include <PatriotMotorized>

IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("myPhoton");
    iot->begin();

    Motorized *vent1 = new Motorized(D7, "vent1");
    iot->addDevice(vent1);

    iot->addBehavior(vent1, new Behavior("Vent", '>', 0, 100));
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
Copyright 2017 Ron Lisle

Refer to the included LICENSE file.
