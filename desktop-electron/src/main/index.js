import { app, shell, BrowserWindow, ipcMain } from 'electron'
import { join, resolve, dirname } from 'path'
import { electronApp, optimizer, is } from '@electron-toolkit/utils'
import icon from '../../resources/icon.png?asset'
import { spawn } from 'child_process'
import { existsSync } from 'fs'

// Storing the C++ process globally
let cppProcess = null

function startCppProcess() {
  const executablePath = getExecutablePath()
  console.log('🚀 Starting C++ backend at:', executablePath)

  if (!existsSync(executablePath)) {
    console.error('❌ C++ executable not found at:', executablePath)
    const mainWindow = BrowserWindow.getAllWindows()[0]
    if (mainWindow) {
      mainWindow.webContents.send('cpp-error', `C++ executable not found at: ${executablePath}`)
    }
    return
  }

  const workingDir = dirname(executablePath)
  console.log('📁 Working directory:', workingDir)
  console.log('🔧 Spawning process:', executablePath, 'with args:', ['--port', '3001'])

  cppProcess = spawn(executablePath, ['--port', '3001'], {
    stdio: ['ignore', 'pipe', 'pipe'], // Don't pipe stdin to avoid process termination
    detached: false,
    cwd: workingDir, // Set working directory to the build folder
    windowsHide: true
  })

  // Handle data from C++ process
  cppProcess.stdout.on('data', (data) => {
    const output = data.toString()
    console.log('🟢 C++ stdout:', output)
    // Send data to renderer process
    const mainWindow = BrowserWindow.getAllWindows()[0]
    if (mainWindow) {
      mainWindow.webContents.send('cpp-output', output)
    }
  })

  cppProcess.stderr.on('data', (data) => {
    const error = data.toString()
    console.error('🔴 C++ stderr:', error)
    const mainWindow = BrowserWindow.getAllWindows()[0]
    if (mainWindow) {
      mainWindow.webContents.send('cpp-error', error)
    }
  })

  cppProcess.on('close', (code, signal) => {
    console.log(`🛑 C++ process exited with code ${code} and signal ${signal}`)
    if (code !== 0 && code !== null) {
      console.error('❌ C++ process exited with non-zero code. Try running the backend manually:', executablePath, '--port 3001')
    }
    if (signal) {
      console.error('💀 C++ process was killed by signal:', signal)
    }
    cppProcess = null
    const mainWindow = BrowserWindow.getAllWindows()[0]
    if (mainWindow) {
      mainWindow.webContents.send('cpp-closed', { code, signal })
    }
  })

  cppProcess.on('error', (error) => {
    console.error('💥 Failed to start C++ process:', error)
    console.error('🔧 Try running the backend manually:', executablePath, '--port 3001')
    const mainWindow = BrowserWindow.getAllWindows()[0]
    if (mainWindow) {
      mainWindow.webContents.send('cpp-error', error.message)
    }
  })
}

// Helper to get the correct executable path
function getExecutablePath() {
  // For development, use the built executable
  if (is.dev) {
    // Get the project root directory by going up from the Electron app directory
    const electronDir = dirname(dirname(__dirname)) // Go up from out/main/
    const projectRoot = dirname(electronDir) // Go up from desktop-electron/
    const execPath = join(projectRoot, 'SimpleBlockchain', 'build', 'simpleblockchain')
    console.log('📍 Resolved executable path:', execPath)
    return execPath
  }

  // For production, look in resources
  const baseDir = join(process.resourcesPath, 'executables')
  const exeName = 'simpleblockchain'
  return join(baseDir, exeName)
}

// Test backend connection
function testBackendConnection() {
  const http = require('http')

  console.log('🔍 Testing backend connection to http://127.0.0.1:3001/api/blockchain')

  const req = http.get('http://127.0.0.1:3001/api/blockchain', (res) => {
    console.log('✅ Backend connection test successful! Status:', res.statusCode)
    let data = ''
    res.on('data', chunk => data += chunk)
    res.on('end', () => {
      console.log('📊 Backend response length:', data.length)
      const mainWindow = BrowserWindow.getAllWindows()[0]
      if (mainWindow) {
        mainWindow.webContents.send('backend-ready', true)
      }
    })
  })

  req.on('error', (err) => {
    console.error('❌ Backend connection test failed:', err.message)
    const mainWindow = BrowserWindow.getAllWindows()[0]
    if (mainWindow) {
      mainWindow.webContents.send('backend-error', `Connection failed: ${err.message}`)
    }
  })

  req.setTimeout(5000, () => {
    console.error('⏰ Backend connection test timed out')
    req.destroy()
  })
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
      sandbox: false,
      nodeIntegration: false,
      contextIsolation: true,
      // IMPORTANT: Enable these for network requests to work
      webSecurity: is.dev ? false : true, // Disable web security only in development
      allowRunningInsecureContent: is.dev,
      experimentalFeatures: true
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
  // IMPORTANT: Disable web security for development
  if (is.dev) {
    app.commandLine.appendSwitch('--no-sandbox')
    app.commandLine.appendSwitch('--disable-setuid-sandbox')
    app.commandLine.appendSwitch('--disable-web-security')
    app.commandLine.appendSwitch('--disable-features=VizDisplayCompositor')
    app.commandLine.appendSwitch('--allow-running-insecure-content')
  }

  electronApp.setAppUserModelId('com.electron')

  app.on('browser-window-created', (_, window) => {
    optimizer.watchWindowShortcuts(window)
  })

  setupIpcHandlers()
  createWindow()

  // Start the C++ backend automatically after a short delay
  setTimeout(() => {
    console.log('🚀 Auto-starting C++ backend...')
    startCppProcess()

    // Test connection after giving the backend time to start
    setTimeout(() => {
      console.log('🔍 Testing backend connection...')
      testBackendConnection()
    }, 5000) // Increased delay to 5 seconds
  }, 1000)

  app.on('activate', function () {
    if (BrowserWindow.getAllWindows().length === 0) createWindow()
  })
})

// Cleanup C++ process on app quit
app.on('before-quit', () => {
  if (cppProcess) {
    console.log('🛑 Killing C++ process before quit')
    cppProcess.kill()
    cppProcess = null
  }
})

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit()
  }
})
