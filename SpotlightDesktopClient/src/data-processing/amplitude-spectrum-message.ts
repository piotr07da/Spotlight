export class AmplitudeSpectrumMessage {
	private constructor(public spectrum: number[]) {}

	static from(rawDataEntries: string[]): AmplitudeSpectrumMessage {
		return new AmplitudeSpectrumMessage(rawDataEntries.map((v) => parseFloat(v)));
	}
}
