'use strict';

Object.defineProperty(exports, "__esModule", {
	value: true
});
exports.default = binaryParser;
// Binary parser for superagent

function binaryParser(res, fn) {
	/* global Buffer */
	var data = [];
	res.on('data', function (chunk) {
		return data.push(chunk);
	});
	res.on('end', function () {
		return fn(null, Buffer.concat(data));
	});
}
module.exports = exports['default'];
//# sourceMappingURL=superagent-binary-parser.js.map