/* Helper methods */
'use strict';

function log(title, msg) {
  console.log('**** ' + title + ': ' + JSON.stringify(msg) + '\n');
}

function createEvent(header, payload, endpoint) {
    return {
        "event": {
            "header": header,
            "endpoint": endpoint,
            "payload": payload
        }
    };
}

function createDirective(header, payload, endpoint) {
  return {
    "header" : header,
    "payload" : payload
  };
}

function createMessageId() {

  var d = new Date().getTime();
  var uuid = 'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'.replace(/[xy]/g, function(c) {
    var r = (d + Math.random()*16)%16 | 0;
    d = Math.floor(d/16);
    return (c=='x' ? r : (r&0x3|0x8)).toString(16);
  });
  return uuid;
}

function createHeader(namespace, name, correlation) {

    if(correlation != undefined) {
        return {
            "messageId": createMessageId(),
            "correlationToken": correlation,
            "namespace": namespace,
            "name": name,
            "payloadVersion": "3"
        };
    }
  return {
    "messageId": createMessageId(),
    "namespace": namespace,
    "name": name,
    "payloadVersion": "3"
  };

}

function createError(correlation, token, endpointId, type, message) {
    var header = {
        "namespace": "Alexa",
        "name": "ErrorResponse",
        "messageId": createMessageId(),
        "correlationToken": correlation,
        payloadVersion: "3"
    };

    var endpoint = {
        "scope": {
            "type": "BearerToken",
            "token": token
        },
        "endpointId": endpointId
    }

    var payload = {
        "type": type,
        "message": message
    };

    var result = {
        "event": {
            "header": header,
            "endpoint": endpoint,
            "payload": payload
        }
    };

    return result;
}

module.exports = {
    createEvent:createEvent,
    createMessageId:createMessageId,
    createHeader:createHeader,
    createDirective:createDirective,
    createError:createError,
    log:log
};
