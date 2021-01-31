/* Control tests */
'use strict';

var chai = require("chai");
var chaiAsPromised = require("chai-as-promised");
chai.use(chaiAsPromised);
var expect = chai.expect;
chai.should();

var control   = require('../src/control');
var constants = require('../constants');
var helper    = require('../src/helper');

var mockDevice = require('./MockDevice');

describe("Control", function() {


    var testConfig = {
        Device: mockDevice
    };
    var testContext = {
        successData: undefined,
        errorData: undefined,
        succeed: function(data) {
            this.successData = data;
        },
        fail: function(error) {
            this.errorData = error;
        }
    };

    describe("turn on request",function() {
        it("calls device controlOn", function() {
            var request = {
                "directive" : { 
                    "header" : { 
                        "name": constants.REQUEST_TURN_ON,
                        "correlationToken": "testToken"
                    },
                    "endpoint": { "scope": { "token": "testToken" }}
                },
                "payload": { "appliance": { applianceId: 'test'}}
            };
            control.handler( request, testContext, testConfig);
            expect(mockDevice.controlReceived).to.equal('controlOn');
        })
    });

    describe("turn off request",function() {
        it("calls device controlOff", function() {
            var testEvent = {
                "header" : { "name" : constants.REQUEST_TURN_OFF },
                "payload": { "appliance": { applianceId: 'test'}}
            };
            control.handler( testEvent, testContext, testConfig);
            expect(mockDevice.controlReceived).to.equal('controlOff');
        })
    });

    describe("percent request",function() {
        it("calls device percentage", function() {
            var testEvent = {
                "header" : { "name" : constants.REQUEST_SET_PERCENT },
                "payload": {
                    "appliance": { applianceId: 'test'},
                    "percentageState": {
                        "value": 123
                    }
                }
            };
            control.handler( testEvent, testContext, testConfig);
            expect(mockDevice.controlReceived).to.equal('percentage');
        })
    });

    describe("unknown request (decrement target)",function() {
        it("return unsupported operation error", function() {
            var testEvent = {
                "header" : { "name" : constants.REQUEST_DEC_TARGET },
                "payload": { "appliance": { applianceId: 'test'}}
            };
            control.handler( testEvent, testContext, testConfig);
            let errorName = testContext.errorData.header.name;
            expect(errorName).to.equal('Unsupported operation');
        })
    })
});
