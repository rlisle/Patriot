'use strict';

Object.defineProperty(exports, "__esModule", {
	value: true
});

var _promise = require('babel-runtime/core-js/promise');

var _promise2 = _interopRequireDefault(_promise);

var _assign = require('babel-runtime/core-js/object/assign');

var _assign2 = _interopRequireDefault(_assign);

var _classCallCheck2 = require('babel-runtime/helpers/classCallCheck');

var _classCallCheck3 = _interopRequireDefault(_classCallCheck2);

var _createClass2 = require('babel-runtime/helpers/createClass');

var _createClass3 = _interopRequireDefault(_createClass2);

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

/* Library
 * Represents a version of a library contributed in the cloud.
 */

var Library = function () {
	function Library(client, data) {
		(0, _classCallCheck3.default)(this, Library);

		// Make client non-enumerable so it doesn't show up in Object.keys, JSON.stringify, etc
		Object.defineProperty(this, 'client', { value: client });
		this._assignAttributes(data);
		this.downloadUrl = data.links && data.links.download;
	}

	(0, _createClass3.default)(Library, [{
		key: '_assignAttributes',
		value: function _assignAttributes(data) {
			(0, _assign2.default)(this, data.attributes);
		}

		/**
   * Download the compressed file containing the source code for this library version.
   * @return {Promise} Resolves to the .tar.gz compressed source code
   */

	}, {
		key: 'download',
		value: function download() {
			if (!this.downloadUrl) {
				return _promise2.default.reject(new Error('No download URL for this library'));
			}
			return this.client.downloadFile(this.downloadUrl);
		}

		/* TODO: add a versions() method to fetch an array of library objects */

	}]);
	return Library;
}();

exports.default = Library;
module.exports = exports['default'];
//# sourceMappingURL=Library.js.map