/**
 * Control
 *
 * Handle control requests (eg. turn on, off, etc)
 *
 * It will signal its completion by calling context.succeed()
 * or context.fail()
 */
'use strict';

var constants   = require('../constants');
var helper      = require('./helper');

function handler(request, context, config) {

    var requestedName = request.directive.header.name;
    var token = request.directive.endpoint.scope.token;  // example says payload, but its endpoint
    var timestamp = new Date().toISOString();
    var correlation = request.directive.header.correlationToken;
    var endpoint = request.directive.endpoint;

    helper.log('Control',requestedName);

    switch (requestedName) {
        case "TurnOn":
            config.Device.controlOn(request, context, config).then(function(result){
                // For now, hard code the entire response. Refactor later after its understood better.
                let contextResult = {
                    "properties": [{
                        "namespace": "Alexa.PowerController",
                        "name": "powerState",
                        "value": "ON",
                        "timeOfSample": timestamp,
                        "uncertaintyInMilliseconds": 50
                    }]
                };
                var responseHeader = request.directive.header;
                responseHeader.namespace = "Alexa";
                responseHeader.name = "Response";
                responseHeader.messageId = responseHeader.messageId + "-R";
                var response = {
                    "context": contextResult,
                    "event": {
                        "header": responseHeader,
                        "endpoint": endpoint,
                        "payload": {}
                    }
                };
                helper.log("Returning response: ", response);
                context.succeed(response);
            },
            function(error) {
                helper.log("Control turn on error",error);
                context.fail("Control - turn on error: "+error);
            });
            break;

        case "TurnOff":
            config.Device.controlOff(request, context, config).then(function(result){
                    // For now, hard code the entire response. Refactor later after its understood better.
                    let contextResult = {
                        "properties": [{
                            "namespace": "Alexa.PowerController",
                            "name": "powerState",
                            "value": "OFF",
                            "timeOfSample": timestamp,
                            "uncertaintyInMilliseconds": 50
                        }]
                    };
                    var responseHeader = request.directive.header;
                    responseHeader.namespace = "Alexa";
                    responseHeader.name = "Response";
                    responseHeader.messageId = responseHeader.messageId + "-R";
                    var response = {
                        "context": contextResult,
                        "event": {
                            "header": responseHeader,
                            "endpoint": endpoint,
                            "payload": {}
                        }
                    };
                    helper.log("Returning response: ", response);
                    context.succeed(response);
                },
                function(error) {
                    helper.log("Control turn off error",error);
                    context.fail("Control - turn off error: "+error);
                });
            break;

        case "SetPowerLevel":
            var percent = request.directive.payload.powerLevel;
            config.Device.percentage(request, context, config).then(function(result){
                    // For now, hard code the entire response. Refactor later after its understood better.
                    let contextResult = {
                        "properties": [{
                            "namespace": "Alexa.PowerLevelController",
                            "name": "powerLevel",
                            "value": percent,
                            "timeOfSample": timestamp,
                            "uncertaintyInMilliseconds": 50
                        }]
                    };
                    var responseHeader = request.directive.header;
                    responseHeader.namespace = "Alexa";
                    responseHeader.name = "Response";
                    responseHeader.messageId = responseHeader.messageId + "-R";
                    var response = {
                        "context": contextResult,
                        "event": {
                            "header": responseHeader,
                            "endpoint": endpoint,
                            "payload": {}
                        }
                    };
                    helper.log("Returning response: ", response);
                    context.succeed(response);
                },
                function(error) {
                    helper.log("Control power level error",error);
                    context.fail("Control - power level error: "+error);
                });
            break;

        case "AdjustPowerLevel":
            var delta = request.directive.payload.powerLevelDelta;
            config.Device.percentage(request, context, config).then(function(result){
                    // For now, hard code the entire response. Refactor later after its understood better.
                    let contextResult = {
                        "properties": [{
                            "namespace": "Alexa.PowerLevelController",
                            "name": "powerLevel",
                            "value": delta,                 //TODO: this s/b the new level, not the delta
                            "timeOfSample": timestamp,
                            "uncertaintyInMilliseconds": 50
                        }]
                    };
                    var responseHeader = request.directive.header;
                    responseHeader.namespace = "Alexa";
                    responseHeader.name = "Response";
                    responseHeader.messageId = responseHeader.messageId + "-R";
                    var response = {
                        "context": contextResult,
                        "event": {
                            "header": responseHeader,
                            "endpoint": endpoint,
                            "payload": {}
                        }
                    };
                    helper.log("Returning response: ", response);
                    context.succeed(response);
                },
                function(error) {
                    helper.log("Control power level error",error);
                    context.fail("Control - power level error: "+error);
                });
            break;

        default:
            helper.log("Control - unsupported operation",requestedName);
            context.fail("Control - Unsupported operation: "+requestedName);
    }
    /*
     Note: No return value or promise.
           Device methods will call context.succeed(result) or context.fail(controllError)
     */
}

/* Export our public functions */
module.exports = {
    handler:handler,
};
