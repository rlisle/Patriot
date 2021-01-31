/**
 * Created by Ron on 3/20/17.
 */
'use strict';

var constants = require('../constants');

module.exports = function MockResponse() {
    return {
        response: undefined,
        reprompt: undefined,
        card: undefined,
        sendSet: false,
        endSession: undefined,
        say: function (response, reprompt) {
            this.response = response;
            this.reprompt = reprompt;
            return this;
        },
        send: function() {
            this.sendSet = true;
            return this;
        },
        fail: function() {
            this.didFail = true;
            return this;
        },
        shouldEndSession: function(shouldEnd) {
            this.endSession = shouldEnd;
            return this;
        }
    }
};
