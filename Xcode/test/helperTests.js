/* Helper tests */
'use strict';
var chai = require("chai");
var expect = chai.expect;

var Helper    = require('../src/helper');
var constants = require('../constants');

describe('Helper',function() {

    describe('log',function() {

        it('writes title and message to console', function() {
            expect(Helper.log("Test","Testing...")).to.not.throw;
        })
    })

    describe('createDirective',function() {

        describe('creates an object', function() {

            let result = Helper.createDirective('TestHeader','TestPayload');

            it('with the specified header property', function() {
                expect(result.header).to.equal('TestHeader');
            });

            it('with the specified payload property', function() {
                expect(result.payload).to.equal('TestPayload');
            });

        });
    })

    describe('createHeader', function() {

        let result = Helper.createHeader('TestNamespace', 'TestName');

        describe('returns an object',function() {

            describe('containing a UUID messageId', function () {

                it('that is 36 characters long', function () {
                    expect(result.messageId).to.be.lengthOf(36);
                })

                it('has 4 dashes in the correct spots', function () {
                    expect(result.messageId[8]).to.equal('-');
                    expect(result.messageId[13]).to.equal('-');
                    expect(result.messageId[18]).to.equal('-');
                    expect(result.messageId[23]).to.equal('-');
                })

                it('is different between subsequent calls', function () {
                    let result2 = Helper.createHeader('TestNamespace2', 'TestName2');
                    expect(result.messageId).to.not.equal(result2.messageId);
                })

            })

            it('containing a specified namespace', function () {
                expect(result.namespace).to.be.equal('TestNamespace');
            })

            it('containing a specified name', function () {
                expect(result.name).to.be.equal('TestName');
            })
        })
    })

});
