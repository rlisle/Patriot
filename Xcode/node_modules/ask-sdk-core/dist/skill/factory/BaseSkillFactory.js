"use strict";
/*
 * Copyright 2018 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */
Object.defineProperty(exports, "__esModule", { value: true });
exports.BaseSkillFactory = void 0;
const ask_sdk_runtime_1 = require("ask-sdk-runtime");
const CustomSkill_1 = require("../CustomSkill");
class BaseSkillFactory {
    static init() {
        const runtimeConfigurationBuilder = new ask_sdk_runtime_1.RuntimeConfigurationBuilder();
        let thisCustomUserAgent;
        let thisSkillId;
        return {
            addRequestHandler(matcher, executor) {
                const canHandle = typeof matcher === 'string'
                    ? ({ requestEnvelope }) => matcher === (requestEnvelope.request.type === 'IntentRequest'
                        ? requestEnvelope.request.intent.name
                        : requestEnvelope.request.type)
                    : matcher;
                runtimeConfigurationBuilder.addRequestHandler(canHandle, executor);
                return this;
            },
            addRequestHandlers(...requestHandlers) {
                runtimeConfigurationBuilder.addRequestHandlers(...requestHandlers);
                return this;
            },
            addRequestInterceptors(...executors) {
                runtimeConfigurationBuilder.addRequestInterceptors(...executors);
                return this;
            },
            addResponseInterceptors(...executors) {
                runtimeConfigurationBuilder.addResponseInterceptors(...executors);
                return this;
            },
            addErrorHandler(matcher, executor) {
                runtimeConfigurationBuilder.addErrorHandler(matcher, executor);
                return this;
            },
            addErrorHandlers(...errorHandlers) {
                runtimeConfigurationBuilder.addErrorHandlers(...errorHandlers);
                return this;
            },
            withCustomUserAgent(customUserAgent) {
                thisCustomUserAgent = customUserAgent;
                return this;
            },
            withSkillId(skillId) {
                thisSkillId = skillId;
                return this;
            },
            getSkillConfiguration() {
                const runtimeConfiguration = runtimeConfigurationBuilder.getRuntimeConfiguration();
                return Object.assign(Object.assign({}, runtimeConfiguration), { customUserAgent: thisCustomUserAgent, skillId: thisSkillId });
            },
            create() {
                return new CustomSkill_1.CustomSkill(this.getSkillConfiguration());
            },
            lambda() {
                const skill = new CustomSkill_1.CustomSkill(this.getSkillConfiguration());
                return (event, context, callback) => {
                    skill.invoke(event, context)
                        .then((response) => {
                        callback(null, response);
                    })
                        .catch((err) => {
                        callback(err, null);
                    });
                };
            },
        };
    }
    constructor() { }
}
exports.BaseSkillFactory = BaseSkillFactory;
//# sourceMappingURL=BaseSkillFactory.js.map