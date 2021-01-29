"use strict";
/*
 * Copyright 2020 Amazon.com, Inc. or its affiliates. All Rights Reserved.
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
exports.UserAgentManager = void 0;
/**
 * Static manager of environment level SDK user agent information.
 */
class UserAgentManager {
    /**
     * Retrieves the full user agent string, containing all registered components.
     */
    static getUserAgent() {
        return this.userAgent;
    }
    /**
     * Registers a user agent component. This will be appended to the generated
     * user agent string. Duplicate components will be ignored.
     *
     * @param component string component to add to the full user agent
     */
    static registerComponent(component) {
        if (!this.components.has(component)) {
            this.components.add(component);
            let updatedUserAgent;
            for (const component of this.components) {
                updatedUserAgent = updatedUserAgent ? `${updatedUserAgent} ${component}` : component;
            }
            this.userAgent = updatedUserAgent;
        }
    }
}
exports.UserAgentManager = UserAgentManager;
UserAgentManager.components = new Set();
UserAgentManager.userAgent = '';
//# sourceMappingURL=UserAgentManager.js.map