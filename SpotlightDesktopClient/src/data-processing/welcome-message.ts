export class WelcomeMessage {
	constructor(public message: string) {}

	static from(rawDataEntries: string[]): WelcomeMessage {
		return new WelcomeMessage(rawDataEntries[0]);
	}
}
