/**
 * Report
 *
 * Handle ReportState requests
 *
 * It will signal its completion by calling context.succeed()
 * or context.fail()
 */
'use strict';

var constants   = require('../constants');
var helper      = require('./helper');

/**
 * ReportState Handler
 * @param request
 * @param context
 * @param config
 * @return Nothing. Context.succeed() or fail() will be called.
 */
function handler(request, context, config) {

    var requestedName = request.directive.header.name;
    var token = request.directive.endpoint.scope.token;
    var timestamp = new Date().toISOString();
    var correlation = request.directive.header.correlationToken;
    var endpoint = request.directive.endpoint;

    helper.log('ReportState', request);

    switch (requestedName) {    // This better be ReportState or we shouldn't be here.
        case "ReportState":
            helper.log("Calling device reportState");
            config.Device.reportState(request, context, config).then(function(result){
                
                helper.log("Result from device reportState", result);

                // For now, hard code the entire response. Refactor later after its understood better.
                let contextResult = {
                    "properties": [{
                        "namespace": "Alexa.PowerController",
                        "name": "powerState",
                        "value": (result ? "ON" : "OFF"),
                        "timeOfSample": timestamp,
                        "uncertaintyInMilliseconds": 50
                    }]
                };
                var responseHeader = request.directive.header;
                responseHeader.namespace = "Alexa";
                responseHeader.name = "StateReport";
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
                helper.log("ReportState error",error);
                context.fail("ReportState error: "+error);
            });
            break;

        default:
            helper.log("Report - unsupported operation",requestedName);
            context.fail("Report - Unsupported operation: "+requestedName);
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
