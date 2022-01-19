import { ipcRenderer, contextBridge } from "electron";

export const samplesApi = {
  onNext: (callback: (samples: number[]) => void) : void => {
    ipcRenderer.on("ch1", (event, msg) => {
      callback(msg.samples);
    });
  }
};

contextBridge.exposeInMainWorld("samplesApi", samplesApi);

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

