import { contextBridge, ipcRenderer } from 'electron'
import { electronAPI } from '@electron-toolkit/preload'

const api = {
  // Methods to control C++ process
  startCpp: () => ipcRenderer.invoke('start-cpp'),
  stopCpp: () => ipcRenderer.invoke('stop-cpp'),
  sendToCpp: (data) => ipcRenderer.invoke('send-to-cpp', data),

  // Event listeners for C++ process output
  onCppOutput: (callback) => ipcRenderer.on('cpp-output', callback),
  onCppError: (callback) => ipcRenderer.on('cpp-error', callback),
  onCppClosed: (callback) => ipcRenderer.on('cpp-closed', callback),

  // Event removal methods
  offCppOutput: (callback) => ipcRenderer.removeListener('cpp-output', callback),
  offCppError: (callback) => ipcRenderer.removeListener('cpp-error', callback),
  offCppClosed: (callback) => ipcRenderer.removeListener('cpp-closed', callback),

  getChainStatus: () => ipcRenderer.invoke('get-chain-status'),
  startMining: () => ipcRenderer.invoke('start-mining'),
  stopMining: () => ipcRenderer.invoke('stop-mining'),
  sendTransaction: (tx) => ipcRenderer.invoke('send-to-cpp', JSON.stringify(tx))
}

if (process.contextIsolated) {
  try {
    contextBridge.exposeInMainWorld('electron', electronAPI)
    contextBridge.exposeInMainWorld('api', api)
  } catch (error) {
    console.error(error)
  }
} else {
  window.electron = electronAPI
  window.api = api
}
