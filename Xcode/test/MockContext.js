/**
 * Created by Ron on 3/21/17.
 */
'use strict';

var mockContext = {
    successMsg: 'none',
    failMsg:    'none',

    succeed: function(msg) {
        this.successMsg = msg;
    },

    fail: function(msg) {
        this.failMsg = msg;
    }
};

module.exports = {
    mockContext: mockContext
}
