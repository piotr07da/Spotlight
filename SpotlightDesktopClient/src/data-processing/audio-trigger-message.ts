export class AudioTriggerMessage {
	private constructor(
		public bandMinFrequencyValueIndex: number = 0,
		public bandMaxFrequencyValueIndex: number = 0,
		public bandsSpectrumAverage: number = 0,
		public oldestWholeAverage: number = 0,
		public oldestBandsAverage: number = 0,
		public amplitudeSpectrum: number[] = []
	) {}

	static from(rawDataEntries: string[]): AudioTriggerMessage {
		const m = new AudioTriggerMessage();

		for (let i = 0; i < rawDataEntries.length; ++i) {
			if (rawDataEntries[i] == "bandSelector") {
				m.bandMinFrequencyValueIndex = parseInt(rawDataEntries[i + 1]);
				m.bandMaxFrequencyValueIndex = parseInt(rawDataEntries[i + 2]);
				i += 2;
			}
			if (rawDataEntries[i] == "bandsSpectrumAverage") {
				m.bandsSpectrumAverage = parseFloat(rawDataEntries[i + 1]);
				i += 1;
			}
			if (rawDataEntries[i] == "oldestWholeAverage") {
				m.oldestWholeAverage = parseFloat(rawDataEntries[i + 1]);
				i += 1;
			}
			if (rawDataEntries[i] == "oldestBandsAverage") {
				m.oldestBandsAverage = parseFloat(rawDataEntries[i + 1]);
				i += 1;
			}
			if (rawDataEntries[i] == "amplitudeSpectrum") {
				for (let j = i + 1; j < rawDataEntries.length; ++j) {
					m.amplitudeSpectrum[j - i - 1] = parseFloat(rawDataEntries[j]);
				}
				break;
			}
		}

		return m;
	}
}
