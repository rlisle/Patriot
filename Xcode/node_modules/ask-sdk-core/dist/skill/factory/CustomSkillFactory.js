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
exports.CustomSkillFactory = void 0;
const BaseSkillFactory_1 = require("./BaseSkillFactory");
/**
 * Provider for {@link CustomSkillBuilder}
 */
class CustomSkillFactory {
    static init() {
        let thisPersistenceAdapter;
        let thisApiClient;
        const baseSkillBuilder = BaseSkillFactory_1.BaseSkillFactory.init();
        return Object.assign(Object.assign({}, baseSkillBuilder), { getSkillConfiguration() {
                const skillConfiguration = baseSkillBuilder.getSkillConfiguration();
                return Object.assign(Object.assign({}, skillConfiguration), { persistenceAdapter: thisPersistenceAdapter, apiClient: thisApiClient });
            },
            withPersistenceAdapter(persistenceAdapter) {
                thisPersistenceAdapter = persistenceAdapter;
                return this;
            },
            withApiClient(apiClient) {
                thisApiClient = apiClient;
                return this;
            } });
    }
    constructor() { }
}
exports.CustomSkillFactory = CustomSkillFactory;
//# sourceMappingURL=CustomSkillFactory.js.map