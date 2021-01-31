/**
 * This file contains configuration information needed for runtime.
 * These values might be changed during testing.
 */
'use strict';

var config = {

    /**
     * Alexa Smart Home skill id
     */
    AppId           : 'amzn1.ask.skill.8aaef1d2-644f-470d-980e-fad4f1e471e4',

    /**
     * In the future this may be programmable from the device and
     * exposed as a variable, but for now just use a fixed name.
     */
    EventName       : 'patriot',

    PublishEndPoint : 'https://api.particle.io/v1/devices/events',

};
module.exports = config;
