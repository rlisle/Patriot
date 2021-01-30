/**
 * Particle.io API
 * This function interfaces with the particle.io REST server.
 * It returns a promise to simplify asynchronous handling.
 *
 * Publish/Subscribe Model
 * curl https://api.particle.io/v1/devices/events -d "name=lislerv" -d "data=BoothSw3:On" -d access_token=3030a462f44ce7c80b2eabadd4a0f7f3c3fc6233
 *
 * Updated: 2/3/18 use devices only, not supported.
 */
'use strict';

const Promise = require('bluebird');

var constants = require('../constants');
var helper      = require('../src/helper');

var Particle = require('particle-api-js');

var particle = new Particle();
var token;

/**
 * Handle control On
 * @param event
 * @param context
 * @param config
 * @returns {Promise} success or fail
 */
function controlOn(event, context, config) {
    let command = event.directive.endpoint.cookie.command;
    let data = command+":100";
    let accessToken = event.directive.endpoint.scope.token;
    return publish(config.EventName, data, accessToken).then(function(result) {
        return result;
    });
}

function controlOff(event, context, config) {
    let command = event.directive.endpoint.cookie.command;  // use endpointId instead?
    var data = command+":0"
    let accessToken = event.directive.endpoint.scope.token;
    return publish(config.EventName, data, accessToken).then(function(result) {
        return result;
    });
}

function percentage(event, context, config) {
    var command = event.directive.endpoint.endpointId;
    var percentage = event.directive.payload.powerLevel;
    var data = command+":"+percentage;
    let accessToken = event.directive.endpoint.scope.token;
    return publish(config.EventName, data, accessToken).then(function(result) {
        return result;
    });
}

function adjust(event, context, config) {
    var command = event.directive.endpoint.endpointId;
    var delta = event.directive.payload.powerLevelDelta;
    var data = command+"+"+delta;
    let accessToken = event.directive.endpoint.scope.token;
    return publish(config.EventName, data, accessToken).then(function(result) {
        return result;
    });
}

function login(config) {
    return particle.login({username: config.UserName, password: config.Password}).then(
        function (data) {
            token = data.body.access_token;
            return token;
        },
        function(err) {
            console.log("Could not log in: "+err);
            return err;
        });

}

function getVariable(deviceId, variableName, token) {
    let args = { deviceId: deviceId, name: variableName, auth: token};
    return particle.getVariable(args).then(function(response){
        return response.body.result;
    })
}

function callFunction(deviceId, functionName, argument, token) {
    let args = { deviceId: deviceId, name: functionName, argument: argument, auth: token};
    return particle.callFunction(args).then(function(response){
        return response.body.return_value;
    })
}

function publish(name, data, token) {
    let args = { name: name, data: data, auth: token, isPrivate: true };
    return particle.publishEvent(args).then(function(response){
        let result = response.body.ok;
        return result;
    });
}

/**
 * getEndpoints
 * @param token
 * @returns {Promise}
 */
function getEndpoints(token) {

    // Get list of active Photons
    return particle.listDevices({ auth: token}).then(function(result) {

            helper.log("getEndpoints listDevices",result);

            let statusCode = result.statusCode; // s/b 200
            if(statusCode != 200) {
                console.log("Error listing devices: "+statusCode);
                return [];
            }

            let photonNames = result.body.filter(function(item) {
                return item.connected;
            }).map(function(item){
                return item.name;
            });

            // Read list of devices from each Photon
            // Use promises to wait until all are done'
            let endpoints = [];
            let promises = [];
            photonNames.forEach(function(name) {

                let p1 = getVariable(name, 'Devices', token).then(function (devices) {

                    helper.log("getEndpoints devices", devices);

                        let deviceStrings = devices.split(',');     //Split string into array of individual name:type strings
                        deviceStrings.forEach(function (item) {
                            helper.log("getEndpoints device", item);
                            if(item) {
                                endpoints.push(endpointInfo(item))
                            }
                        });
                    },
                    function (err) {
                        console.log("Error reading Devices variable for " + name);
                    });
                promises.push(p1);

/*                let p2 = getVariable(name, 'Supported', token).then(function (supported) {
                            let supportedStrings = supported.split(',');
                            supportedStrings.forEach(function (item) {
                                if(item) {
                                    endpoints.push(sceneEndpointInfo(item));
                                }
                            });
                        },
                        function (err) {
                            console.log("Error reading Supported variable for " + name);
                        });
                promises.push(p2);
 */
            });

            return Promise.all(promises).then(values => {
                helper.log("getEndpoints endpoints", endpoints);
                return endpoints;
            }).timeout(5000,"timeout");
        },
        function(err){
            console.log("Error listing devices. Returning appliances = "+JSON.stringify(appliances));
            return endpoints;
        });
}

/* I think these are V2 methods */
/** Define a device or scene
 *      The name can be multiple words and should be exactly what the user
 *      will speak (eg. "Table lamp").
 *      The appliance ID will be the name with spaces removed and converted to lower case.
 *      It is what is specified in the event.
 *
 *      Note that a space in the ID causes the device to not appear.
 */
function discoveryDevice(name, controllerId) {
    helper.log("discoveryDevice", name);
    let device = applianceInfo(name);
    device.actions.push('setPercentage');
    device.additionalApplianceDetails.controllerId = controllerId;
    return device;
}

/** Define a scene
 *    id is the data that will be passed to particle
 *       Note that a space causes the device to not appear.
 *    name is the word(s) spoken to control it. " scene" will be appended.
 *    description will also have " scene" appended.
 */
function discoveryScene(name) {
    helper.log("discoveryScene", name);
    let scene = applianceInfo(name);
    return scene;
}

// This function will create the endpoint JSON for each device or activity
// The name argument is used for the friendlyName.
// It will be lower-cased and spaces removed for the endpointId
// TODO: support different device types
function endpointInfo(name) {
    helper.log('Device endpoint info:',name);
    //TODO: Strip off prefix and use to determine device type: Curtain, Door, Fan, Light, Motion, Switch, Temp
    let dispCategory = "LIGHT";
    if(name.charAt(1)==':') {
        var devType = name.charAt(0);
        name = name.substring(2);
        if(devType=='C') {
            dispCategory = "INTERIOR_BLIND"; // Best fit for Curtain
        } else if(devType=='D') {
            dispCategory = "DOOR";
        } else if(devType=='F') {
            dispCategory = "FAN";
        } else if(devType=='L') {
            dispCategory = "LIGHT"; // redundant
        } else if(devType=='M') {
            dispCategory = "MOTION_SENSOR";
        } else if(devType=='S') {
            dispCategory = "SWITCH";
        } else if(devType=='T') {
            dispCategory = "TEMPERATURE_SENSOR";
        }
    }
    let id = name.replace(/\s/g,'').toLocaleLowerCase();    // Remove spaces. Numbers, letters, _-=#;:?@& only
    let friendlyName = name.toLocaleLowerCase();            // Name lower case to simplify compares. No special chars
    let description = name + ' RvSmartHome';  //
    var endpoint = {
        "endpointId": id,
        "friendlyName": friendlyName,
        "description": description,
        "manufacturerName": 'Ron Lisle',
        "displayCategories": [          // LIGHT, SMARTPLUG, SWITCH, CAMERA, DOOR, TEMPERATURE_SENSOR,
            dispCategory                // THERMOSTAT, SMARTLOCK, SCENE_TRIGGER, ACTIVITY_TRIGGER, OTHER
        ],
        "cookie": {                     // This can be anything we want, and will be passed back
            "name":     name,           // This is the mixed case name
            "command":  friendlyName    // Currently using lower case with spaces
        },
        "capabilities":[
            {
                "type": "AlexaInterface",
                "interface": "Alexa",
                "version": "3"
            },                          // TODO: use devType to determine capabilities
            {
                "type": "AlexaInterface",              // Is . needed? Doc shows both ways
                "interface": "Alexa.PowerController",
                "version": "3",
                "properties": {
                    "supported": [
                        {
                            "name": "powerState"
                        }
                    ],
                    "retrievable": true
                }
            },
            {
                "type": "AlexaInterface",              // Is . needed? Doc shows both ways
                "interface": "Alexa.PowerLevelController",
                "version": "3",
                "properties": {
                    "supported": [
                        {
                            "name": "powerLevel"
                        }
                    ],
                    "retrievable": true
                }
            }
        ]
    };
    helper.log("Device endpoint info returning", endpoint);
    return endpoint;
}

// This function will create the endpoint JSON for each supported activity (scene)
// The name argument is used for the friendlyName.
// It will be lower-cased and spaces removed for the endpointId
function sceneEndpointInfo(name) {
    helper.log('Scene endpoint info:',name);
    let id = name.replace(/\s/g,'').toLocaleLowerCase();    // Remove spaces. Numbers, letters, _-=#;:?@& only
    let friendlyName = name.toLocaleLowerCase();            // Name lower case to simplify compares. No special chars
    let description = name + ' connected via Particle.io';  //
    var endpoint = {
        "endpointId": id,
        "friendlyName": friendlyName,
        "description": description,
        "manufacturerName": 'Ron Lisle',
        "displayCategories": [          // LIGHT, SMARTPLUG, SWITCH, CAMERA, DOOR, TEMPERATURE_SENSOR,
            "SCENE_TRIGGER"             // THERMOSTAT, SMARTLOCK, SCENE_TRIGGER, ACTIVITY_TRIGGER, OTHER
        ],
        "cookie": {                     // This can be anything we want, and will be passed back
            "name":     name,           // This is the mixed case name
            "command":  friendlyName    // Currently using lower case with spaces
        },
        "capabilities":[
            {
                "type": "AlexaInterface",              // Is . needed? Doc shows both ways
                "interface": "Alexa.SceneController",
                "version": "3",
                "supportsDeactivation": true,   // Example shows not enclosed in "properties" like the devices code
                "proactivelyReported": true
            }
        ]
    };
    return endpoint;
}

module.exports = {
    controlOn:controlOn,
    controlOff:controlOff,
    percentage:percentage,
    adjust:adjust,
    login:login,
    getVariable:getVariable,
    callFunction:callFunction,
    publish:publish,
    getEndpoints:getEndpoints,
    discoveryDevice:discoveryDevice,
    discoveryScene:discoveryScene
};
