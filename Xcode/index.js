/**
 * RV Smart Home skill
 *
 * Updated for V3 API changes 9/28/17
 *
 * Note that per Amazon V2 must continue to be supported until
 * all devices have been updated to V3.
 *
 * This module sets up the production dependencies then calls main.
 *
 */
var main            = require('./src/main');
var config          = require('./config');
var particle        = require('./device/particle');

function handler(event, context) {
    config.Device = particle;
    main.handler(event, context, config);
}

module.exports = {
    handler:handler,
    config:config
};
