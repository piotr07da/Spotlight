import { app, BrowserWindow, WebContents } from "electron";
import * as net from "net";
import * as path from "path";
import { DataProcessor, MessageType, WelcomeMessage, SamplesBatchMessage, AmplitudeSpectrumMessage, AudioTriggerMessage } from "./data-processing/data-processing-module";

function supplyData(webContents: WebContents) {
	const dataProcesseor = new DataProcessor((messageType, message) => {
		switch (messageType) {
			case MessageType.Welcome: {
				const welcomeMessage = <WelcomeMessage>message;
				webContents.send("welcome-channel", welcomeMessage);
				break;
			}
			case MessageType.SamplesBatch: {
				const samplesBatchMessage = <SamplesBatchMessage>message;
				webContents.send("samples-channel", samplesBatchMessage);
				break;
			}
			case MessageType.AmplitudeSpectrum: {
				const samplesBatchMessage = <AmplitudeSpectrumMessage>message;
				webContents.send("spectrum-channel", samplesBatchMessage);
				break;
			}
			case MessageType.AudioTrigger: {
				const audioTriggerMessage = <AudioTriggerMessage>message;
				webContents.send("audio-trigger-channel", audioTriggerMessage);
				break;
			}
		}
	});

	const connect = function () {
		// TURN OFF VPN !
		const port = 33334;
		const ip = "192.168.0.45";
		const client = new net.Socket();
		client.setKeepAlive(true, 1000);
		client.connect(port, ip);
		client.on("data", (data) => {
			const sData = data.toString();
			dataProcesseor.process(sData);
		});
		client.on("error", () => reconnect(client));
		client.on("close", () => reconnect(client));
		client.on("end", () => reconnect(client));
	};

	const reconnect = function (client: net.Socket) {
		client.removeAllListeners();
		client.destroy();
		connect();
	};

	connect();
}

function supplyFakeData(webContents: WebContents) {
	setTimeout(() => {
		webContents.send("ch1", { samples: Array.from({ length: 2205 }, () => Math.floor(Math.random() * 4095)) });
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
			preload: path.join(__dirname, "preload.js"),
		},
	});

	// and load the index.html of the app.
	mainWindow.loadFile(path.join(__dirname, "../index.html"));

	// Open the DevTools.
	mainWindow.webContents.openDevTools();

	setTimeout(() => {
		supplyData(mainWindow.webContents);
	}, 1000);
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
