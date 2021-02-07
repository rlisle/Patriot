/**
 * Particle Tests
 *
 * These tests use a particle.io test account
 * and a dedicated test Photon named 'RonTest'
 * running the RonTest.ino sketch.
 *
 * It includes a blueLed device.
 *
 * To run just these tests: "npm test -- -g Particle"
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

const testDeviceName   = 'RonTest';
const testFriendlyName = 'blueLed';
//const testAccessToken  = process.env.PARTICLE_TEST_TOKEN; // Test account token. Update and use when publishing
const testAccessToken = process.env.PARTICLE_ACCESS_TOKEN;  // PRIVATE! Keep account keys in env

describe("Particle", function () {

    var particle = new Particle();

    it('loads particle sdk', function () {
        expect(particle).to.not.be.undefined;
    });

    describe('getVariable',function() {

        it('returns test value',function() {

            return device.getVariable(testDeviceName, 'Devices', testAccessToken)
                .then(function(data) {
                    expect(data).to.contain('blueLed');
                    //expect(data).to.be.equal('blueled');
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

            return device.publish('patriot/blueLed', '100', testAccessToken)
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
})
