/**
 * main.js
 *
 * Handles incoming events from Alexa Smart Home APIs
 *
 * Completes handling by calling context.succeed(result) or context.fail(controllError)
 *
 * 1/27/21 Removed V2 code
 * 9/28/17 Updated to v3 API
 */
'use strict';

const constants     = require('../constants');
var helper          = require('./helper');
var control         = require('./control');
var discovery       = require('./discovery');

/**
 */

/**
 * Main entry point.
 *
 * Incoming events from Alexa Smart Home APIs are processed via this method
 * after having dependencies injected by index.js and passed in via the config argument
 *
 * @param event   passed by Alexa API
 * @param context passed by Alexa API
 * @param config  dependencies
 */
function handler(event, context, config) {

    helper.log("handler event",event);
    helper.log("handler config",config);

    var requestedNamespace = event.directive.header.namespace;

    try {

        helper.log("namespace",requestedNamespace);

        switch (requestedNamespace) {

            case constants.NAMESPACE_ALEXA:
//                reportState.handler(event, context, config);
                helper.log("MAIN","V3 Alexa namespace");
                break;

            case constants.NAMESPACE_DISCOVERY:
                discovery.handler(event, context, config);
                break;

            case constants.NAMESPACE_POWER_CONTROLLER:
                control.handler(event, context, config);
                break;

            case constants.NAMESPACE_POWER_LEVEL:
                control.handler(event, context, config);
                break;

            default:
                helper.log("Main unexpected namespace", requestedNamespace);
                    //Note: the documented error format doesn't work as of 9/10/17
                    //var correlation = event.directive.header.correlationToken;    // doesn't exist for discovery
                    //var token = event.directive.payload.scope.token;
                    //var endpointId = event.directive.payload.endpointId;
                    //context.fail(helper.createError(correlation, token, endpointId, constants.ERROR_UNEXPECTED_INFO, "Unexpected requested namespace"));
                    context.fail("Main: unexpected namespace: "+requestedNamespace);
        }

    } catch (error) {
        helper.log("Main Error", error);
        //Note: the documented error format doesn't work as of 9/10/17
        context.fail("Main - Unknown error");
    }
    /*
     Note: callback will be called by handlers above
     by calling context.succeed(result) or context.fail(controllError)
     */
}

module.exports = {
    handler: handler
};
