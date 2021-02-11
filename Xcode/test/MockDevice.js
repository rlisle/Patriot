'use strict';

var constants = require('../constants');

var controlReceived     = 'none';
var deviceNameReceived  = 'none';
var valueReceived       = 'none';

function controlOn(event, context, config) {
    this.controlReceived = 'controlOn';
    this.deviceNameReceived = event.directive.header.name;
    return Promise.resolve(this);
}

function controlOff(event, context, config) {
    this.controlReceived = 'controlOff';
    this.deviceNameReceived = event.directive.header.name;
    return Promise.resolve(this);
}

function percentage(event, context, config) {
    this.controlReceived     = 'percentage';
    this.deviceNameReceived  = event.directive.header.name;
    this.valueReceived       = event.directive.payload.powerLevel.value;
    return Promise.resolve(this);
}

function getVariable(deviceId, variableName, token) {
    this.controlReceived = 'getVariable';
    return Promise.resolve(this);
}

function callFunction(deviceId, functionName, argument, token) {
    this.controlReceived = 'callFunction';
    return Promise.resolve(this);
}

function publish(name, data, token) {
    this.controlReceived = 'publish';
    return Promise.resolve(this);
}

function getEndpoints(token) {
    this.controlReceived = 'getEndpoints';
    return Promise.resolve(this);
}

module.exports = {
    controlReceived:controlReceived,
    deviceNameReceived:deviceNameReceived,
    valueReceived:valueReceived,
    controlOn:controlOn,
    controlOff:controlOff,
    percentage:percentage,
    getVariable:getVariable,
    callFunction:callFunction,
    publish:publish,
    getEndpoints:getEndpoints
};
