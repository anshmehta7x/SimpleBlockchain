import { contextBridge, ipcRenderer } from 'electron'
import { electronAPI } from '@electron-toolkit/preload'

const API_BASE_URL = 'http://127.0.0.1:3001/api'

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

  // HTTP API methods for blockchain operations
  getBlockchain: async () => {
    try {
      console.log(`Fetching blockchain from ${API_BASE_URL}/blockchain`);
      const response = await fetch(`${API_BASE_URL}/blockchain`);
      console.log('Response status:', response.status);
      if (!response.ok) {
        throw new Error(`HTTP ${response.status}: ${response.statusText}`);
      }
      const data = await response.json();
      console.log('Blockchain data received:', data);
      return data;
    } catch (error) {
      console.error('getBlockchain error:', error);
      return { success: false, message: error.message };
    }
  },

  getBlock: async (index) => {
    try {
      const response = await fetch(`${API_BASE_URL}/block/${index}`)
      return await response.json()
    } catch (error) {
      return { success: false, message: error.message }
    }
  },

  getTransactions: async () => {
    try {
      const response = await fetch(`${API_BASE_URL}/transactions`)
      return await response.json()
    } catch (error) {
      return { success: false, message: error.message }
    }
  },

  addTransaction: async (sender, receiver, amount) => {
    try {
      const response = await fetch(`${API_BASE_URL}/transaction`, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({ sender, receiver, amount })
      })
      return await response.json()
    } catch (error) {
      return { success: false, message: error.message }
    }
  },

  editTransaction: async (hash, sender, receiver, amount) => {
    try {
      const response = await fetch(`${API_BASE_URL}/transaction/${hash}`, {
        method: 'PUT',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({ sender, receiver, amount })
      })
      return await response.json()
    } catch (error) {
      return { success: false, message: error.message }
    }
  },

  removeTransaction: async (hash) => {
    try {
      const response = await fetch(`${API_BASE_URL}/transaction/${hash}`, {
        method: 'DELETE'
      })
      return await response.json()
    } catch (error) {
      return { success: false, message: error.message }
    }
  },

  verifyBlockchain: async () => {
    try {
      const response = await fetch(`${API_BASE_URL}/verify`)
      return await response.json()
    } catch (error) {
      return { success: false, message: error.message }
    }
  },

  initializeBlockchain: async (difficulty, blockSize) => {
    try {
      const response = await fetch(`${API_BASE_URL}/initialize`, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({ difficulty, blockSize })
      })
      return await response.json()
    } catch (error) {
      return { success: false, message: error.message }
    }
  },

  tamperWithBlock: async (blockIndex) => {
    try {
      const response = await fetch(`${API_BASE_URL}/tamper`, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({ blockIndex })
      })
      return await response.json()
    } catch (error) {
      return { success: false, message: error.message }
    }
  }
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
