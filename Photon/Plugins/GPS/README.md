# PatriotGPS

A Patriot plugin to provide MQTT location messages


## Usage

Include this library in one Photon sketch to provide location information
for use by all controllers on the same MQTT network.
Refer to the more complex examples in the main Patriot IoT examples
 directory.

This example creates a single GPS device to broadcast
messages to the MQTT network.

You can use the Particle.io console to monitor these states via the
published 'States' variable, or observe them in realtime on MQTT
(eg. using mosquitto_sub)

```
#include <IoT.h>
#include <PatriotGPS.h>

IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->begin();

    // Create GPS device
    iot->addDevice(new GPS());
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
Copyright 2017-2021 Ron Lisle

Refer to the included LICENSE file.
