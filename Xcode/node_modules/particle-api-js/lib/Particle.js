'use strict';

Object.defineProperty(exports, "__esModule", {
	value: true
});

var _assign = require('babel-runtime/core-js/object/assign');

var _assign2 = _interopRequireDefault(_assign);

var _classCallCheck2 = require('babel-runtime/helpers/classCallCheck');

var _classCallCheck3 = _interopRequireDefault(_classCallCheck2);

var _createClass2 = require('babel-runtime/helpers/createClass');

var _createClass3 = _interopRequireDefault(_createClass2);

var _superagent = require('superagent');

var _superagent2 = _interopRequireDefault(_superagent);

var _superagentBinaryParser = require('./superagent-binary-parser');

var _superagentBinaryParser2 = _interopRequireDefault(_superagentBinaryParser);

var _Defaults = require('./Defaults');

var _Defaults2 = _interopRequireDefault(_Defaults);

var _EventStream = require('./EventStream');

var _EventStream2 = _interopRequireDefault(_EventStream);

var _Agent = require('./Agent');

var _Agent2 = _interopRequireDefault(_Agent);

var _Client = require('./Client');

var _Client2 = _interopRequireDefault(_Client);

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

/**
 * Particle Cloud API wrapper.
 *
 * See <https://docs.particle.io/reference/javascript/> for examples
 * of using the `Particle` class.
 *
 * Most Particle methods take a single unnamed argument object documented as
 * `options` with key/value pairs for each option.
 */
var Particle = function () {
	/**
  * Contructor for the Cloud API wrapper.
  *
  * Create a new Particle object and call methods below on it.
  *
  * @param  {Object} options Options for this API call Options to be used for all requests (see [Defaults](../src/Defaults.js))
  */
	function Particle() {
		var options = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : {};
		(0, _classCallCheck3.default)(this, Particle);

		// todo - this seems a bit dangerous - would be better to put all options/context in a contained object
		(0, _assign2.default)(this, _Defaults2.default, options);
		this.context = {};
		this.agent = new _Agent2.default(this.baseUrl);
	}

	(0, _createClass3.default)(Particle, [{
		key: '_isValidContext',
		value: function _isValidContext(name, context) {
			return (name === 'tool' || name === 'project') && context !== undefined;
		}
	}, {
		key: 'setContext',
		value: function setContext(name, context) {
			if (context !== undefined) {
				if (this._isValidContext(name, context)) {
					this.context[name] = context;
				} else {
					throw Error('uknown context name or undefined context: ' + name);
				}
			}
		}

		/**
   * Builds the final context from the context parameter and the context items in the api.
   * @param  {Object} context       The invocation context, this takes precedence over the local context.
   * @return {Object} The context to use.
   * @private
   */

	}, {
		key: '_buildContext',
		value: function _buildContext(context) {
			return (0, _assign2.default)(this.context, context);
		}

		/**
   * Login to Particle Cloud using an existing Particle acccount.
   * @param  {Object} options Options for this API call
   * @param  {String} options.username      Username for the Particle account
   * @param  {String} options.password      Password for the Particle account
   * @param  {Number} options.tokenDuration How long the access token should last in seconds
   * @return {Promise}
   */

	}, {
		key: 'login',
		value: function login(_ref) {
			var username = _ref.username,
			    password = _ref.password,
			    _ref$tokenDuration = _ref.tokenDuration,
			    tokenDuration = _ref$tokenDuration === undefined ? this.tokenDuration : _ref$tokenDuration,
			    context = _ref.context;

			return this.request({ uri: '/oauth/token', form: {
					username: username,
					password: password,
					grant_type: 'password',
					client_id: this.clientId,
					client_secret: this.clientSecret,
					expires_in: tokenDuration
				}, method: 'post', context: context });
		}

		/**
   * Create a user account for the Particle Cloud
   * @param  {Object} options Options for this API call
   * @param  {String} options.username Email of the new user
   * @param  {String} options.password Password
   * @param  {String} options.accountInfo Object that contains account information fields such as user real name, company name, business account flag etc
   * @return {Promise}
   */

	}, {
		key: 'createUser',
		value: function createUser(_ref2) {
			var username = _ref2.username,
			    password = _ref2.password,
			    accountInfo = _ref2.accountInfo,
			    context = _ref2.context;

			return this.post('/v1/users', {
				username: username,
				password: password,
				account_info: accountInfo
			}, undefined, context);
		}

		/**
   * Verify new user account via verification email
   * @param  {Object} options Options for this API call
   * @param  {String} options.token the string token sent in the verification email
   * @return {Promise}
   */

	}, {
		key: 'verifyUser',
		value: function verifyUser(_ref3) {
			var token = _ref3.token,
			    context = _ref3.context;

			return this.post('/v1/user/verify', {
				token: token
			}, undefined, context);
		}

		/**
   * Send reset password email for a Particle Cloud user account
   * @param  {Object} options Options for this API call
   * @param  {String} options.username Email of the user
   * @return {Promise}
   */

	}, {
		key: 'resetPassword',
		value: function resetPassword(_ref4) {
			var username = _ref4.username,
			    context = _ref4.context;

			return this.post('/v1/user/password-reset', { username: username }, undefined, context);
		}

		/**
   * Revoke an access token
   * @param  {Object} options Options for this API call
   * @param  {String} options.username Username of the Particle cloud account that the token belongs to.
   * @param  {String} options.password Password for the account
   * @param  {String} options.token    Access token you wish to revoke
   * @return {Promise}
   */

	}, {
		key: 'deleteAccessToken',
		value: function deleteAccessToken(_ref5) {
			var username = _ref5.username,
			    password = _ref5.password,
			    token = _ref5.token,
			    context = _ref5.context;

			return this.delete('/v1/access_tokens/' + token, {
				access_token: token
			}, { username: username, password: password }, context);
		}

		/**
   * Revoke the current session access token
   * @param  {Object} options Options for this API call
   * @param  {String} options.auth         Access Token
   * @return {Promise}
   */

	}, {
		key: 'deleteCurrentAccessToken',
		value: function deleteCurrentAccessToken(_ref6) {
			var auth = _ref6.auth,
			    context = _ref6.context;

			return this.delete('/v1/access_tokens/current', undefined, auth, context);
		}

		/**
   * List all valid access tokens for a Particle Cloud account
   * @param  {Object} options Options for this API call
   * @param  {String} options.username Username
   * @param  {String} options.password Password
   * @return {Promise}
   */

	}, {
		key: 'listAccessTokens',
		value: function listAccessTokens(_ref7) {
			var username = _ref7.username,
			    password = _ref7.password,
			    context = _ref7.context;

			return this.get('/v1/access_tokens', { username: username, password: password }, undefined, context);
		}

		/**
   * Retrieves the information that is used to identify the current login for tracking.
   * @param  {Object} options Options for this API call
   * @param  {String} options.auth      The access token
   * @param  {Boolean} options.full      When true, retrieve all information for registering a user with the tracking API. When false,
   *  retrieve only the unique tracking ID for the current login.
   * @param  {Object} options.context   Context information.
   * @returns {Promise<Object>} Resolve the tracking identify of the current login
   */

	}, {
		key: 'trackingIdentity',
		value: function trackingIdentity() {
			var _ref8 = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : {},
			    auth = _ref8.auth,
			    _ref8$full = _ref8.full,
			    full = _ref8$full === undefined ? false : _ref8$full,
			    context = _ref8.context;

			return this.get('/v1/user/identify', auth, full ? undefined : { tracking: 1 }, context);
		}

		/**
   * List devices claimed to the account or product
   * @param  {Object} options Options for this API call
   * @param  {String} [options.deviceId]   (Product only) Filter results to devices with this ID (partial matching)
   * @param  {String} [options.deviceName] (Product only) Filter results to devices with this name (partial matching)
   * @param  {String} [options.sortAttr]   (Product only) The attribute by which to sort results. See API docs for options.
   * @param  {String} [options.sortDir]    (Product only) The direction of sorting. See API docs for options.
   * @param  {Number} [options.page]       (Product only) Current page of results
   * @param  {Number} [options.perPage]    (Product only) Records per page
   * @param  {String} [options.product]    List devices in this product ID or slug
   * @param  {String} options.auth         Access Token
   * @return {Promise}
   */

	}, {
		key: 'listDevices',
		value: function listDevices(_ref9) {
			var deviceId = _ref9.deviceId,
			    deviceName = _ref9.deviceName,
			    sortAttr = _ref9.sortAttr,
			    sortDir = _ref9.sortDir,
			    page = _ref9.page,
			    perPage = _ref9.perPage,
			    product = _ref9.product,
			    auth = _ref9.auth,
			    context = _ref9.context;

			var uri = product ? '/v1/products/' + product + '/devices' : '/v1/devices';
			var query = product ? { deviceId: deviceId, deviceName: deviceName, sortAttr: sortAttr, sortDir: sortDir, page: page, per_page: perPage } : undefined;
			return this.get(uri, auth, query, context);
		}

		/**
   * Get detailed informationa about a device
   * @param  {Object} options Options for this API call
   * @param  {String} options.deviceId  Device ID or Name
   * @param  {String} [options.product] Device in this product ID or slug
   * @param  {String} options.auth      Access token
   * @return {Promise}
   */

	}, {
		key: 'getDevice',
		value: function getDevice(_ref10) {
			var deviceId = _ref10.deviceId,
			    product = _ref10.product,
			    auth = _ref10.auth,
			    context = _ref10.context;

			var uri = this.deviceUri({ deviceId: deviceId, product: product });
			return this.get(uri, auth, undefined, context);
		}

		/**
   * Claim a device to the account. The device must be online and unclaimed.
   * @param  {Object} options Options for this API call
   * @param  {String} options.deviceId Device ID
   * @param  {String} options.auth     Access Token
   * @return {Promise}
   */

	}, {
		key: 'claimDevice',
		value: function claimDevice(_ref11) {
			var deviceId = _ref11.deviceId,
			    requestTransfer = _ref11.requestTransfer,
			    auth = _ref11.auth,
			    context = _ref11.context;

			return this.post('/v1/devices', {
				id: deviceId,
				request_transfer: !!requestTransfer
			}, auth, context);
		}

		/**
   * Add a device to a product or move device out of quarantine.
   * @param  {Object} options Options for this API call
   * @param  {String} options.deviceId Device ID
   * @param  {String} options.product  Add to this product ID or slug
   * @param  {String} options.auth     Access Token
   * @return {Promise}
   */

	}, {
		key: 'addDeviceToProduct',
		value: function addDeviceToProduct(_ref12) {
			var deviceId = _ref12.deviceId,
			    product = _ref12.product,
			    auth = _ref12.auth,
			    context = _ref12.context;

			var uri = '/v1/products/' + product + '/devices';
			return this.post(uri, {
				id: deviceId
			}, auth, context);
		}

		/**
   * Unclaim / Remove a device from your account or product, or deny quarantine
   * @param  {Object} options Options for this API call
   * @param  {String} options.deviceId Device ID or Name
   * @param  {Boolean} [options.deny]  (Product only) Deny this quarantined device, instead of removing an already approved device
   * @param  {String} options.product  Remove from this product ID or slug
   * @param  {String} options.auth     Access Token
   * @return {Promise}
   */

	}, {
		key: 'removeDevice',
		value: function removeDevice(_ref13) {
			var deviceId = _ref13.deviceId,
			    deny = _ref13.deny,
			    product = _ref13.product,
			    auth = _ref13.auth,
			    context = _ref13.context;

			var uri = this.deviceUri({ deviceId: deviceId, product: product });
			var data = product ? { deny: deny } : undefined;
			return this.delete(uri, data, auth, context);
		}

		/**
   * Unclaim a product device its the owner, but keep it in the product
   * @param  {Object} options Options for this API call
   * @param  {String} options.deviceId Device ID or Name
   * @param  {String} options.product  Remove from this product ID or slug
   * @param  {String} options.auth     Access Token
   * @return {Promise}
   */

	}, {
		key: 'removeDeviceOwner',
		value: function removeDeviceOwner(_ref14) {
			var deviceId = _ref14.deviceId,
			    deny = _ref14.deny,
			    product = _ref14.product,
			    auth = _ref14.auth,
			    context = _ref14.context;

			var uri = '/v1/products/' + product + '/devices/' + deviceId + '/owner';
			return this.delete(uri, undefined, auth, context);
		}

		/**
   * Rename a device
   * @param  {Object} options Options for this API call
   * @param  {String} options.deviceId Device ID or Name
   * @param  {String} options.name     Desired Name
   * @param  {String} [options.product] Rename device in this product ID or slug
   * @param  {String} options.auth     Access Token
   * @return {Promise}
   */

	}, {
		key: 'renameDevice',
		value: function renameDevice(_ref15) {
			var deviceId = _ref15.deviceId,
			    name = _ref15.name,
			    product = _ref15.product,
			    auth = _ref15.auth,
			    context = _ref15.context;

			return this.updateDevice({ deviceId: deviceId, name: name, product: product, auth: auth, context: context });
		}

		/**
   * Instruct the device to turn on/off the LED in a rainbow pattern
   * @param  {Object} options Options for this API call
   * @param  {String} options.deviceId Device ID or Name
   * @param  {Boolean} options.signal   Signal on or off
   * @param  {String} [options.product] Device in this product ID or slug
   * @param  {String} options.auth     Access Token
   * @return {Promise}
   */

	}, {
		key: 'signalDevice',
		value: function signalDevice(_ref16) {
			var deviceId = _ref16.deviceId,
			    signal = _ref16.signal,
			    product = _ref16.product,
			    auth = _ref16.auth,
			    context = _ref16.context;

			return this.updateDevice({ deviceId: deviceId, signal: signal, product: product, auth: auth, context: context });
		}

		/**
   * Store some notes about device
   * @param  {Object} options Options for this API call
   * @param  {String} options.deviceId  Device ID or Name
   * @params {String} options.notes     Your notes about this device
   * @param  {String} [options.product] Device in this product ID or slug
   * @param  {String} options.auth      Access Token
   * @return {Promise}
   */

	}, {
		key: 'setDeviceNotes',
		value: function setDeviceNotes(_ref17) {
			var deviceId = _ref17.deviceId,
			    notes = _ref17.notes,
			    product = _ref17.product,
			    auth = _ref17.auth,
			    context = _ref17.context;

			return this.updateDevice({ deviceId: deviceId, notes: notes, product: product, auth: auth, context: context });
		}

		/**
   * Mark device as being used in development of a product so it opts out of automatic firmware updates
   * @param  {Object} options Options for this API call
   * @param  {String} options.deviceId      Device ID or Name
   * @param  {Boolean} options.development  Set to true to mark as development, false to return to product fleet
   * @param  {String} options.product       Device in this product ID or slug
   * @param  {String} options.auth          Access Token
   * @return {Promise}
   */

	}, {
		key: 'markAsDevelopmentDevice',
		value: function markAsDevelopmentDevice(_ref18) {
			var deviceId = _ref18.deviceId,
			    _ref18$development = _ref18.development,
			    development = _ref18$development === undefined ? true : _ref18$development,
			    product = _ref18.product,
			    auth = _ref18.auth,
			    context = _ref18.context;

			return this.updateDevice({ deviceId: deviceId, development: development, product: product, auth: auth, context: context });
		}

		/**
   * Mark device as being used in development of a product so it opts out of automatic firmware updates
   * @param  {Object} options Options for this API call
   * @param  {String} options.deviceId      Device ID or Name
   * @params {Number} options.desiredFirmwareVersion Lock the product device to run this firmware version.
   * @params {Boolean} [options.flash]      Immediately flash firmware indicated by desiredFirmwareVersion
   * @param  {String} options.product       Device in this product ID or slug
   * @param  {String} options.auth          Access Token
   * @return {Promise}
   */

	}, {
		key: 'lockDeviceProductFirmware',
		value: function lockDeviceProductFirmware(_ref19) {
			var deviceId = _ref19.deviceId,
			    desiredFirmwareVersion = _ref19.desiredFirmwareVersion,
			    flash = _ref19.flash,
			    product = _ref19.product,
			    auth = _ref19.auth,
			    context = _ref19.context;

			return this.updateDevice({ deviceId: deviceId, desiredFirmwareVersion: desiredFirmwareVersion, flash: flash, product: product, auth: auth, context: context });
		}

		/**
   * Mark device as receiving automatic firmware updates
   * @param  {Object} options Options for this API call
   * @param  {String} options.deviceId      Device ID or Name
   * @param  {String} options.product       Device in this product ID or slug
   * @param  {String} options.auth          Access Token
   * @return {Promise}
   */

	}, {
		key: 'unlockDeviceProductFirmware',
		value: function unlockDeviceProductFirmware(_ref20) {
			var deviceId = _ref20.deviceId,
			    product = _ref20.product,
			    auth = _ref20.auth,
			    context = _ref20.context;

			return this.updateDevice({ deviceId: deviceId, desiredFirmwareVersion: null, product: product, auth: auth, context: context });
		}

		/**
   * Update multiple device attributes at the same time
   * @param  {Object} options Options for this API call
   * @param  {String} options.deviceId       Device ID or Name
   * @param  {String} [options.name]         Desired Name
   * @param  {Boolean} options.signal        Signal device on or off
   * @params {String} [options.notes]        Your notes about this device
   * @param  {Boolean} [options.development] (Product only) Set to true to mark as development, false to return to product fleet
   * @params {Number} [options.desiredFirmwareVersion] (Product only) Lock the product device to run this firmware version.
   *                                              Pass `null` to unlock firmware and go back to released firmware.
   * @params {Boolean} [options.flash]       (Product only) Immediately flash firmware indicated by desiredFirmwareVersion
   * @param  {String} [options.product]      Device in this product ID or slug
   * @param  {String} options.auth           Access Token
   * @return {Promise}
   */

	}, {
		key: 'updateDevice',
		value: function updateDevice(_ref21) {
			var deviceId = _ref21.deviceId,
			    name = _ref21.name,
			    signal = _ref21.signal,
			    notes = _ref21.notes,
			    development = _ref21.development,
			    desiredFirmwareVersion = _ref21.desiredFirmwareVersion,
			    flash = _ref21.flash,
			    product = _ref21.product,
			    auth = _ref21.auth,
			    context = _ref21.context;

			signal = signal ? '1' : '0';
			var uri = this.deviceUri({ deviceId: deviceId, product: product });
			var data = product ? { name: name, signal: signal, notes: notes, development: development, desired_firmware_version: desiredFirmwareVersion, flash: flash } : { name: name, signal: signal, notes: notes };
			return this.put(uri, data, auth, context);
		}

		/**
   * Provision a new device for products that allow self-provisioning
   * @param  {Object} options Options for this API call
   * @param  {String} options.productId Product ID where to create this device
   * @param  {String} options.auth      Access Token
   * @return {Promise}
   */

	}, {
		key: 'provisionDevice',
		value: function provisionDevice(_ref22) {
			var productId = _ref22.productId,
			    auth = _ref22.auth,
			    context = _ref22.context;

			return this.post('/v1/devices', { product_id: productId }, auth, context);
		}

		/**
   * Generate a claim code to use in the device claiming process.
   * To generate a claim code for a product, the access token MUST belong to a
   * customer of the product.
   * @param  {Object} options Options for this API call
   * @param  {String} [options.iccid] ICCID of the SIM card used in the Electron
   * @param  {String} [options.product] Device in this product ID or slug
   * @param  {String} options.auth  Access Token
   * @return {Promise}
   */

	}, {
		key: 'getClaimCode',
		value: function getClaimCode(_ref23) {
			var iccid = _ref23.iccid,
			    product = _ref23.product,
			    auth = _ref23.auth,
			    context = _ref23.context;

			var uri = product ? '/v1/products/' + product + '/device_claims' : '/v1/device_claims';
			return this.post(uri, { iccid: iccid }, auth, context);
		}
	}, {
		key: 'validatePromoCode',
		value: function validatePromoCode(_ref24) {
			var auth = _ref24.auth,
			    promoCode = _ref24.promoCode,
			    context = _ref24.context;

			return this.get('/v1/promo_code/' + promoCode, auth, undefined, context);
		}
	}, {
		key: 'changeProduct',
		value: function changeProduct(_ref25) {
			var deviceId = _ref25.deviceId,
			    productId = _ref25.productId,
			    auth = _ref25.auth,
			    context = _ref25.context;

			return this.put('/v1/devices/' + deviceId, {
				product_id: productId
			}, auth, context);
		}

		/**
   * Get the value of a device variable
   * @param  {Object} options Options for this API call
   * @param  {String} options.deviceId Device ID or Name
   * @param  {String} options.name     Variable name
   * @param  {String} [options.product] Device in this product ID or slug
   * @param  {String} options.auth     Access Token
   * @return {Promise}
   */

	}, {
		key: 'getVariable',
		value: function getVariable(_ref26) {
			var deviceId = _ref26.deviceId,
			    name = _ref26.name,
			    product = _ref26.product,
			    auth = _ref26.auth,
			    context = _ref26.context;

			var uri = product ? '/v1/products/' + product + '/devices/' + deviceId + '/' + name : '/v1/devices/' + deviceId + '/' + name;
			return this.get(uri, auth, undefined, context);
		}

		/**
   * Compile and flash application firmware to a device. Pass a pre-compiled binary to flash it directly to the device.
   * @param  {Object} options Options for this API call
   * @param  {String} options.deviceId      Device ID or Name
   * @param  {Object} options.files         Object containing files to be compiled and flashed. Keys should be the filenames, and the values should be a path or Buffer of the file contents.
   * @param  {String} [options.targetVersion=latest] System firmware version to compile against
   * @param  {String} options.auth          String
   * @return {Promise}
   */

	}, {
		key: 'flashDevice',
		value: function flashDevice(_ref27) {
			var deviceId = _ref27.deviceId,
			    files = _ref27.files,
			    targetVersion = _ref27.targetVersion,
			    auth = _ref27.auth,
			    context = _ref27.context;

			var form = {};
			if (targetVersion) {
				form.build_target_version = targetVersion;
			} else {
				form.latest = 'true';
			}
			return this.request({ uri: '/v1/devices/' + deviceId,
				files: files, auth: auth, form: form, context: context, method: 'put' });
		}

		/**
   * DEPRECATED: Flash the Tinker application to a device. Instead compile and flash the Tinker source code.
   * @param  {Object} options Options for this API call
   * @param  {String} options.deviceId Device ID or Name
   * @param  {String} options.auth     Access Token
   * @return {Promise}
   */

	}, {
		key: 'flashTinker',
		value: function flashTinker(_ref28) {
			var deviceId = _ref28.deviceId,
			    auth = _ref28.auth,
			    context = _ref28.context;

			/* eslint-disable no-console */
			if (console && console.warning) {
				console.warning('Particle.flashTinker is deprecated');
			}
			/* eslint-enable no-console */
			return this.put('/v1/devices/' + deviceId, {
				app: 'tinker'
			}, auth, context);
		}

		/**
   * Compile firmware using the Particle Cloud
   * @param  {Object} options Options for this API call
   * @param  {Object} options.files         Object containing files to be compiled. Keys should be the filenames, and the values should be a path or Buffer of the file contents.
   * @param  {Number} [options.platformId]    Platform id number of the device you are compiling for. Common values are 0=Core, 6=Photon, 10=Electron.
   * @param  {String} [options.targetVersion=latest] System firmware version to compile against
   * @param  {String} options.auth          Access Token
   * @return {Promise}
   */

	}, {
		key: 'compileCode',
		value: function compileCode(_ref29) {
			var files = _ref29.files,
			    platformId = _ref29.platformId,
			    targetVersion = _ref29.targetVersion,
			    auth = _ref29.auth,
			    context = _ref29.context;

			var form = { platform_id: platformId };
			if (targetVersion) {
				form.build_target_version = targetVersion;
			} else {
				form.latest = 'true';
			}
			return this.request({ uri: '/v1/binaries',
				files: files, auth: auth, form: form, context: context, method: 'post' });
		}

		/**
   * Download a firmware binary
   * @param  {Object} options Options for this API call
   * @param  {String} options.binaryId Binary ID received from a successful compile call
   * @param  {String} options.auth     Access Token
   * @return {Request}
   */

	}, {
		key: 'downloadFirmwareBinary',
		value: function downloadFirmwareBinary(_ref30) {
			var binaryId = _ref30.binaryId,
			    auth = _ref30.auth,
			    context = _ref30.context;

			var uri = '/v1/binaries/' + binaryId;
			var req = (0, _superagent2.default)('get', uri);
			req.use(this.prefix);
			this.headers(req, auth);
			if (this.debug) {
				this.debug(req);
			}
			return req;
		}

		/**
   * Send a new device public key to the Particle Cloud
   * @param  {Object} options Options for this API call
   * @param  {String} options.deviceId  Device ID or Name
   * @param  {(String|Buffer)} options.key       Public key contents
   * @param  {String} [options.algorithm=rsa] Algorithm used to generate the public key. Valid values are `rsa` or `ecc`.
   * @param  {String} options.auth      Access Token
   * @return {Promise}
   */

	}, {
		key: 'sendPublicKey',
		value: function sendPublicKey(_ref31) {
			var deviceId = _ref31.deviceId,
			    key = _ref31.key,
			    algorithm = _ref31.algorithm,
			    auth = _ref31.auth,
			    context = _ref31.context;

			return this.post('/v1/provisioning/' + deviceId, {
				deviceID: deviceId,
				publicKey: typeof key === 'string' ? key : key.toString(),
				filename: 'particle-api',
				order: 'manual_' + Date.now(),
				algorithm: algorithm || 'rsa'
			}, auth, context);
		}

		/**
   * Call a device function
   * @param  {Object} options Options for this API call
   * @param  {String} options.deviceId Device ID or Name
   * @param  {String} options.name     Function name
   * @param  {String} options.argument Function argument
   * @param  {String} [options.product] Device in this product ID or slug
   * @param  {String} options.auth     Access Token
   * @return {Promise}
   */

	}, {
		key: 'callFunction',
		value: function callFunction(_ref32) {
			var deviceId = _ref32.deviceId,
			    name = _ref32.name,
			    argument = _ref32.argument,
			    product = _ref32.product,
			    auth = _ref32.auth,
			    context = _ref32.context;

			var uri = product ? '/v1/products/' + product + '/devices/' + deviceId + '/' + name : '/v1/devices/' + deviceId + '/' + name;
			return this.post(uri, { args: argument }, auth, context);
		}

		/**
   * Get a stream of events
   * @param  {Object} options Options for this API call
   * @param  {String} [options.deviceId] Device ID or Name, or `mine` to indicate only your devices.
   * @param  {String} [options.name]     Event Name
   * @param  {String} [options.org]     Organization Slug
   * @param  {String} [options.product] Events for this product ID or slug
   * @param  {String} options.auth     Access Token
   * @return {Promise} If the promise resolves, the resolution value will be an EventStream object that will
   * emit 'event' events, as well as the specific named event.
   */

	}, {
		key: 'getEventStream',
		value: function getEventStream(_ref33) {
			var deviceId = _ref33.deviceId,
			    name = _ref33.name,
			    org = _ref33.org,
			    product = _ref33.product,
			    auth = _ref33.auth,
			    context = _ref33.context;

			var uri = '/v1/';
			if (org) {
				uri += 'orgs/' + org + '/';
			}

			if (product) {
				uri += 'products/' + product + '/';
			}

			if (deviceId) {
				uri += 'devices/';
				if (!(deviceId.toLowerCase() === 'mine')) {
					uri += deviceId + '/';
				}
			}

			uri += 'events';

			if (name) {
				uri += '/' + encodeURIComponent(name);
			}

			return new _EventStream2.default('' + this.baseUrl + uri, auth, { debug: this.debug }).connect();
		}

		/**
   * Publish a event to the Particle Cloud
   * @param  {Object} options Options for this API call
   * @param  {String} options.name      Event name
   * @param  {String} options.data      Event data
   * @param  {Boolean} options.isPrivate Should the event be publicly available?
   * @param  {String} [options.product]  Event for this product ID or slug
   * @param  {String} options.auth      Access Token
   * @return {Promise}
   */

	}, {
		key: 'publishEvent',
		value: function publishEvent(_ref34) {
			var name = _ref34.name,
			    data = _ref34.data,
			    isPrivate = _ref34.isPrivate,
			    product = _ref34.product,
			    auth = _ref34.auth,
			    context = _ref34.context;

			var uri = product ? '/v1/products/' + product + '/events' : '/v1/devices/events';
			var postData = { name: name, data: data, private: isPrivate };
			return this.post(uri, postData, auth, context);
		}

		/**
   * Create a webhook
   * @param  {Object} options Options for this API call
   * @param  {String} options.deviceId           Trigger webhook only for this device ID or Name
   * @param  {String} options.name               Webhook name
   * @param  {String} options.url                URL the webhook should hit
   * @param  {String} [options.requestType=POST]        HTTP method to use
   * @param  {Object} [options.headers]            Additional headers to add to the webhook
   * @param  {Object} [options.json]               JSON data
   * @param  {Object} [options.query]              Query string data
   * @param  {String} [options.body]               Custom webhook request body
   * @param  {Object} [options.responseTemplate]   Webhook response template
   * @param  {Object} [options.responseTopic]      Webhook response topic
   * @param  {Boolean} [options.rejectUnauthorized] Reject invalid HTTPS certificates
   * @params {Boolean} [options.noDefaults]        Don't include default event data in the webhook request
   * @param  {Object} [options.webhookAuth]        HTTP Basic Auth information
   * @param  {Object} [options.form]               Form data
   * @param  {String} [options.product]          Webhook for this product ID or slug
   * @param  {String} options.auth               Access Token
   * @return {Promise}
   */

	}, {
		key: 'createWebhook',
		value: function createWebhook(_ref35) {
			var deviceId = _ref35.deviceId,
			    name = _ref35.name,
			    url = _ref35.url,
			    requestType = _ref35.requestType,
			    headers = _ref35.headers,
			    json = _ref35.json,
			    query = _ref35.query,
			    body = _ref35.body,
			    responseTemplate = _ref35.responseTemplate,
			    responseTopic = _ref35.responseTopic,
			    rejectUnauthorized = _ref35.rejectUnauthorized,
			    webhookAuth = _ref35.webhookAuth,
			    noDefaults = _ref35.noDefaults,
			    form = _ref35.form,
			    product = _ref35.product,
			    auth = _ref35.auth,
			    context = _ref35.context;

			// deviceId: 'mine' is deprecated since webhooks only trigger on your device anyways
			if (deviceId === 'mine') {
				deviceId = undefined;
			}
			var uri = product ? '/v1/products/' + product + '/webhooks' : '/v1/webhooks';
			var data = { event: name, deviceid: deviceId, url: url, requestType: requestType, headers: headers, json: json, query: query, body: body, responseTemplate: responseTemplate, responseTopic: responseTopic, rejectUnauthorized: rejectUnauthorized, auth: webhookAuth, noDefaults: noDefaults, form: form };
			return this.post(uri, data, auth, context);
		}

		/**
   * Delete a webhook
   * @param  {Object} options Options for this API call
   * @param  {String} options.hookId Webhook ID
   * @param  {String} [options.product]          Webhook for this product ID or slug
   * @param  {String} options.auth   Access Token
   * @return {Promise}
   */

	}, {
		key: 'deleteWebhook',
		value: function deleteWebhook(_ref36) {
			var hookId = _ref36.hookId,
			    product = _ref36.product,
			    auth = _ref36.auth,
			    context = _ref36.context;

			var uri = product ? '/v1/products/' + product + '/webhooks/' + hookId : '/v1/webhooks/' + hookId;
			return this.delete(uri, undefined, auth, context);
		}

		/**
   * List all webhooks owned by the account or product
   * @param  {Object} options Options for this API call
   * @param  {String} [options.product]          Webhooks for this product ID or slug
   * @param  {String} options.auth Access Token
   * @return {Promise}
   */

	}, {
		key: 'listWebhooks',
		value: function listWebhooks(_ref37) {
			var product = _ref37.product,
			    auth = _ref37.auth,
			    context = _ref37.context;

			var uri = product ? '/v1/products/' + product + '/webhooks' : '/v1/webhooks';
			return this.get(uri, auth, undefined, context);
		}

		/**
   * Create an integration to send events to an external service
      *
   * See the API docs for details https://docs.particle.io/reference/api/#integrations-webhooks-
   *
   * @param  {Object} options Options for this API call
   * @param  {String} options.integrationType  The kind of external integration. One of Webhook, AzureIotHub, GoogleCloudPubSub, GoogleMaps
   * @param  {String} options.event            Event that triggers the integration
   * @params {Object} options.settings         Settings specific to that integration type
   * @param  {String} [options.deviceId]       Trigger integration only for this device ID or Name
   * @param  {String} [options.product]        Integration for this product ID or slug
   * @param  {String} options.auth             Access Token
   * @return {Promise}
   */

	}, {
		key: 'createIntegration',
		value: function createIntegration(_ref38) {
			var integrationType = _ref38.integrationType,
			    event = _ref38.event,
			    settings = _ref38.settings,
			    deviceId = _ref38.deviceId,
			    product = _ref38.product,
			    auth = _ref38.auth,
			    context = _ref38.context;

			var uri = product ? '/v1/products/' + product + '/integrations' : '/v1/integrations';
			var data = (0, _assign2.default)({ event: event, deviceid: deviceId }, settings);
			return this.post(uri, data, auth, context);
		}

		/**
   * Edit an integration to send events to an external service
   *
   * See the API docs for details https://docs.particle.io/reference/api/#integrations-webhooks-
   *
   * @param  {Object} options Options for this API call
   * @param  {String} options.integrationId    The integration to edit
   * @param  {String} [options.event]          Change the event that triggers the integration
   * @params {Object} [options.settings]       Change the settings specific to that integration type
   * @param  {String} [options.deviceId]       Trigger integration only for this device ID or Name
   * @param  {String} [options.product]        Integration for this product ID or slug
   * @param  {String} options.auth             Access Token
   * @return {Promise}
   */

	}, {
		key: 'editIntegration',
		value: function editIntegration(_ref39) {
			var integrationId = _ref39.integrationId,
			    event = _ref39.event,
			    settings = _ref39.settings,
			    deviceId = _ref39.deviceId,
			    product = _ref39.product,
			    auth = _ref39.auth,
			    context = _ref39.context;

			var uri = product ? '/v1/products/' + product + '/integrations/' + integrationId : '/v1/integrations/' + integrationId;
			var data = (0, _assign2.default)({ event: event, deviceid: deviceId }, settings);
			return this.put(uri, data, auth, context);
		}

		/**
   * Delete an integration to send events to an external service
   *
   * @param  {Object} options Options for this API call
   * @param  {String} options.integrationId    The integration to remove
   * @param  {String} [options.product]        Integration for this product ID or slug
   * @param  {String} options.auth             Access Token
   * @return {Promise}
   */

	}, {
		key: 'deleteIntegration',
		value: function deleteIntegration(_ref40) {
			var integrationId = _ref40.integrationId,
			    product = _ref40.product,
			    auth = _ref40.auth,
			    context = _ref40.context;

			var uri = product ? '/v1/products/' + product + '/integrations/' + integrationId : '/v1/integrations/' + integrationId;
			return this.delete(uri, undefined, auth, context);
		}

		/**
   * List all integrations owned by the account or product
   * @param  {Object} options Options for this API call
   * @param  {String} [options.product]        Integrations for this product ID or slug
   * @param  {String} options.auth Access Token
   * @return {Promise}
   */

	}, {
		key: 'listIntegrations',
		value: function listIntegrations(_ref41) {
			var product = _ref41.product,
			    auth = _ref41.auth,
			    context = _ref41.context;

			var uri = product ? '/v1/products/' + product + '/integrations' : '/v1/integrations';
			return this.get(uri, auth, undefined, context);
		}

		/**
   * Get details about the current user
   * @param  {Object} options Options for this API call
   * @param  {String} options.auth Access Token
   * @return {Promise}
   */

	}, {
		key: 'getUserInfo',
		value: function getUserInfo(_ref42) {
			var auth = _ref42.auth,
			    context = _ref42.context;

			return this.get('/v1/user', auth, undefined, context);
		}

		/**
   * Set details on the current user
   * @param  {Object} options Options for this API call
   * @param  {String} options.auth Access Token
   * @param  {String} options.stripeToken Set user's stripe token for payment
   * @param  {String} options.accountInfo Set user's extended info fields (name, business account, company name, etc)
   * @param  {String} options.password Change authenticated user password
   * @return {Promise}
   */

	}, {
		key: 'setUserInfo',
		value: function setUserInfo(_ref43) {
			var stripeToken = _ref43.stripeToken,
			    accountInfo = _ref43.accountInfo,
			    password = _ref43.password,
			    auth = _ref43.auth,
			    context = _ref43.context;

			var bodyObj = {};

			stripeToken ? bodyObj.stripe_token = stripeToken : null;
			accountInfo ? bodyObj.account_info = accountInfo : null;
			password ? bodyObj.password = password : null;

			return this.put('/v1/user', bodyObj, auth, context);
		}

		/**
   * List SIM cards owned by a user or product
   * @param  {Object} options Options for this API call
   * @param  {String} [options.iccid]    (Product only) Filter to SIM cards matching this ICCID
   * @param  {String} [options.deviceId] (Product only) Filter to SIM cards matching this device ID
   * @param  {String} [options.deviceName] (Product only) Filter to SIM cards matching this device name
   * @param  {Number} [options.page]     (Product only) Current page of results
   * @param  {Number} [options.perPage]  (Product only) Records per page
   * @param  {String} [options.product]  SIM cards for this product ID or slug
   * @param  {String} options.auth       Access Token
   * @return {Promise}
   */

	}, {
		key: 'listSIMs',
		value: function listSIMs(_ref44) {
			var iccid = _ref44.iccid,
			    deviceId = _ref44.deviceId,
			    deviceName = _ref44.deviceName,
			    page = _ref44.page,
			    perPage = _ref44.perPage,
			    product = _ref44.product,
			    auth = _ref44.auth,
			    context = _ref44.context;

			var uri = product ? '/v1/products/' + product + '/sims' : '/v1/sims';
			var query = product ? { iccid: iccid, deviceId: deviceId, deviceName: deviceName, page: page, per_page: perPage } : undefined;
			return this.get(uri, auth, query, context);
		}

		/**
   * Get data usage for one SIM card for the current billing period
   * @param  {Object} options Options for this API call
   * @param  {String} options.iccid      ICCID of the SIM card
   * @param  {String} [options.product]  SIM card for this product ID or slug
   * @param  {String} options.auth       Access Token
   * @return {Promise}
   */

	}, {
		key: 'getSIMDataUsage',
		value: function getSIMDataUsage(_ref45) {
			var iccid = _ref45.iccid,
			    product = _ref45.product,
			    auth = _ref45.auth,
			    context = _ref45.context;

			var uri = product ? '/v1/products/' + product + '/sims/' + iccid + '/data_usage' : '/v1/sims/' + iccid + '/data_usage';
			return this.get(uri, auth, undefined, context);
		}

		/**
   * Get data usage for all SIM cards in a product the current billing period
   * @param  {Object} options Options for this API call
   * @param  {String} options.product  SIM cards for this product ID or slug
   * @param  {String} options.auth     Access Token
   * @return {Promise}
   */

	}, {
		key: 'getFleetDataUsage',
		value: function getFleetDataUsage(_ref46) {
			var product = _ref46.product,
			    auth = _ref46.auth,
			    context = _ref46.context;

			return this.get('/v1/products/' + product + '/sims/data_usage', auth, undefined, context);
		}
	}, {
		key: 'checkSIM',
		value: function checkSIM(_ref47) {
			var iccid = _ref47.iccid,
			    auth = _ref47.auth,
			    context = _ref47.context;

			return this.head('/v1/sims/' + iccid, auth, undefined, context);
		}

		/**
   * Activate and add SIM cards to an account or product
   * @param  {Object} options Options for this API call
   * @param  {String} options.iccid        ICCID of the SIM card
   * @param  {Array<String>} options.iccids (Product only) ICCID of multiple SIM cards to import
   * @param  {String} options.countryCode The ISO country code for the SIM cards
   * @param  {String} [options.product]  SIM cards for this product ID or slug
   * @param  {String} options.auth       Access Token
   * @return {Promise}
   */

	}, {
		key: 'activateSIM',
		value: function activateSIM(_ref48) {
			var iccid = _ref48.iccid,
			    iccids = _ref48.iccids,
			    countryCode = _ref48.countryCode,
			    promoCode = _ref48.promoCode,
			    product = _ref48.product,
			    auth = _ref48.auth,
			    context = _ref48.context;

			// promoCode is deprecated
			iccids = iccids || [iccid];
			var uri = product ? '/v1/products/' + product + '/sims' : '/v1/sims/' + iccid;
			var data = product ? { sims: iccids, countryCode: countryCode } : { countryCode: countryCode, promoCode: promoCode, action: 'activate' };
			var method = product ? 'post' : 'put';

			return this.request({ uri: uri, method: method, data: data, auth: auth, context: context });
		}

		/**
   * Deactivate a SIM card so it doesn't incur data usage in future months.
   * @param  {Object} options Options for this API call
   * @param  {String} options.iccid      ICCID of the SIM card
   * @param  {String} [options.product]  SIM cards for this product ID or slug
   * @param  {String} options.auth       Access Token
   * @return {Promise}
   */

	}, {
		key: 'deactivateSIM',
		value: function deactivateSIM(_ref49) {
			var iccid = _ref49.iccid,
			    product = _ref49.product,
			    auth = _ref49.auth,
			    context = _ref49.context;

			var uri = product ? '/v1/products/' + product + '/sims/' + iccid : '/v1/sims/' + iccid;
			var data = { action: 'deactivate' };
			return this.put(uri, data, auth, context);
		}

		/**
   * Reactivate a SIM card the was deactivated or unpause a SIM card that was automatically paused
   * @param  {Object} options Options for this API call
   * @param  {String} options.iccid      ICCID of the SIM card
   * @param  {Number} [options.mbLimit]  New monthly data limit. Necessary if unpausing a SIM card
   * @param  {String} [options.product]  SIM cards for this product ID or slug
   * @param  {String} options.auth       Access Token
   * @return {Promise}
   */

	}, {
		key: 'reactivateSIM',
		value: function reactivateSIM(_ref50) {
			var iccid = _ref50.iccid,
			    mbLimit = _ref50.mbLimit,
			    product = _ref50.product,
			    auth = _ref50.auth,
			    context = _ref50.context;

			var uri = product ? '/v1/products/' + product + '/sims/' + iccid : '/v1/sims/' + iccid;
			var data = { mb_limit: mbLimit, action: 'reactivate' };
			return this.put(uri, data, auth, context);
		}

		/**
   * Update SIM card data limit
   * @param  {Object} options Options for this API call
   * @param  {String} options.iccid        ICCID of the SIM card
   * @param  {Array}  options.mbLimit     Data limit in megabyte for the SIM card
   * @param  {String} [options.product]  SIM cards for this product ID or slug
   * @param  {String} options.auth       Access Token
   * @return {Promise}
   */

	}, {
		key: 'updateSIM',
		value: function updateSIM(_ref51) {
			var iccid = _ref51.iccid,
			    mbLimit = _ref51.mbLimit,
			    product = _ref51.product,
			    auth = _ref51.auth,
			    context = _ref51.context;

			var uri = product ? '/v1/products/' + product + '/sims/' + iccid : '/v1/sims/' + iccid;
			var data = { mb_limit: mbLimit };
			return this.put(uri, data, auth, context);
		}

		/**
   * Remove a SIM card from an account so it can be activated by a different account
   * @param  {Object} options Options for this API call
   * @param  {String} options.iccid      ICCID of the SIM card
   * @param  {String} [options.product]  SIM cards for this product ID or slug
   * @param  {String} options.auth       Access Token
   * @return {Promise}
   */

	}, {
		key: 'removeSIM',
		value: function removeSIM(_ref52) {
			var iccid = _ref52.iccid,
			    product = _ref52.product,
			    auth = _ref52.auth,
			    context = _ref52.context;

			var uri = product ? '/v1/products/' + product + '/sims/' + iccid : '/v1/sims/' + iccid;
			return this.delete(uri, undefined, auth, context);
		}

		/**
   * List valid build targets to be used for compiling
   * @param  {Object} options Options for this API call
   * @param  {Boolean} [options.onlyFeatured=false] Only list featured build targets
   * @param  {String} options.auth       Access Token
   * @return {Promise}
   */

	}, {
		key: 'listBuildTargets',
		value: function listBuildTargets(_ref53) {
			var onlyFeatured = _ref53.onlyFeatured,
			    auth = _ref53.auth,
			    context = _ref53.context;

			var query = void 0;
			if (onlyFeatured !== undefined) {
				query = { featured: !!onlyFeatured };
			}
			return this.get('/v1/build_targets', auth, query, context);
		}

		/**
   * List firmware libraries
   * @param  {Object} options Options for this API call
   * @param  {Number} options.page Page index (default, first page)
   * @param  {Number} options.limit Number of items per page
   * @param  {String} options.filter Search term for the libraries
   * @param  {String} options.sort Ordering key for the library list
   * @param  {Array<String>}  options.architectures List of architectures to filter
   * @param  {String} options.category Category to filter
   * @param  {String} options.scope The library scope to list. Default is 'all'. Other values are
   * - 'all' - list public libraries and my private libraries
   * - 'public' - list only public libraries
   * - 'private' - list only my private libraries
   * - 'mine' - list my libraries (public and private)
   * - 'official' - list only official libraries
   * - 'verified' - list only verified libraries
   * - 'featured' - list only featured libraries
   * @param  {String} options.excludeScopes  list of scopes to exclude
   * @param  {String} options.category Category to filter
   * @param  {String} options.auth Access Token
   * @return {Promise}
   */

	}, {
		key: 'listLibraries',
		value: function listLibraries(_ref54) {
			var page = _ref54.page,
			    limit = _ref54.limit,
			    filter = _ref54.filter,
			    sort = _ref54.sort,
			    architectures = _ref54.architectures,
			    category = _ref54.category,
			    scope = _ref54.scope,
			    excludeScopes = _ref54.excludeScopes,
			    auth = _ref54.auth,
			    context = _ref54.context;

			return this.get('/v1/libraries', auth, {
				page: page,
				filter: filter,
				limit: limit,
				sort: sort,
				architectures: this._asList(architectures),
				category: category,
				scope: scope,
				excludeScopes: this._asList(excludeScopes)
			}, context);
		}
	}, {
		key: '_asList',
		value: function _asList(value) {
			return Array.isArray(value) ? value.join(',') : value;
		}

		/**
   * Get firmware library details
   * @param  {Object} options Options for this API call
   * @param  {String} options.name Name of the library to fetch
   * @param  {String} options.version Version of the library to fetch (default: latest)
   * @param  {String} options.auth Access Token
   * @return {Promise}
   */

	}, {
		key: 'getLibrary',
		value: function getLibrary(_ref55) {
			var name = _ref55.name,
			    version = _ref55.version,
			    auth = _ref55.auth,
			    context = _ref55.context;

			return this.get('/v1/libraries/' + name, auth, { version: version }, context);
		}

		/**
   * Firmware library details for each version
   * @param  {Object} options Options for this API call
   * @param  {String} options.name Name of the library to fetch
   * @param  {Number} options.page Page index (default, first page)
   * @param  {Number} options.limit Number of items per page
   * @param  {String} options.auth Access Token
   * @return {Promise}
   */

	}, {
		key: 'getLibraryVersions',
		value: function getLibraryVersions(_ref56) {
			var name = _ref56.name,
			    page = _ref56.page,
			    limit = _ref56.limit,
			    auth = _ref56.auth,
			    context = _ref56.context;

			return this.get('/v1/libraries/' + name + '/versions', auth, {
				page: page,
				limit: limit
			}, context);
		}

		/**
   * Contribute a new library version from a compressed archive
   * @param  {Object} options Options for this API call
   * @param  {String} options.archive Compressed archive file containing the library sources
   * @param  {String} options.auth Access Token
   * @return {Promise}
   */

	}, {
		key: 'contributeLibrary',
		value: function contributeLibrary(_ref57) {
			var archive = _ref57.archive,
			    auth = _ref57.auth,
			    context = _ref57.context;

			var files = {
				'archive.tar.gz': archive
			};

			return this.request({ uri: '/v1/libraries',
				files: files, auth: auth, context: context, method: 'post' });
		}

		/**
   * Publish the latest version of a library to the public
   * @param  {Object} options Options for this API call
   * @param  {String} options.name Name of the library to publish
   * @param  {String} options.auth Access Token
   * @return {Promise}
   */

	}, {
		key: 'publishLibrary',
		value: function publishLibrary(_ref58) {
			var name = _ref58.name,
			    auth = _ref58.auth,
			    context = _ref58.context;

			return this.request({ uri: '/v1/libraries/' + name,
				auth: auth, context: context, method: 'patch', data: { visibility: 'public' } });
		}

		/**
   * Delete one version of a library or an entire private library
   * @param  {Object} options Options for this API call
   * @param  {String} options.name Name of the library to remove
   * @param  {String} options.force Key to force deleting a public library
   * @param  {String} options.auth Access Token
   * @return {Promise}
   */

	}, {
		key: 'deleteLibrary',
		value: function deleteLibrary(_ref59) {
			var name = _ref59.name,
			    force = _ref59.force,
			    auth = _ref59.auth,
			    context = _ref59.context;

			return this.delete('/v1/libraries/' + name, { force: force }, auth, context);
		}

		/**
   * Download an external file that may not be on the API
   * @param  {Object} options Options for this API call
   * @param  {String} options.url URL of the file.
   * @return {Promise} Resolves to a buffer with the file data
   */

	}, {
		key: 'downloadFile',
		value: function downloadFile(_ref60) {
			var url = _ref60.url;

			var req = _superagent2.default.get(url);
			if (req.buffer) {
				req = req.buffer(true).parse(_superagentBinaryParser2.default);
			} else if (req.responseType) {
				req = req.responseType('arraybuffer').then(function (res) {
					res.body = res.xhr.response;
					return res;
				});
			}
			return req.then(function (res) {
				return res.body;
			});
		}

		/**
   * List OAuth client created by the account
   * @param  {Object} options Options for this API call
   * @param  {String} [options.product] List clients for this product ID or slug
   * @param  {String} options.auth Access Token
   * @return {Promise}
   */

	}, {
		key: 'listOAuthClients',
		value: function listOAuthClients(_ref61) {
			var product = _ref61.product,
			    auth = _ref61.auth,
			    context = _ref61.context;

			var uri = product ? '/v1/products/' + product + '/clients' : '/v1/clients';
			return this.get(uri, auth, undefined, context);
		}

		/**
   * Create an OAuth client
   * @param  {Object} options Options for this API call
   * @param  {String} options.name               Name of the OAuth client
   * @param  {String} options.type               web, installed or web
   * @param  {String} [options.redirect_uri]     URL to redirect after OAuth flow. Only for type web.
   * @param  {Object} [options.scope]            Limits what the access tokens created by this client can do.
   * @param  {String} [options.product]          Create client for this product ID or slug
   * @param  {String} options.auth               Access Token
   * @return {Promise}
   */

	}, {
		key: 'createOAuthClient',
		value: function createOAuthClient(_ref62) {
			var name = _ref62.name,
			    type = _ref62.type,
			    redirect_uri = _ref62.redirect_uri,
			    scope = _ref62.scope,
			    product = _ref62.product,
			    auth = _ref62.auth,
			    context = _ref62.context;

			var uri = product ? '/v1/products/' + product + '/clients' : '/v1/clients';
			var data = { name: name, type: type, redirect_uri: redirect_uri, scope: scope };
			return this.post(uri, data, auth, context);
		}

		/**
   * Update an OAuth client
   * @param  {Object} options Options for this API call
   * @param  {String} options.clientId           The OAuth client to update
   * @param  {String} [options.name]             New Name of the OAuth client
   * @param  {Object} [options.scope]            New scope of the OAuth client
   * @param  {String} [options.product]          Update client linked to this product ID or slug
   * @param  {String} options.auth               Access Token
   * @return {Promise}
   */

	}, {
		key: 'updateOAuthClient',
		value: function updateOAuthClient(_ref63) {
			var clientId = _ref63.clientId,
			    name = _ref63.name,
			    scope = _ref63.scope,
			    product = _ref63.product,
			    auth = _ref63.auth,
			    context = _ref63.context;

			var uri = product ? '/v1/products/' + product + '/clients/' + clientId : '/v1/clients/' + clientId;
			var data = { name: name, scope: scope };
			return this.put(uri, data, auth, context);
		}

		/**
   * Delete an OAuth client
   * @param  {Object} options Options for this API call
   * @param  {String} options.clientId           The OAuth client to update
   * @param  {String} [options.product]          OAuth client linked to this product ID or slug
   * @param  {String} options.auth               Access Token
   * @return {Promise}
   */

	}, {
		key: 'deleteOAuthClient',
		value: function deleteOAuthClient(_ref64) {
			var clientId = _ref64.clientId,
			    product = _ref64.product,
			    auth = _ref64.auth,
			    context = _ref64.context;

			var uri = product ? '/v1/products/' + product + '/clients/' + clientId : '/v1/clients/' + clientId;
			return this.delete(uri, undefined, auth, context);
		}

		/**
   * List products the account has access to
   * @param  {Object} options Options for this API call
   * @param  {String} options.auth Access Token
   * @return {Promise}
   */

	}, {
		key: 'listProducts',
		value: function listProducts(_ref65) {
			var auth = _ref65.auth,
			    context = _ref65.context;

			return this.get('/v1/products', auth, undefined, context);
		}

		/**
   * Get detailed information about a product
   * @param  {Object} options Options for this API call
   * @param  {String} options.product  Product ID or slug
   * @param  {String} options.auth     Access token
   * @return {Promise}
   */

	}, {
		key: 'getProduct',
		value: function getProduct(_ref66) {
			var product = _ref66.product,
			    auth = _ref66.auth,
			    context = _ref66.context;

			return this.get('/v1/products/' + product, auth, undefined, context);
		}

		/**
   * List product firmware versions
   * @param  {Object} options Options for this API call
   * @param  {String} options.product Firmware for this product ID or slug
   * @param  {String} options.auth Access Token
   * @return {Promise}
   */

	}, {
		key: 'listProductFirmware',
		value: function listProductFirmware(_ref67) {
			var product = _ref67.product,
			    auth = _ref67.auth,
			    context = _ref67.context;

			return this.get('/v1/products/' + product + '/firmware', auth, undefined, context);
		}

		/**
   * List product firmware versions
   * @param  {Object} options Options for this API call
   * @param  {Object} options.file    Path or Buffer of the new firmware file
   * @param  {Number} options.version Version number of new firmware
   * @param  {String} options.title   Short identifier for the new firmware
   * @param  {String} [options.description] Longer description for the new firmware
   * @param  {String} options.product Firmware for this product ID or slug
   * @param  {String} options.auth Access Token
   * @return {Promise}
   */

	}, {
		key: 'uploadProductFirmware',
		value: function uploadProductFirmware(_ref68) {
			var file = _ref68.file,
			    version = _ref68.version,
			    title = _ref68.title,
			    description = _ref68.description,
			    product = _ref68.product,
			    auth = _ref68.auth,
			    context = _ref68.context;

			return this.request({
				uri: '/v1/products/' + product + '/firmware',
				method: 'post',
				files: {
					'firmware.bin': file
				},
				form: {
					version: version,
					title: title,
					description: description
				},
				context: context,
				auth: auth
			});
		}

		/**
   * Get information about a product firmware version
   * @param  {Object} options Options for this API call
   * @param  {Number} options.version Version number of firmware
   * @param  {String} options.product Firmware for this product ID or slug
   * @param  {String} options.auth    Access token
   * @return {Promise}
   */

	}, {
		key: 'getProductFirmware',
		value: function getProductFirmware(_ref69) {
			var version = _ref69.version,
			    product = _ref69.product,
			    auth = _ref69.auth,
			    context = _ref69.context;

			return this.get('/v1/products/' + product + '/firmware/' + version, auth, undefined, context);
		}

		/**
   * Update information for a product firmware version
   * @param  {Object} options Options for this API call
   * @param  {Number} options.version Version number of new firmware
   * @param  {String} [options.title]   New title
   * @param  {String} [options.description] New description
   * @param  {String} options.product Firmware for this product ID or slug
   * @param  {String} options.auth Access Token
   * @return {Promise}
   */

	}, {
		key: 'updateProductFirmware',
		value: function updateProductFirmware(_ref70) {
			var version = _ref70.version,
			    title = _ref70.title,
			    description = _ref70.description,
			    product = _ref70.product,
			    auth = _ref70.auth,
			    context = _ref70.context;

			var uri = '/v1/products/' + product + '/firmware/' + version;
			return this.put(uri, { title: title, description: description }, auth, context);
		}

		/**
   * Download a product firmware binary
   * @param  {Object} options Options for this API call
   * @param  {Number} options.version Version number of new firmware
   * @param  {String} options.product Firmware for this product ID or slug
   * @param  {String} options.auth    Access Token
   * @return {Request}
   */

	}, {
		key: 'downloadProductFirmware',
		value: function downloadProductFirmware(_ref71) {
			var version = _ref71.version,
			    product = _ref71.product,
			    auth = _ref71.auth,
			    context = _ref71.context;

			var uri = '/v1/products/' + product + '/firmware/' + version + '/binary';
			var req = (0, _superagent2.default)('get', uri);
			req.use(this.prefix);
			this.headers(req, auth);
			if (this.debug) {
				this.debug(req);
			}
			return req;
		}

		/**
   * Release a product firmware version as the default version
   * @param  {Object} options Options for this API call
   * @param  {Number} options.version Version number of new firmware
   * @param  {String} options.product Firmware for this product ID or slug
   * @param  {String} options.auth Access Token
   * @return {Promise}
   */

	}, {
		key: 'releaseProductFirmware',
		value: function releaseProductFirmware(_ref72) {
			var version = _ref72.version,
			    product = _ref72.product,
			    auth = _ref72.auth,
			    context = _ref72.context;

			var uri = '/v1/products/' + product + '/firmware/release';
			return this.put(uri, { version: version }, auth, context);
		}

		/**
   * List product team members
   * @param  {Object} options Options for this API call
   * @param  {String} options.product Team for this product ID or slug
   * @param  {String} options.auth Access Token
   * @return {Promise}
   */

	}, {
		key: 'listTeamMembers',
		value: function listTeamMembers(_ref73) {
			var product = _ref73.product,
			    auth = _ref73.auth,
			    context = _ref73.context;

			return this.get('/v1/products/' + product + '/team', auth, undefined, context);
		}

		/**
   * Invite Particle user to a product team
   * @param  {Object} options Options for this API call
   * @param  {String} options.username  Username for the Particle account
   * @param  {String} options.product Team for this product ID or slug
   * @param  {String} options.auth Access Token
   * @return {Promise}
   */

	}, {
		key: 'inviteTeamMember',
		value: function inviteTeamMember(_ref74) {
			var username = _ref74.username,
			    product = _ref74.product,
			    auth = _ref74.auth,
			    context = _ref74.context;

			return this.post('/v1/products/' + product + '/team', { username: username }, auth, context);
		}

		/**
   * Remove Particle user to a product team
   * @param  {Object} options Options for this API call
   * @param  {String} options.username  Username for the Particle account
   * @param  {String} options.product Team for this product ID or slug
   * @param  {String} options.auth Access Token
   * @return {Promise}
   */

	}, {
		key: 'removeTeamMember',
		value: function removeTeamMember(_ref75) {
			var username = _ref75.username,
			    product = _ref75.product,
			    auth = _ref75.auth,
			    context = _ref75.context;

			return this.delete('/v1/products/' + product + '/team/' + username, undefined, auth, context);
		}

		/**
   * API URI to access a device
   * @param  {Object} options Options for this API call
   * @param  {String} options.deviceId  Device ID to access
   * @param  {String} [options.product] Device only in this product ID or slug
   * @private
   * @returns {string}
   */

	}, {
		key: 'deviceUri',
		value: function deviceUri(_ref76) {
			var deviceId = _ref76.deviceId,
			    product = _ref76.product;

			return product ? '/v1/products/' + product + '/devices/' + deviceId : '/v1/devices/' + deviceId;
		}
	}, {
		key: 'get',
		value: function get(uri, auth, query, context) {
			context = this._buildContext(context);
			return this.agent.get(uri, auth, query, context);
		}
	}, {
		key: 'head',
		value: function head(uri, auth, query, context) {
			context = this._buildContext(context);
			return this.agent.head(uri, auth, query, context);
		}
	}, {
		key: 'post',
		value: function post(uri, data, auth, context) {
			context = this._buildContext(context);
			return this.agent.post(uri, data, auth, context);
		}
	}, {
		key: 'put',
		value: function put(uri, data, auth, context) {
			context = this._buildContext(context);
			return this.agent.put(uri, data, auth, context);
		}
	}, {
		key: 'delete',
		value: function _delete(uri, data, auth, context) {
			context = this._buildContext(context);
			return this.agent.delete(uri, data, auth, context);
		}
	}, {
		key: 'request',
		value: function request(args) {
			args.context = this._buildContext(args.context);
			return this.agent.request(args);
		}
	}, {
		key: 'client',
		value: function client() {
			var options = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : {};

			return new _Client2.default((0, _assign2.default)({ api: this }, options));
		}
	}]);
	return Particle;
}();

// Aliases for backwards compatibility


Particle.prototype.removeAccessToken = Particle.prototype.deleteAccessToken;

exports.default = Particle;
module.exports = exports['default'];
//# sourceMappingURL=Particle.js.map