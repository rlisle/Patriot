# PatriotLight

A Patriot plugin to support ultrasonic proximity detector devices.

This library is used to support ultrasonic proximity devices.
Any 2 Photon I/O pins may be used to provide pulse trigger and echo status.

## Usage

Include this library in any Photon sketch that needs to support an ultrasonic
proximity sensor such as the HC-SR04. 
These are inexpensive devices that generate an ultrasonic
pulse when triggered, and raise an echo line when that pulse is detected.
Distance is calculated by measuring the time between the trigger and the
received echo.
Refer to the more complex examples in the main Patriot IoT examples
 directory.

This example creates a single ultrasonic proximity device connected to pins
A1 and A0.

When the state of the input D4 pin changes, an event named "switch"
will be published. Other device can define a behavior that uses
this event.

You can use the Particle.io console to monitor these events.

```
#include <IoT.h>
#include <PatriotUltrasonic.h>

IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("myPhoton");
    iot->begin();

    // Create the HC-SR04 sensor device
    Ultrasonic ultra1 = new Ultrasonic(A1, A0, "Proximity");

    // Add it to IoT
    iot->addDevice(ultra1);
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
