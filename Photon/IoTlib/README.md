# IoT

A framework for building powerful Photon based IoT projects.

## Welcome to ParticleIoT

This library makes it quick and easy to implement powerful particle.io Photon
based automation projects that combine inputs from sensors, Alexa, and iOS devices.

## Usage

Start creating your Photon based project using any of the Photon examples provided.

Here is the switchesAndLEDs example. It includes 4 switches and 4 LEDs.

Note that the switches do not directly control the LEDs.
Instead, each switch creates an event. Each event can control 1 or more
different devices, each with different level settings.
 
In this example, four switches are used to trigger four events as shown in this table:

| switch | event   | LED 1 outside | LED 2 kitchen | LED 3 bedroom | LED 4 bathroom |
| ------ | ------- | ------------- | ------------- | ------------- | -------------- |
|   1    | WakeUp  | Low           |  Low          |  On           | On             |
|   2    | Sleep   | Off           |  Off          |  Off          | Low            |
|   3    | Watchtv | On            |  Low          |  Low          | Off            |
|   4    | Cook    | On            |  On           |  Low          | Low            |

 
```
IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("myPhoton");
    iot->setPublishName("myEvents");
    iot->begin();
    
    iot->addSwitch(kSwitch4Pin, kEvent4);
    iot->addSwitch(kSwitch3Pin, kEvent3);
    iot->addSwitch(kSwitch2Pin, kEvent2);
    iot->addSwitch(kSwitch1Pin, kEvent1);
    
    iot->addLight(kLed1Pin, kLight1);
    iot->addLight(kLed2Pin, kLight2);
    iot->addLight(kLed3Pin, kLight3);
    iot->addLight(kLed4Pin, kLight4);

    iot->addBehavior(kLight1, new Behavior(kEvent1,'>',0, 100));
    iot->addBehavior(kLight2, new Behavior(kEvent2,'>',0, 100));
    iot->addBehavior(kLight3, new Behavior(kEvent3,'>',0, 100));
    iot->addBehavior(kLight4, new Behavior(kEvent4,'>',0, 100));

    iot->exposeControllers();
    iot->exposeActivities();
}

void loop() {
    iot->loop(millis());
}
```

The polling of the switches, and fading of the LEDs is performed by the library.
You sketch code just needs to call the IoT loop method, passing the current millis()
time.

See the [examples](examples) folder for more examples.

## Documentation

TODO: Describe `IoT`

Once you have created one or more photon based devices, you can access them
using either or both an Alexa skill or iOS app.

Connect these to your particle.io account by adding your Particle.io
account information where indicated in the sources.
 
(TODO: allow connecting without modifying source and rebuilding) 


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

Licensed under the <insert your choice of license here> license
