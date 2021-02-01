/* Discovery */
'use strict';

var constants = require('../constants');
var helper      = require('./helper');

/**
 * Discovery Handler
 * @param event
 * @param context
 * @param config
 * @return Nothing. Context.succeed() or fail() will be called.
 */
function handler(event, context, config) {

    var header = helper.createHeader(constants.NAMESPACE_DISCOVERY, constants.RESPONSE_DISCOVER);
    var accessToken = event.directive.payload.scope.token;

    config.Device.getEndpoints(accessToken).then(function (endpoints) {

        helper.log('Discovery endpoints',endpoints);

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

module.exports = {
    handler: handler
};
