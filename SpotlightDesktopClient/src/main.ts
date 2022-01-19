import { app, BrowserWindow, WebContents, } from "electron";
import * as net from "net";
import * as path from "path";

class WelcomeMessage {

  message: string;

  constructor(message: string) {
    this.message = message;
  }
}

class SamplesBatchMessage {

  samples: number[];

  constructor(samples: number[]) {
    this.samples = samples;
  }
}

class AmplitudeSpectrumMessage {

  spectrum: number[];

  constructor(spectrum: number[]) {
    this.spectrum = spectrum;
  }
}

enum MessageType { Welcome, SamplesBatch, AmplitudeSpectrum }

enum DataProcessorState { Begin, Header, DataOrEnd, }

class DataProcessor {
  private unprocessedRawData: string;
  private state: DataProcessorState;
  private currentMessageType: MessageType;
  private currentHeader: string;
  private dataEntries: string[];
  private onMessage: { (messageType: MessageType, message: unknown) : void };

  constructor(onMessage: { (messageType: MessageType, message: unknown) : void }) {
    this.unprocessedRawData = "";
    this.state = DataProcessorState.Begin;
    this.onMessage = onMessage;
  }

  process(data: string) {
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
          }
          catch (ex) {
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
    }
    return undefined;
  }

  createMessage(): unknown {
    switch (this.currentMessageType) {
      case MessageType.Welcome:
        return new WelcomeMessage(this.dataEntries[0]);
      case MessageType.SamplesBatch:
        return new SamplesBatchMessage(this.dataEntries.map(v => parseInt(v)));
      case MessageType.AmplitudeSpectrum:
        return new AmplitudeSpectrumMessage(this.dataEntries.map(v => parseFloat(v)));
    }
  }
}

function supplyData(webContents: WebContents) {
  const dataProcesseor = new DataProcessor((messageType, message) => {
    switch (messageType) {
      case MessageType.Welcome: {
        const welcomeMessage = <WelcomeMessage>message;
        webContents.send("welcome-channel", { "message": welcomeMessage.message });
        break;
      }
      case MessageType.SamplesBatch: {
        const samplesBatchMessage = <SamplesBatchMessage>message;
        webContents.send("samples-channel", { "samples": samplesBatchMessage.samples });
        break;
      }
      case MessageType.AmplitudeSpectrum: {
        const samplesBatchMessage = <AmplitudeSpectrumMessage>message;
        webContents.send("spectrum-channel", { "spectrum": samplesBatchMessage.spectrum });
        break;
      }
    }
  });
  const client = net.connect(33334, "192.168.0.45");
  client.on('data', (data) => {
    const sData = data.toString();
    dataProcesseor.process(sData);
    //console.log(sData);
    // const splittedData = sData.split(";");
    // const samples:number[] = [];
    // let i = 0;
    // splittedData.forEach(sv => {
    //   if (i < 2205) {
    //     ++i;
    //     const v = parseInt(sv);
    //     if (!isNaN(v)) {
    //       samples.push(v);
    //     }
    //   }
    // });
    // if (samples.length > 0) {
    //   webContents.send("ch1", { "samples": samples });
    // }
  });
}

function supplyFakeData(webContents: WebContents)
{
  setTimeout(() => {
    webContents.send("ch1", { "samples": Array.from({length: 2205}, () => Math.floor(Math.random() * 4095)) });
    supplyFakeData(webContents);
  }, 50);
}

function createWindow() {
  // Create the browser window.
  const mainWindow = new BrowserWindow({
    width: 1200,
    height: 800,
    webPreferences: {
      nodeIntegration: false,
      contextIsolation: true,
      preload: path.join(__dirname, "preload.js")
    },
  });

  // and load the index.html of the app.
  mainWindow.loadFile(path.join(__dirname, "../index.html"));

  // Open the DevTools.
  mainWindow.webContents.openDevTools();

  setTimeout(() =>{
    supplyData(mainWindow.webContents);
  }, 3000);
}

// This method will be called when Electron has finished
// initialization and is ready to create browser windows.
// Some APIs can only be used after this event occurs.
app.on("ready", () => {
  createWindow();

  app.on("activate", function () {
    // On macOS it's common to re-create a window in the app when the
    // dock icon is clicked and there are no other windows open.
    if (BrowserWindow.getAllWindows().length === 0) createWindow();
  });
});

// Quit when all windows are closed, except on macOS. There, it's common
// for applications and their menu bar to stay active until the user quits
// explicitly with Cmd + Q.
app.on("window-all-closed", () => {
  if (process.platform !== "darwin") {
    app.quit();
  }
});

// In this file you can include the rest of your app"s specific main process
// code. You can also put them in separate files and require them here.

