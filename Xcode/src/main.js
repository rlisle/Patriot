/**
 * main.js
 *
 * Handles incoming events from Alexa Smart Home APIs
 *
 * Completes handling by calling context.succeed(result) or context.fail(controllError)
 *
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
    // helper.log("handler context",context);
    helper.log("handler config",config);

    var version = getDirectiveVersion(event);
    var requestedNamespace;
    if(version == "3") {
        // Note in V3 that token is passed in different places,
        //      so we'll parse it out in each handler instead of here.
        requestedNamespace = event.directive.header.namespace;

    } else {                        // V2
        config.AccessToken = event.payload.accessToken;
        requestedNamespace = event.header.namespace;
    }

    try {

        helper.log("namespace",requestedNamespace);

        switch (requestedNamespace) {

            // V3 API
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


            // Legacy V2 API (remove in the future)
            case constants.NAMESPACE_DISCOVERY_V2:
                helper.log("MAIN","V2 discovery namespace");
                discovery.handlerV2(event, context, config);
                break;

            case constants.NAMESPACE_CONTROL_V2:
                control.handlerV2(event, context, config);
                break;

            default:
                helper.log("Main unexpected namespace", requestedNamespace);
                if(version == "3") {
                    //Note: the documented error format doesn't work as of 9/10/17
                    //var correlation = event.directive.header.correlationToken;    // doesn't exist for discovery
                    //var token = event.directive.payload.scope.token;
                    //var endpointId = event.directive.payload.endpointId;
                    //context.fail(helper.createError(correlation, token, endpointId, constants.ERROR_UNEXPECTED_INFO, "Unexpected requested namespace"));
                    context.fail("Main: unexpected namespace: "+requestedNamespace);
                } else {
                    context.fail(helper.createErrorV2(requestedNamespace, "Unexpected info", constants.ERROR_UNEXPECTED_INFO, "Unexpected info"));
                }
        }

    } catch (error) {
        helper.log("Main Error", error);
        if(version == "3") {
            //Note: the documented error format doesn't work as of 9/10/17
            context.fail("Main - Unknown error");
        } else {
            context.fail(helper.createErrorV2(requestedNamespace, error, constants.ERROR_DRIVER_INTERNAL, "Unknown exception"));
        }
    }
    /*
     Note: callback will be called by handlers above
     by calling context.succeed(result) or context.fail(controllError)
     */
}

function getDirectiveVersion(request)
{
    if(request.directive == undefined) return "2";
    let version = request.directive.header.payloadVersion;
    if(version == undefined) return "2";
    return version;
}

module.exports = {
    handler: handler
};
