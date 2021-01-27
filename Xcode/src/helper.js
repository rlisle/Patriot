/* Helper methods */
'use strict';

function log(title, msg) {
  console.log('**** ' + title + ': ' + JSON.stringify(msg) + '\n');
}

function createEvent(header, payload, endpoint) {
    if(endpoint != undefined) { //v3
        return {
            "event": {
                "header": header,
                "endpoint": endpoint,
                "payload": payload
            }
        };
    }
    return {
        "event": {
            "header": header,
            "payload": payload
        }
    };
}

function createDirective(header, payload, endpoint) {
    if(endpoint != undefined) { // v3
        return {
            "header" : header,
            "endpoint": endpoint,
            "payload" : payload
        };
    }
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

function createHeaderV2(namespace, name) {

    return {
        "messageId": createMessageId(),
        "namespace": namespace,
        "name": name,
        "payloadVersion": "2"
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

function createErrorV2(namespace, name, code, description) {
    var headers = {
        namespace: namespace,
        name: name,
        payloadVersion: '1'
    };

    var payload = {
        exception: {
            code: code,
            description: description
        }
    };

    var result = {
        header: headers,
        payload: payload
    };

    return result;
}

module.exports = {
    createEvent:createEvent,
    createMessageId:createMessageId,
    createHeader:createHeader,
    createHeaderV2:createHeaderV2,
    createDirective:createDirective,
    createError:createError,
    createErrorV2:createErrorV2,
    log:log
};
