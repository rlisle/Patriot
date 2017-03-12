# IoT

A framework for building powerful Photon based IoT projects.

## Welcome to ParticleIoT

This library makes it quick and easy to implement powerful particle.io Photon
based automation projects that combine inputs from sensors, Alexa, and iOS devices.

## Usage

Start creating your Photon based project using any of the examples provided.
Connect to your particle.io account by adding your user information 
where indicated in the example. Then expand your project by adding the
Alexa skill code and/or iOS app code provided.

```
IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("lislerv");
    iot->setPublishName("RonTest");
    iot->begin();
    
    iot->addSwitch(kSwitch4Pin, kEvent4);
    iot->addSwitch(kSwitch3Pin, kEvent3);
    iot->addSwitch(kSwitch2Pin, kEvent2);
    iot->addSwitch(kSwitch1Pin, kEvent1);
    
    iot->addLight(kLed1Pin, kLight1);
    iot->addLight(kLed2Pin, kLight2);
    iot->addLight(kLed3Pin, kLight3);
    iot->addLight(kLed4Pin, kLight4);

    iot->exposeControllers();
    iot->exposeActivities();
}

void loop() {
    iot->loop(millis());
}
```

See the [examples](examples) folder for more details.

## Documentation

TODO: Describe `IoT`

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
