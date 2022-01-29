export class SamplesBatchMessage {
	constructor(public samples: number[]) {}

	static from(rawDataEntries: string[]): SamplesBatchMessage {
		return new SamplesBatchMessage(rawDataEntries.map((v) => parseFloat(v)));
	}
}
