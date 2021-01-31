var constants = {

    NAMESPACE_ALEXA            : "Alexa",
    NAMESPACE_POWER_CONTROLLER : "Alexa.PowerController",
    NAMESPACE_POWER_LEVEL      : "Alexa.PowerLevelController",
    NAMESPACE_DISCOVERY        : "Alexa.Discovery",
    NAMESPACE_SCENE_CONTROLLER : "Alexa.SceneController",
    PAYLOAD_VERSION            : 3,
    RESPONSE_DISCOVER          : "Discover.Response",
    REQUEST_ACTIVATE           : "Activate",
    REQUEST_DEACTIVATE         : "Deactivate",
    REQUEST_TURN_ON            : "TurnOn",
    REQUEST_TURN_OFF           : "TurnOff",
    RESPONSE_ACTIVATE          : "ActivationStarted",
    RESPONSE_DEACTIVATE        : "DeactivationStarted",

    // Discovered Appliances Actions
    INCREMENT_ACTION      : "incrementPercentage",
    DECREMENT_ACTION      : "decrementPercentage",
    SET_PERCENTAGE_ACTION : "setPercentage",
    TURN_ON_ACTION        : "turnOn",
    TURN_OFF_ACTION       : "turnOff",

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

    REQUEST_HEALTH_CHECK  : "HealthCheckRequest",
    RESPONSE_HEALTH_CHECK : "HealthCheckResponse",

    ERROR_VALUE_OUT_OF_RANGE : "ValueOutOfRangeError",
    ERROR_TARGET_OFFLINE     : "TargetOfflineError",
    ERROR_BRIDGE_OFFLINE     : "BridgeOfflineError",
    ERROR_NO_SUCH_TARGET     : "NoSuchTargetError",
    ERROR_DRIVER_INTERNAL    : "DriverInternalError",
    ERROR_DEPENDENT_SERVICE  : "DependentServiceUnavailableError",
    ERROR_TARGET_FW_OUTDATED : "TargetFirmwareOutdatedError",
    ERROR_BRIDGE_FW_OUTDATED : "TargetBridgeFirmwareOutdatedError",
    ERROR_TARGET_HW_MALFUNCTION : "TargetHardwareMalfunctionError",
    ERROR_BRIDGE_HW_MALFUNCTION : "TargetBridgeHardwareMalfunctionError",
    ERROR_UNWILLING_TO_SET_VALUE : "UnwillingToSetValueError",
    ERROR_RATE_LIMIT_EXCEEDED : "RateLimitExceededError",
    ERROR_NOT_SUPPORT_IN_MODE : "NotSupportedInCurrentModeError",
    ERROR_EXPIRED_TOKEN      : "ExpiredAccessTokenError",
    ERROR_INVALID_TOKEN      : "InvalidAccessTokenError",
    ERROR_UNSUPPORTED_TARGET : "UnsupportedTargetError",
    ERROR_UNSUPPORTED_OPERATION : "UnsupportedOperationError",
    ERROR_UNSUPPORTED_TARGET_SETTING : "UnsupportedTargetSettingError",
    ERROR_UNEXPECTED_INFO    : "UnexpectedInformationReceivedError"

};

module.exports = constants;
