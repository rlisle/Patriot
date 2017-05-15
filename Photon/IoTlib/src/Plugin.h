//
// Created by Ron Lisle on 5/14/17.
//
#pragma once

class String;

class Plugin {
public:

    String  name;

    Plugin(String name);

    void    loop();
};
