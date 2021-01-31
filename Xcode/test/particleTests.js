/**
 * Particle Tests
 *
 * These tests use a particle.io test account
 * and a dedicated test Photon named 'myPhoton'
 * running the starter.ino sketch.
 *
 */
'use strict';

var chai = require("chai");
var chaiAsPromised = require("chai-as-promised");
chai.use(chaiAsPromised);
var expect = chai.expect;
chai.should();

var device = require('../device/particle');
var constants = require('../constants');
var helper = require('../src/helper');
var config = require('../config');
var Particle = require('particle-api-js');

const testDeviceName   = 'UnitTest';
const testApplianceId  = 'photon';      // V2
const testFriendlyName = 'photon';
//const testAccessToken  = '8e9c665522d622e2ee3f1b91ef44a7f512f18a06';    // Test account. Use when publishing
const testAccessToken = '3030a462f44ce7c80b2eabadd4a0f7f3c3fc6233';     // PRIVATE! Ron's account only!

describe("Particle", function () {

    var particle = new Particle();

    it('loads particle sdk', function () {
        expect(particle).to.not.be.undefined;
    });

    // Not testing Login on test account since only token exposed, not id and pw.
    // describe("login", function () {
    //
    //     it('through direct SDK call returns 40 char token for valid username and password', function () {
    //         return particle.login({username: config.UserName, password: config.Password})
    //             .then(function (data) {
    //                 var token = data.body.access_token;
    //                 expect(token).to.be.lengthOf(40);
    //             });
    //     });
    //
    //     it('returns 40 char token for valid username and password', function () {
    //
    //         return device.login(config)
    //             .then(function (token) {
    //                 expect(token).to.be.lengthOf(40);
    //             });
    //     })
    // });

    describe('getVariable',function() {

        it('returns test value',function() {

            return device.getVariable(testDeviceName, 'Devices', testAccessToken)
                .then(function(data) {
                    expect(data).to.be.equal('blueled');
                })
        })
    });

    // Currently no functions supported by starter.ino
    // describe('callFunction',function() {
    //
    //     it('returns test response value',function() {
    //
    //         return device.callFunction('RonTest', 'TestFunction', 'TEST:55', token)
    //             .then(function(data) {
    //                 expect(data).to.be.equal(55);
    //             })
    //     })
    // });

    describe("publish", function () {

        it("returns ok", function () {

            return device.publish('patriot', 'photon:100', testAccessToken)
                .then(function (status) {
                    expect(status).to.be.true;
                });
        });

        // it("generates test event", function() {
        //
        //     var event;
        //
        //     //TODO: subscribe to an event
        //     particle.getEventStream({ deviceId: 'RonTest', name: 'test', auth: token })
        //         .then(function(stream) {
        //             stream.on('event', function(data) {
        //                 event = data;
        //                 console.log("Event: " + data);
        //             });
        //             device.publish('test:100',testConfig);
        //         });
        //     expect(event).to.eventually.equal('test:100');
        //
        //     // return device.publish('test:100',testConfig)
        //     //     .then(function(status) {
        //     //         expect(status.ok).to.be.true;
        //     //     })
        // });

        //TODO: subscribe to event and ensure it is actually sent

    });

    // V2 Legacy
//    describe("getAppliancesV2", function () {
//
//        this.timeout(3000);
//
//        it("returns list of appliances", function () {
//            return device.getAppliancesV2(testAccessToken).then((appliances) => {
//                    expect(appliances).to.not.be.null;
//            })
//        });
//
//        it("returns only the test action", function () {
//            return device.getAppliancesV2(testAccessToken).then((appliances) => {
//                    let testDevices = appliances.filter(function (item) {
//                        let applianceId = item.applianceId;
//                        return applianceId === testApplianceId;
//                    });
//                    //helper.log('getAppliancesV2',appliances);
//                    // The device name should not be exposed, only the behavior name
//                    expect(appliances).to.have.length(1);
//                    //helper.log('testDevices',testDevices);
//                    let testDevice = testDevices[0];
//                    expect(testDevice.friendlyName).to.equal(testFriendlyName);
//            });
//        });
//    })
//
//    describe('discoveryDevice',function(){
//        let result = device.discoveryDevice("My Device Name", "MyController");
//        describe('returns formatted object',function() {
//            it('formats ID to lowercase without spaces',function() {
//                expect(result.applianceId).to.equal('mydevicename');
//            });
//            it('leaves spaces in friendly name lowercase',function() {
//                expect(result.friendlyName).to.equal('my device name');
//            });
//            it('creates description',function() {
//                expect(result.friendlyDescription).to.equal('My Device Name connected via Particle.io');
//            })
//        })
//    })
})
