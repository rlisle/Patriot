# PatriotDHT

A Patriot plugin to support DHT11/22 Temp & Humidity sensor.

This library is used to support DHT11 and DHT22 temperature and
humidity sensors. Any 2 Photon I/O pins may be used.

## Usage

Include this library in any Photon sketch that needs to support a
DHT11 or DHT22 temperature sensor.

This example creates a single DHT sensor device connected to pin D4
and D5.

When the reported temperature or humidity changes, an event named
"indoor" will be published. Other device can define a behavior that uses
this event.

You can use the Particle.io console to monitor these events.

```
#include <IoT.h>
#include <PatriotDHT.h>

IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("myPhoton");
    iot->begin();

    DHT *dht1 = new DHT(D4, D5, "indoor");
    iot->addDevice(dht1);
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
