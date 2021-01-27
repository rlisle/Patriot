/* Discovery */
'use strict';

var constants = require('../constants');
var helper      = require('./helper');

/**
 * Discovery Handler (Alexa V3 API)
 * @param event
 * @param context
 * @param config
 * @return Nothing. Context.succeed() or fail() will be called.
 */
function handler(event, context, config) {

    var header = helper.createHeader(constants.NAMESPACE_DISCOVERY, constants.RESPONSE_DISCOVER);
    var accessToken = event.directive.payload.scope.token;

    config.Device.getEndpoints(accessToken).then(function (endpoints) {

        helper.log('V3 Discovery endpoints',endpoints);

        var payload = {
            "endpoints": endpoints
        };
        let result = helper.createEvent(header, payload);
        context.succeed(result)

    }).catch(function (error) {
        helper.log('Discovery error 1', 'Returning empty array. Error = '+JSON.stringify(error));
        var payload = {
            "endpoints": []
        }
        // Discovery does not return errors, just an empty array
        let result = helper.createEvent(header, payload);
        context.succeed(result);
    });
}

/**
 * Legacy V2 API Discovery Handler
 * Amazon says that this needs to be supported until older devices upgraded
 * @param event
 * @param context
 * @param config
 * @return Nothing. Context.succeed() or fail() will be called.
 */
function handlerV2(event, context, config) {

    helper.log("V2 discovery handler",event);

    var header = helper.createHeaderV2(constants.NAMESPACE_DISCOVERY_V2, constants.RESPONSE_DISCOVER_V2);
    config.Device.getAppliancesV2(config.AccessToken).then(function (appliances) {

        helper.log('V2 Discovery appliances',appliances);

        var payload = {
            "discoveredAppliances": appliances
        };
        let result = helper.createDirective(header, payload);
        context.succeed(result)

    }).catch(function (error) {
        helper.log('V2 Discovery error', 'Returning empty array. Error = '+JSON.stringify(error));
        var payload = {
            "discoveredAppliances": []
        }
        // Discovery does not return errors.
        let result = helper.createDirective(header, payload);
        context.succeed(result);
    });
}

module.exports = {
    handlerV2: handlerV2,
    handler: handler
};
