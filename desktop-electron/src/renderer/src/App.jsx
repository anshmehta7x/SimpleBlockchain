import React, { useState } from 'react'

function App() {
  const ipcHandle = () => window.electron.ipcRenderer.send('ping')
  const [isChainLoaded, setIsChainLoaded] = useState(false)

  return (
    <main className="flex flex-col justify-end items-center">
      <h1 className="text-4xl font-bold">SimpleBlockchain</h1>
      <div className="flex flex-col justify-center items-center">
        <h2 className="text-2xl font-bold">Drag and drop your blockchain file here</h2>
        <button
          className="bg-blue-500 hover:bg-blue-700 text-white font-bold py-2 px-4 rounded"
          onClick={() => {
            setIsChainLoaded(true)
          }}
        >
          Load blockchain
        </button>
        {isChainLoaded && <h2 className="text-2xl font-bold">Blockchain loaded</h2>}
      </div>
    </main>
  )
}

export default App
