# PatriotLight

A Patriot plugin to support input switch devices.

This library is used to support input switch devices.
Any Photon I/O pin may be used to provide on/off status.

## Usage

Include this library in any Photon sketch that needs to support a Switch.
Refer to the more complex examples in the main Patriot IoT examples
 directory.

This example creates a single switch device connected to pin D4.
The internal pull-up resistor is used, so the switch needs only
ground the pin to change its state. For experimenting, you can
use a jumper wire to ground.

When the state of the input D4 pin changes, an event named "switch"
will be published. Other device can define a behavior that uses
this event.

You can use the Particle.io console to monitor these events.

```
#include <IoT>
#include <PatriotSwitch>

IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("myPhoton");
    iot->begin();

    Switch *switch1 = new Switch(D4, "Switch");
    iot->addDevice(switch1);
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
