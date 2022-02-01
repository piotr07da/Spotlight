import { ipcRenderer, contextBridge } from "electron";

export const spotlightApi = {
	onNextSamples: (callback: (samples: number[]) => void): void => {
		ipcRenderer.on("samples-channel", (event, msg) => {
			callback(msg.samples);
		});
	},
	onNextSpectrum: (callback: (spectrum: number[]) => void): void => {
		ipcRenderer.on("spectrum-channel", (event, msg) => {
			callback(msg.spectrum);
		});
	},
	onNextAudioTrigger: (
		callback: (bandMinFrequencyValueIndex: number, bandMaxFrequencyValueIndex: number, wholeSpectrumAvg: number, bandsSpectrumAvg: number, amplitudeSpectrum: number[]) => void
	): void => {
		ipcRenderer.on("audio-trigger-channel", (event, msg) => {
			callback(msg.bandMinFrequencyValueIndex, msg.bandMaxFrequencyValueIndex, msg.wholeSpectrumAvg, msg.bandsSpectrumAvg, msg.amplitudeSpectrum);
		});
	},
};

contextBridge.exposeInMainWorld("spotlightApi", spotlightApi);

// All of the Node.js APIs are available in the preload process.
// It has the same sandbox as a Chrome extension.
window.addEventListener("DOMContentLoaded", () => {
	const replaceText = (selector: string, text: string) => {
		const element = document.getElementById(selector);
		if (element) {
			element.innerText = text;
		}
	};

	for (const type of ["chrome", "node", "electron"]) {
		replaceText(`${type}-version`, process.versions[type as keyof NodeJS.ProcessVersions]);
	}
});
