import { app, shell, BrowserWindow, ipcMain } from 'electron'
import { join } from 'path'
import { electronApp, optimizer, is } from '@electron-toolkit/utils'
import icon from '../../resources/icon.png?asset'
import { spawn } from 'child_process'

// Storing the C++ process globally
let cppProcess = null

function startCppProcess() {
  const executablePath = getExecutablePath()

  cppProcess = spawn(executablePath)

  // Handle data from C++ process
  cppProcess.stdout.on('data', (data) => {
    // Send data to renderer process
    const mainWindow = BrowserWindow.getAllWindows()[0]
    if (mainWindow) {
      mainWindow.webContents.send('cpp-output', data.toString())
    }
  })

  cppProcess.stderr.on('data', (data) => {
    const mainWindow = BrowserWindow.getAllWindows()[0]
    if (mainWindow) {
      mainWindow.webContents.send('cpp-error', data.toString())
    }
  })

  cppProcess.on('close', (code) => {
    console.log(`C++ process exited with code ${code}`)
    cppProcess = null
    const mainWindow = BrowserWindow.getAllWindows()[0]
    if (mainWindow) {
      mainWindow.webContents.send('cpp-closed', code)
    }
  })

  cppProcess.on('error', (error) => {
    console.error('Failed to start C++ process:', error)
    const mainWindow = BrowserWindow.getAllWindows()[0]
    if (mainWindow) {
      mainWindow.webContents.send('cpp-error', error.message)
    }
  })
}

// Helper to get the correct executable path
function getExecutablePath() {
  const baseDir = is.dev
    ? join(__dirname, '../../resources/executables')
    : join(process.resourcesPath, 'executables')

  const exeName = 'simpleblockchain'
  return join(baseDir, exeName)
}

// IPC handlers for C++ process interaction
function setupIpcHandlers() {
  // Start the C++ process
  ipcMain.handle('start-cpp', () => {
    if (!cppProcess) {
      startCppProcess()
      return { success: true, message: 'C++ process started' }
    }
    return { success: false, message: 'C++ process already running' }
  })

  // Send data to C++ process
  ipcMain.handle('send-to-cpp', (_, data) => {
    if (cppProcess && cppProcess.stdin.writable) {
      cppProcess.stdin.write(data + '\n')
      return { success: true }
    }
    return { success: false, message: 'C++ process not running' }
  })

  // Stop the C++ process
  ipcMain.handle('stop-cpp', () => {
    if (cppProcess) {
      cppProcess.kill()
      cppProcess = null
      return { success: true }
    }
    return { success: false, message: 'C++ process not running' }
  })

  ipcMain.handle('get-chain-status', async () => {
    if (!cppProcess) return { success: false, message: 'Process not running' }
    return new Promise((resolve) => {
      cppProcess.stdin.write('status\n')
      const timeout = setTimeout(() => {
        resolve({ success: false, message: 'Timeout' })
      }, 5000)

      cppProcess.stdout.once('data', (data) => {
        clearTimeout(timeout)
        resolve({ success: true, data: JSON.parse(data.toString()) })
      })
    })
  })

  ipcMain.handle('start-mining', async () => {
    if (!cppProcess) return { success: false }
    cppProcess.stdin.write('mine\n')
    return { success: true }
  })

  ipcMain.handle('stop-mining', async () => {
    if (!cppProcess) return { success: false }
    cppProcess.stdin.write('stop\n')
    return { success: true }
  })
}

function createWindow() {
  const mainWindow = new BrowserWindow({
    width: 900,
    height: 670,
    show: false,
    autoHideMenuBar: true,
    ...(process.platform === 'linux' ? { icon } : {}),
    webPreferences: {
      preload: join(__dirname, '../preload/index.js'),
      sandbox: false
    }
  })

  mainWindow.on('ready-to-show', () => {
    mainWindow.show()
  })

  mainWindow.webContents.setWindowOpenHandler((details) => {
    shell.openExternal(details.url)
    return { action: 'deny' }
  })

  if (is.dev && process.env['ELECTRON_RENDERER_URL']) {
    mainWindow.loadURL(process.env['ELECTRON_RENDERER_URL'])
  } else {
    mainWindow.loadFile(join(__dirname, '../renderer/index.html'))
  }
}

app.whenReady().then(() => {
  electronApp.setAppUserModelId('com.electron')

  app.on('browser-window-created', (_, window) => {
    optimizer.watchWindowShortcuts(window)
  })

  setupIpcHandlers()
  createWindow()

  app.on('activate', function () {
    if (BrowserWindow.getAllWindows().length === 0) createWindow()
  })
})

// Cleanup C++ process on app quit
app.on('before-quit', () => {
  if (cppProcess) {
    cppProcess.kill()
    cppProcess = null
  }
})

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit()
  }
})
