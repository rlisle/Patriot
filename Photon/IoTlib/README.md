# IoT

A framework for building powerful Photon based IoT projects.

## Welcome to ParticleIoT

This library makes it quick and easy to implement powerful particle.io Photon
based automation projects that combine inputs from sensors, Alexa, and iOS devices.

## Usage

Start creating your Photon based project using any of the Photon examples provided
in the examples folder.

Here is the switchesAndLEDs example. It includes 4 switches and 4 LEDs.

Note that the switches do not directly control the LEDs. They could, but
it's much more flexible to have them specify an _activity_, such as watching TV
or going to sleep.  This is shown in the table below.

Each activity can then control one or more different devices, 
each with individually specified settings, such as 0% to 100% brightness.
 

| Switch | Activity | LED 1 Outside | LED 2 Kitchen | LED 3 Bedroom | LED 4 Bathroom |
| ------ | -------- | ------------- | ------------- | ------------- | -------------- |
|   1    | WakeUp   | Low           |  Low          |  On           | On             |
|   2    | WatchTV  | On            |  Low          |  Low          | Off            |
|   3    | Cook     | On            |  On           |  Low          | Low            |
|   4    | Sleep    | Off           |  Off          |  Off          | Low            |

 
```
#include <IoT>
IoT *iot;

void setup() {
    iot = IoT::getInstance();
    iot->setControllerName("myPhoton");
    iot->setPublishName("myEvents");
    iot->begin();
    
    iot->addSwitch(kSwitch4Pin, 'WakeUp');
    iot->addSwitch(kSwitch3Pin, 'WatchTV');
    iot->addSwitch(kSwitch2Pin, 'Cook');
    iot->addSwitch(kSwitch1Pin, 'Sleep');
    
    iot->addLight(kLed1Pin, 'Outside');
    iot->addLight(kLed2Pin, 'Kitchen');
    iot->addLight(kLed3Pin, 'Bedroom');
    iot->addLight(kLed4Pin, 'Bathroom');

    iot->addBehavior('Outside', new Behavior('Wakeup','>',0, 100));
    iot->addBehavior('Kitchen', new Behavior('WatchTV','>',0, 100));
    iot->addBehavior('Bedroom', new Behavior('Cook','>',0, 100));
    iot->addBehavior('Bathroom', new Behavior('Sleep','>',0, 0));

    iot->exposeControllers();
    iot->exposeActivities();
}

void loop() {
    iot->loop(millis());
}
```

The polling of the switches, and fading of the LEDs is performed by the library.
Your sketch code just needs to call the IoT loop method, passing the current millis()
time.

See the [examples](examples) folder for more examples.

## Documentation

Wouldn't it be great if you could quickly prototype an idea using a particle.io
Photon, and have it just automatically work with other Photons, Alexa, and iOS apps?
That's the main idea behind this library. In addition to providing a lot of common
home automation functions, such as controlling LED lights and reading switches
and other sensors, it includes iOS apps and Alexa skills that can automatically
detect when you connect a new device to your network.

### Self Registration

I refer to this automatic detection of new devices as _self registration'.
Each Photon controller exposes information about itself that allows other
applications to detect and interact with it.

### Event based interactions

In addition, the use of _events_ provides a many-to-many network, where new
devices can be added without having to modifying any other devices.

### Activities and Behaviors

Instead of directly controlling devices using commands such as 
"turn the living room lamp on", each device is controlled by defining
_behaviors_ that respond to _activities_. So for example, in order to turn
that living room light on, you might define an activity called "Watch TV",
and a behavior for that lamp that would turn it on whenever "Watch TV" is
active.

By using activities and behaviors, new devices can be added and start working
without modification to other devices. For example, you might add a switch
that detects when the TV is turned on. It could then send the "Watch TV" event,
and the living room lamp would immediately respond to it.

Or you could add another lamp controller, and have it respond to the "Watch TV" 
activity also, and you wouldn't have to modify the existing controllers. 

This may sound similar to what HomeKit and other IoT architectures refer to
as _scenes_. And while they are indeed similar, a key difference between the
use of _activities_ and _scenes_ is that _activities_ start and stop, and it
is expected that multiple _activities_ can be active at the same time.
When one activity ends, all other current activities are evaluated in order
to set the new state of devices. This is different from most _scene_ approaches
that simply set the state of a group of devices when a _scene_ is set, regardless
of what other _scenes_ might have been set previously.

### Why Particle.io and Photon?

When evaluating the plethora of microcontrollers and communications protocols
available, I eventually decided on a few important requirements:

1. Controllers must be over-the-air programmable. 
   I'm mounting light controllers inside the walls or behind switch plates, 
   so I don't want to have tear them out of the wall everything I make a change
    to their behavior or fix a bug.

2. Wifi.
   Having the controllers directly connect to Wifi just simplifies everything.

3. Super easy to use.
    I'd like as much functionality out-of-the-box as possible, so I don't have
      to create and maintain it. I'd also like great support, and a large user
      base to help answer questions. And it needs to be reliable. I hate getting
      into the middle of a project and get diverted to chasing hardware bugs.
      
I was initially put off by the $19 price tag on these parts, but everything
considered it's a great deal. I'll probably use at most about 25 of these in
my home automation projects, meaning that the total cost of these will be
roughly $500. Think about how much can be controlled with 25 individual
controllers. Using my own custom PCBs (also posted on this repo), these
25 controllers can be built-out for $5 to $10 each, bringing the anticipated
cost of all 25 controllers to around $700.


### Creating a new Photon Controller

Start with one of the examples. You can use any of the available IDEs or CLI.
Use the notes in the examples to connect the correct pins to LEDs, switches, etc.
Then flash the device. 

Monitor events using the particle.io console logs page.

You can use the particle CLI to generate events, and/or monitor the state
of exposed variables.

I'll be adding a Hackster.io article about doing this in the near future.

### Companion Alexa Skill and iOS apps 

Once you have created one or more photon based devices, you can access them
using either or both an Alexa skill or iOS app.

The source for these is included in this repo.
Currently you'll need to build these yourself, including your Particle.io
account information where indicated in the sources.
 
Later on I plan on extending all these apps to use OAuth to allow users to
log into their particle.io account safely, without modifying the source.
Once this is done I'm going to try to submit these so that they can be used
without having to build them from source. It remains to be seen if Apple and
Amazon will allow an app for use by hobbyist created hardware, but I'm going 
to try.

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
