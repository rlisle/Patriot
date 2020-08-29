# Front Panel

Controller mounted behind the front control panel.
It controls all switches on the front panel.

It is accessible by removing the entire wood panel.

## Hardware
 * Hardware
 * 1. Photon
 * 2. ControlEverything.com NCD8Relay Photon Controller
 * 3. ControlEverything.com NCD8Dimming I2C Controller
 * 4. I2C cable between boards
 * Photon board relay n/o connections 0x20
   - Sink                 (1)
   - Front Awning LEDs    (2)
   - Right Trim           (3)
   - Left Trim            (4)
   - Ceiling              (5)
   - Door Side Floods     (6)
   - Other Side Floods    (7)
   - Porch                (8)
 * I2C board relay n/o connections 0x21
   - High Kitchen         (1)
   - Kitchen Ceiling      (2)
   - Vent Fan             (3)
   - Vent Open            (4)
   - Vent Close           (5)
   - Front Awning Extend  (6)
   - Front Awning Retract (7)
   - unused               (8)

## Published Libraries
This project uses the published Patriot libraries:
* PatriotIoT (IoT)
* PatriotLight
* PatriotMotorized

When any library is updated, rerun "particle library add <name>" from the project directory.

## Code Organization

#### ```/src``` folder:  
Contains all the source files. 
It should *not* be renamed. 
Anything that is in this folder when you compile your project will be sent to our compile service
and compiled into a firmware binary for the Particle device that you have targeted.

If your application contains multiple files, they should all be included in the `src` folder.
If your firmware depends on Particle libraries, those dependencies are specified in
the `project.properties` file referenced below.

#### ```.ino``` file:
This file is the firmware that will run as the primary application on your Particle device.
It contains a `setup()` and `loop()` function, and can be written in Wiring or C/C++.
For more information about using the Particle firmware API to create firmware for your
Particle device, refer to the [Firmware Reference](https://docs.particle.io/reference/firmware/)
section of the Particle documentation.

#### ```project.properties``` file:  
This is the file that specifies the name and version number of the libraries that your project depends on.
Dependencies are added automatically to your `project.properties` file when you add a library to a
project using the `particle library add` command in the CLI or add a library in the Desktop IDE.

## Adding additional files to the project

#### Projects with multiple sources
If you would like add additional files to your application, they should be added to the `/src` folder.
All files in the `/src` folder will be sent to the Particle Cloud to produce a compiled binary.

#### Projects with external unpublished libraries
If your project includes a library that has not been registered in the Particle libraries system,
you should create a new folder named `/lib/<libraryname>/src` under `/<project dir>` and add
the `.h` and `.cpp` files for your library there. All contents of the `/lib` folder and subfolders
will also be sent to the Cloud for compilation.

## Compiling your project

When you're ready to compile your project, make sure you have the correct Particle device target
selected and run `particle compile <platform>` in the CLI or click the Compile button in the
Desktop IDE. The following files in your project folder will be sent to the compile service:

- Everything in the `/src` folder, including your `.ino` application file
- The `project.properties` file for your project
- Any libraries stored under `lib/<libraryname>/src`