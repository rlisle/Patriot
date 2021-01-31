'use strict';

var constants = require('../constants');

module.exports = function MockRequest(intentName) {
    return {
        data: {
            request: {
                intent: {
                    name: intentName,
                    slots: {
                        "SLOTNAME": {
                            "name": "SLOTNAME",
                            "value": "slotvalue"
                        }
                    }
                }
            }
        }
    }
}
