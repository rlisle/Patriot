exports.good = {

  /**
   * This is Ron's particle.io account information
   * Do NOT share or publish this.
   */
    accessToken          : process.env.PARTICLE_ACCESS_TOKEN,

    NAMESPACE_CONTROL    : "Alexa.ConnectedHome.Control",
    NAMESPACE_DISCOVERY  : "Alexa.ConnectedHome.Discovery",
    RESPONSE_DISCOVER    : "DiscoverAppliancesResponse",

    PAYLOAD_VERSION      : 2,

    // Discovered Appliances Actions
    INCREMENT_ACTION     : "incrementPercentage",
    DECREMENT_ACTION     : "decrementPercentage",
    SET_PERCENTAGE_ACTION : "setPercentage",
    TURN_ON_ACTION       : "turnOn",
    TURN_OFF_ACTION      : "turnOff",

    REQUEST_TURN_ON      : "TurnOnRequest",
    RESPONSE_TURN_ON     : "TurnOnConfirmation",
    REQUEST_TURN_OFF     : "TurnOffRequest",
    RESPONSE_TURN_OFF    : "TurnOffConfirmation",

    REQUEST_SET_TARGET   : "SetTargetTempuratureRequest",
    RESPONSE_SET_TARGET  : "SetTargetTemperatureConfirmation",
    REQUEST_INC_TARGET   : "IncrementTargetTempuratureRequest",
    RESPONSE_INC_TARGET  : "IncrementTargetTemperatureConfirmation",
    REQUEST_DEC_TARGET   : "DecrementTargetTempuratureRequest",
    RESPONSE_DEC_TARGET  : "DecrementTargetTemperatureConfirmation",

    REQUEST_SET_PERCENT  : "SetPercentageRequest",
    RESPONSE_SET_PERCENT : "SetPercentageConfirmation",
    REQUEST_INC_PERCENT  : "IncrementPercentageRequest",
    RESPONSE_INC_PERCENT : "IncrementPercentageConfirmation",
    REQUEST_DEC_PERCENT  : "DecrementPercentageRequest",
    RESPONSE_DEC_PERCENT : "DecrementPercentageConfirmation",

    REQUEST_HEALTH_CHECK : "HealthCheckRequest",
    RESPONSE_HEALTH_CHECK : "HealthCheckResponse",

    ERROR_VALUE_OUT_OF_RANGE : "ValueOutOfRangeError",
    ERROR_TARGET_OFFLINE   : "TargetOfflineError",
    ERROR_BRIDGE_OFFLINE   : "BridgeOfflineError",
    ERROR_NO_SUCH_TARGET   : "NoSuchTargetError",
    ERROR_DRIVER_INTERNAL  : "DriverInternalError",
    ERROR_DEPENDENT_SERVICE : "DependentServiceUnavailableError",
    ERROR_TARGET_FW_OUTDATED : "TargetFirmwareOutdatedError",
    ERROR_BRIDGE_FW_OUTDATED : "TargetBridgeFirmwareOutdatedError",
    ERROR_TARGET_HW_MALFUNCTION : "TargetHardwareMalfunctionError",
    ERROR_BRIDGE_HW_MALFUNCTION : "TargetBridgeHardwareMalfunctionError",
    ERROR_UNWILLING_TO_SET_VALUE : "UnwillingToSetValueError",
    ERROR_RATE_LIMIT_EXCEEDED : "RateLimitExceededError",
    ERROR_NOT_SUPPORT_IN_MODE : "NotSupportedInCurrentModeError",
    ERROR_EXPIRED_TOKEN    : "ExpiredAccessTokenError",
    ERROR_INVALID_TOKEN    : "InvalidAccessTokenError",
    ERROR_UNSUPPORTED_TARGET : "UnsupportedTargetError",
    ERROR_UNSUPPORTED_OPERATION : "UnsupportedOperationError",
    ERROR_UNSUPPORTED_TARGET_SETTING : "UnsupportedTargetSettingError",
    ERROR_UNEXPECTED_INFO  : "UnexpectedInformationReceivedError",

    REMOTE_CLOUD_BASE_PATH : '/v1/devices/events',
    REMOTE_CLOUD_HOSTNAME  : 'api.particle.io',
    PUBLISHENDPOINT        : 'https://api.particle.io/v1/devices/events'

};

exports.badAccessToken = {
    accessToken            : '1234567890abcdef1234567890abcdef12345678',
    REMOTE_CLOUD_BASE_PATH : '/v1/devices/events',
    REMOTE_CLOUD_HOSTNAME  : 'api.particle.io',
    PUBLISHENDPOINT        : 'https://api.particle.io/v1/devices/events'
};

exports.badURL = {
  accessToken            : process.env.PARTICLE_ACCESS_TOKEN,
  REMOTE_CLOUD_BASE_PATH : '/v1/devices/events',
  REMOTE_CLOUD_HOSTNAME  : 'bogus.bad',
  PUBLISHENDPOINT        : 'https://api.bogus.bad/v1/devices/events'
};
