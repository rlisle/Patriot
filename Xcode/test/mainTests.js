/* Main tests */
/* Run tests: "npm test" */
'use strict';
var chai = require("chai");
var expect = chai.expect;

var indexjs     = require('../index');
var main        = require('../src/main');
var helper      = require('../src/helper');
var constants   = require('../constants');
var mockDevice  = require('./MockDevice');

// Move to a separate config test
describe('Index',function() {

    describe('config',function() {

        var config = indexjs.config;

        it('has expected properties', function() {
            expect(config).to.not.be.null;
            expect(config.AccessToken).to.not.be.null;
            expect(config.PublishEndPoint).to.equal('https://api.particle.io/v1/devices/events');
        })
    })
})

describe("Main request handler", function() {

    var testConfig = {
        Device: mockDevice
    };

    var testContext = {
        errorMsg: undefined,
        fail: function(error) {
            this.errorMsg = error;
        }
    }

    describe("with invalid namespace",function() {

        var unexpectedEvent = { "header": { "namespace": 'bogusNamespace' }, "payload": { "accessToken": "none" } };

        it('returns unexpected info exception',function() {
            main.handler(unexpectedEvent, testContext, testConfig);
            expect(testContext.errorMsg.payload.exception.code).to.equal('UnexpectedInformationReceivedError');
        });
    });

    describe("with discovery namespace",function() {

        var discoveryEvent = { "header": { "namespace": constants.NAMESPACE_DISCOVERY }, "payload": { "accessToken": "none" } };

        it('calls device getAppliances',function() {
            main.handler(discoveryEvent, testContext, testConfig);
            expect(mockDevice.controlReceived).to.equal('getEndpoints');
        });
    });

    describe("with control namespace",function() {

        var controlEvent = {
            "header": {
                "namespace": constants.NAMESPACE_CONTROL,
                "name": constants.REQUEST_TURN_ON
            },
            "payload": {
                "appliance": {
                    "applianceId": 'TestDevice'
                }
            }
        };

        it('calls device controlOn',function() {
            main.handler(controlEvent, testContext, testConfig);
            expect(mockDevice.controlReceived).to.equal('controlOn');
            expect(mockDevice.deviceNameReceived).to.equal('TestDevice');
        });
    });

});
