/* Discovery tests */
'use strict';

var discovery = require('../src/discovery');
var constants = require('../constants');
var helper    = require('../src/helper');

var chai = require("chai");
var chaiAsPromised = require("chai-as-promised");
chai.use(chaiAsPromised);
var expect = chai.expect;
chai.should();

var mockDevice = require('./MockDevice');
var mockContext = require('./MockContext');

describe("Discovery request", function() {

    let testConfig = {
        Helper          : helper,
        accessToken     : "BogusAccessToken",
        PublishEndPoint : 'BogusURL',
        Device          : mockDevice
    }

    it("should call device getEndpoint",function(){
        var event = { directive: { payload: { scope: { token: "testToken"}}}};
        discovery.handler(event, mockContext, testConfig);
        expect(mockDevice.controlReceived).to.equal("getEndpoints");
    });

});
