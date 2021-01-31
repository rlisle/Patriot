'use strict';

Object.defineProperty(exports, "__esModule", {
	value: true
});

var _getIterator2 = require('babel-runtime/core-js/get-iterator');

var _getIterator3 = _interopRequireDefault(_getIterator2);

var _assign = require('babel-runtime/core-js/object/assign');

var _assign2 = _interopRequireDefault(_assign);

var _classCallCheck2 = require('babel-runtime/helpers/classCallCheck');

var _classCallCheck3 = _interopRequireDefault(_classCallCheck2);

var _createClass2 = require('babel-runtime/helpers/createClass');

var _createClass3 = _interopRequireDefault(_createClass2);

var _Particle = require('./Particle');

var _Particle2 = _interopRequireDefault(_Particle);

var _Library = require('./Library');

var _Library2 = _interopRequireDefault(_Library);

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

var Client = function () {
	function Client(_ref) {
		var auth = _ref.auth,
		    _ref$api = _ref.api,
		    api = _ref$api === undefined ? new _Particle2.default() : _ref$api;
		(0, _classCallCheck3.default)(this, Client);

		(0, _assign2.default)(this, { auth: auth, api: api });
	}

	(0, _createClass3.default)(Client, [{
		key: 'ready',
		value: function ready() {
			return Boolean(this.auth);
		}

		/**
   * Get firmware library objects
   * @param  {Object} query The query parameters for libraries. See Particle.listLibraries
   * @return {Promise}
   */

	}, {
		key: 'libraries',
		value: function libraries() {
			var _this = this;

			var query = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : {};

			return this.api.listLibraries((0, _assign2.default)({}, query, { auth: this.auth })).then(function (payload) {
				var libraries = payload.body.data || [];
				return libraries.map(function (l) {
					return new _Library2.default(_this, l);
				});
			});
		}

		/**
   * Get one firmware library object
   * @param  {String} name Name of the library to fetch
   * @param  {Object} query The query parameters for libraries. See Particle.getLibrary
   * @return {Promise}
   */

	}, {
		key: 'library',
		value: function library(name) {
			var _this2 = this;

			var query = arguments.length > 1 && arguments[1] !== undefined ? arguments[1] : {};

			return this.api.getLibrary((0, _assign2.default)({}, query, { name: name, auth: this.auth })).then(function (payload) {
				var library = payload.body.data || {};
				return new _Library2.default(_this2, library);
			});
		}

		/**
   * Get list of library versions
   * @param  {String} name Name of the library to fetch
   * @param  {Object} query The query parameters for versions. See Particle.getLibraryVersions
   * @return {Promise}
   */

	}, {
		key: 'libraryVersions',
		value: function libraryVersions(name) {
			var _this3 = this;

			var query = arguments.length > 1 && arguments[1] !== undefined ? arguments[1] : {};

			return this.api.getLibraryVersions((0, _assign2.default)({}, query, { name: name, auth: this.auth })).then(function (payload) {
				var libraries = payload.body.data || [];
				return libraries.map(function (l) {
					return new _Library2.default(_this3, l);
				});
			});
		}

		/**
   * Contribute a new library version
   * @param  {Buffer} archive The compressed archive with the library source
   * @return {Promise}
   */

	}, {
		key: 'contributeLibrary',
		value: function contributeLibrary(archive) {
			var _this4 = this;

			return this.api.contributeLibrary({ archive: archive, auth: this.auth }).then(function (payload) {
				var library = payload.body.data || {};
				return new _Library2.default(_this4, library);
			}, function (error) {
				_this4._throwError(error);
			});
		}

		/**
   * Make the the most recent private library version public
   * @param  {string} name The name of the library to publish
   * @return {Promise} To publish the library
   */

	}, {
		key: 'publishLibrary',
		value: function publishLibrary(name) {
			var _this5 = this;

			return this.api.publishLibrary({ name: name, auth: this.auth }).then(function (payload) {
				var library = payload.body.data || {};
				return new _Library2.default(_this5, library);
			}, function (error) {
				_this5._throwError(error);
			});
		}

		/**
   * Delete an entire published library
   * @param  {String} $0.name Name of the library to delete
   * @param  {String} $0.force Key to force deleting a public library
   * @return {Promise}
   */

	}, {
		key: 'deleteLibrary',
		value: function deleteLibrary(_ref2) {
			var _this6 = this;

			var name = _ref2.name,
			    version = _ref2.version,
			    force = _ref2.force;

			return this.api.deleteLibrary({ name: name, force: force, auth: this.auth }).then(function (payload) {
				return true;
			}, function (error) {
				_this6._throwError(error);
			});
		}
	}, {
		key: '_throwError',
		value: function _throwError(error) {
			if (error.body && error.body.errors) {
				var errorMessages = error.body.errors.map(function (e) {
					return e.message;
				}).join('\n');
				throw new Error(errorMessages);
			}
			throw error;
		}
	}, {
		key: 'downloadFile',
		value: function downloadFile(url) {
			return this.api.downloadFile({ url: url });
		}

		/**
   * @param {Object} files Object containing files to be compiled
   * @param {Number} platformId Platform id number of the device you are compiling for
   * @param {String} targetVersion System firmware version to compile against
   * @return {Promise}
   * @deprecated Will be removed in 6.5
   */

	}, {
		key: 'compileCode',
		value: function compileCode(files, platformId, targetVersion) {
			return this.api.compileCode({ files: files, platformId: platformId, targetVersion: targetVersion, auth: this.auth });
		}

		/**
   * @param {String} $0.deviceId Device ID or Name
   * @param {Boolean} $0.signal   Signal on or off
   * @return {Promise}
   * @deprecated Will be removed in 6.5
   */

	}, {
		key: 'signalDevice',
		value: function signalDevice(_ref3) {
			var signal = _ref3.signal,
			    deviceId = _ref3.deviceId;

			return this.api.signalDevice({ signal: signal, deviceId: deviceId, auth: this.auth });
		}

		/**
   * @return {Promise}
   * @deprecated Will be removed in 6.5
   */

	}, {
		key: 'listDevices',
		value: function listDevices() {
			return this.api.listDevices({ auth: this.auth });
		}

		/**
   * @return {Promise}
   * @deprecated Will be removed in 6.5
   */

	}, {
		key: 'listBuildTargets',
		value: function listBuildTargets() {
			return this.api.listBuildTargets({ onlyFeatured: true, auth: this.auth }).then(function (payload) {
				var targets = [];
				var _iteratorNormalCompletion = true;
				var _didIteratorError = false;
				var _iteratorError = undefined;

				try {
					for (var _iterator = (0, _getIterator3.default)(payload.body.targets), _step; !(_iteratorNormalCompletion = (_step = _iterator.next()).done); _iteratorNormalCompletion = true) {
						var target = _step.value;
						var _iteratorNormalCompletion2 = true;
						var _didIteratorError2 = false;
						var _iteratorError2 = undefined;

						try {
							for (var _iterator2 = (0, _getIterator3.default)(target.platforms), _step2; !(_iteratorNormalCompletion2 = (_step2 = _iterator2.next()).done); _iteratorNormalCompletion2 = true) {
								var platform = _step2.value;

								targets.push({
									version: target.version,
									platform: platform,
									prerelease: target.prereleases.indexOf(platform) > -1,
									firmware_vendor: target.firmware_vendor
								});
							}
						} catch (err) {
							_didIteratorError2 = true;
							_iteratorError2 = err;
						} finally {
							try {
								if (!_iteratorNormalCompletion2 && _iterator2.return) {
									_iterator2.return();
								}
							} finally {
								if (_didIteratorError2) {
									throw _iteratorError2;
								}
							}
						}
					}
				} catch (err) {
					_didIteratorError = true;
					_iteratorError = err;
				} finally {
					try {
						if (!_iteratorNormalCompletion && _iterator.return) {
							_iterator.return();
						}
					} finally {
						if (_didIteratorError) {
							throw _iteratorError;
						}
					}
				}

				return targets;
			}, function (error) {});
		}
	}, {
		key: 'trackingIdentity',
		value: function trackingIdentity() {
			var _ref4 = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : {},
			    _ref4$full = _ref4.full,
			    full = _ref4$full === undefined ? false : _ref4$full,
			    context = _ref4.context;

			return this.api.trackingIdentity({ full: full, context: context, auth: this.auth }).then(function (payload) {
				return payload.body;
			});
		}
	}]);
	return Client;
}();

exports.default = Client;
module.exports = exports['default'];
//# sourceMappingURL=Client.js.map