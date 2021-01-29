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
exports.RichTextContentHelper = void 0;
const TextContentHelper_1 = require("./TextContentHelper");
/**
 * Responsible for building rich text content object using ask-sdk-model in Alexa skills kit display interface
 * https://developer.amazon.com/docs/custom-skills/display-interface-reference.html#textcontent-object-specifications.
 */
class RichTextContentHelper extends TextContentHelper_1.TextContentHelper {
    constructor() {
        super();
    }
    /**
     * @returns {interfaces.display.TextContent}
     */
    getTextContent() {
        const textContent = {};
        if (this.primaryText) {
            textContent.primaryText = {
                type: 'RichText',
                text: this.primaryText,
            };
        }
        if (this.secondaryText) {
            textContent.secondaryText = {
                type: 'RichText',
                text: this.secondaryText,
            };
        }
        if (this.tertiaryText) {
            textContent.tertiaryText = {
                type: 'RichText',
                text: this.tertiaryText,
            };
        }
        return textContent;
    }
}
exports.RichTextContentHelper = RichTextContentHelper;
//# sourceMappingURL=RichTextContentHelper.js.map