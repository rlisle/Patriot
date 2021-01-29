/**
 * Static manager of environment level SDK user agent information.
 */
export declare class UserAgentManager {
    private static components;
    private static userAgent;
    /**
     * Retrieves the full user agent string, containing all registered components.
     */
    static getUserAgent(): string;
    /**
     * Registers a user agent component. This will be appended to the generated
     * user agent string. Duplicate components will be ignored.
     *
     * @param component string component to add to the full user agent
     */
    static registerComponent(component: string): void;
}
