# PatriotFan

A Patriot plugin to support variable speed fan devices.

This library is used to support variable speed fans using PWM,
or simple on/off fans if connected to a non-PWM pin.

## Usage

Include this library in any Photon sketch that needs to support a fan.
Refer to the more complex examples in the main Patriot IoT examples
 directory.

This example creates a single fan device connected to pin D6.

A behavior is setup to control the fan with an event named "Temp".

Since Patriot supports automatic detection by the iOS app and Alexa,
either of these can be used to control the fan once this sketch is
loaded to a Photon. There may be a 15 minute delay before Alexa starts
to work, since a "Discover Devices" cycle is needed. You can manually
invoke a discovery using the Alexa app to eliminate the delay.

Note that the device is not controlled directly. It must be controlled
by defining a behavior. The behavior can be turned on or off using
Alexa or the iOS app, in addition to other input devices.

```
#include <IoT.h>
#include <PatriotFan.h>

IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("myPhoton");
    iot->begin();

    Fan *fan1 = new Fan(D6, "kitchenfan");
    iot->addDevice(fan1);

    iot->addBehavior(fan1, new Behavior("Fan", '>', 0, 100));
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
