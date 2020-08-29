# Patriot/Controllers #

This directory contains the sketches for the RV controllers.

### What is this directory for? ###

* Contains a project for each RV controller.
* v1.0.1 (plugins)

### How do I get set up? ###

* A directory exists for each controller
* To create a new controller: 
   * copy the Template directory to a new directory
   * Edit the source and project files
      * Include/update any plugin libraries
   * Use the 'f' alias to compile and flash the unit
      * This assumes that the directory matches the controller name
      * alias f='particle flash ${PWD##*/}'
      * requires that properties file is present

### Contribution guidelines ###

* Be sure to update Git when updating code

### Other Repos ###

* PatriotMac
* Patriot-iOS
