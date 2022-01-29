import { DataProcessorState } from "./data-processor-state";
import { MessageType } from "./message-type";
import { WelcomeMessage } from "./welcome-message";
import { SamplesBatchMessage } from "./samples-batch-message";
import { AmplitudeSpectrumMessage } from "./amplitude-spectrum-message";
import { AudioTriggerMessage } from "./audio-trigger-message";

export class DataProcessor {
	private unprocessedRawData: string;
	private state: DataProcessorState;
	private currentMessageType: MessageType;
	private currentHeader: string;
	private dataEntries: string[];
	private onMessage: { (messageType: MessageType, message: unknown): void };

	constructor(onMessage: { (messageType: MessageType, message: unknown): void }) {
		this.unprocessedRawData = "";
		this.state = DataProcessorState.Begin;
		this.onMessage = onMessage;
	}

	process(data: string): void {
		this.unprocessedRawData += data;
		let continueAnalyze = true;
		while (continueAnalyze) {
			continueAnalyze = this.analyze();
		}
	}

	analyze(): boolean {
		switch (this.state) {
			case DataProcessorState.Begin: {
				const beginIx = this.unprocessedRawData.indexOf("BEGIN ");
				if (beginIx == -1) {
					return false;
				}
				const beginDelimiterIx = this.unprocessedRawData.indexOf(";", beginIx + 6);
				if (beginDelimiterIx == -1) {
					return false;
				}
				const messageName = this.unprocessedRawData.substring(beginIx + 6, beginDelimiterIx);
				this.currentMessageType = this.toMessageType(messageName);
				this.unprocessedRawData = this.unprocessedRawData.substring(beginDelimiterIx + 1);
				this.state = DataProcessorState.Header;
				return true;
			}
			case DataProcessorState.Header: {
				const headerDelimiterIx = this.unprocessedRawData.indexOf(";");
				if (headerDelimiterIx == -1) {
					return false;
				}
				this.currentHeader = this.unprocessedRawData.substring(0, headerDelimiterIx);
				this.unprocessedRawData = this.unprocessedRawData.substring(headerDelimiterIx + 1);
				this.dataEntries = [];
				this.state = DataProcessorState.DataOrEnd;
				return true;
			}
			case DataProcessorState.DataOrEnd: {
				const dataDelimiterIx = this.unprocessedRawData.indexOf(";");
				if (dataDelimiterIx == -1) {
					return false;
				}
				const dataEntry = this.unprocessedRawData.substring(0, dataDelimiterIx);
				if (dataEntry.trim() == "END") {
					try {
						this.onMessage(this.currentMessageType, this.createMessage());
					} catch (ex) {
						console.log(ex);
					}
					this.unprocessedRawData = this.unprocessedRawData.substring(dataDelimiterIx + 1);
					this.state = DataProcessorState.Begin;
					return true;
				}
				this.dataEntries.push(dataEntry);
				this.unprocessedRawData = this.unprocessedRawData.substring(dataDelimiterIx + 1);
				return true;
			}
		}
	}

	toMessageType(messageName: string): MessageType {
		switch (messageName) {
			case "WELCOME":
				return MessageType.Welcome;
			case "SAMPLES_BATCH":
				return MessageType.SamplesBatch;
			case "AMPLITUDE_SPECTRUM":
				return MessageType.AmplitudeSpectrum;
			case "AUDIO_TRIGGER":
				return MessageType.AudioTrigger;
		}
		return undefined;
	}

	createMessage(): unknown {
		switch (this.currentMessageType) {
			case MessageType.Welcome:
				return WelcomeMessage.from(this.dataEntries);
			case MessageType.SamplesBatch:
				return SamplesBatchMessage.from(this.dataEntries);
			case MessageType.AmplitudeSpectrum:
				return AmplitudeSpectrumMessage.from(this.dataEntries);
			case MessageType.AudioTrigger:
				return AudioTriggerMessage.from(this.dataEntries);
		}
	}
}
