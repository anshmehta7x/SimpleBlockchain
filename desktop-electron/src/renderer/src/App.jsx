import { useState, useEffect } from 'react';
import SetupChain from './assets/SetupChain';

function App() {
  const [output, setOutput] = useState([]);
  const [isRunning, setIsRunning] = useState(false);
  const [showSetup, setShowSetup] = useState(false); // State to show or hide SetupChain

  const startProcess = async () => {
    try {
      await window.api.startCpp();
      setIsRunning(true);
    } catch (error) {
      console.error('Failed to start C++ process:', error);
    }
  };

  const stopProcess = async () => {
    try {
      await window.api.stopCpp();
      setIsRunning(false);
    } catch (error) {
      console.error('Failed to stop C++ process:', error);
    }
  };

  const sendCommand = async (command) => {
    try {
      await window.api.sendToCpp(command);
    } catch (error) {
      console.error('Failed to send command:', error);
    }
  };

  useEffect(() => {
    // Start process on component mount
    startProcess();

    // Set up listeners for C++ process
    const outputListener = (_event, data) => {
      if (data === 'BlockChain service Started') {
        setShowSetup(true); // Show setup popup when blockchain service starts
      }
      setOutput((prev) => [...prev, { type: 'output', text: data }]);
    };

    const errorListener = (_event, data) => {
      setOutput((prev) => [...prev, { type: 'error', text: data }]);
    };

    const closedListener = (_event, code) => {
      setIsRunning(false);
      setOutput((prev) => [...prev, { type: 'system', text: `Process exited with code ${code}` }]);
    };

    // Register listeners
    window.api.onCppOutput(outputListener);
    window.api.onCppError(errorListener);
    window.api.onCppClosed(closedListener);

    // Cleanup function
    return () => {
      stopProcess();
      // Remove listeners if needed
      window.api.offCppOutput && window.api.offCppOutput(outputListener);
      window.api.offCppError && window.api.offCppError(errorListener);
      window.api.offCppClosed && window.api.offCppClosed(closedListener);
    };
  }, []);

  return (
    <main className="flex flex-col h-screen p-4">
      <div className="flex-1 bg-gray-100 p-4 rounded overflow-auto">
        {output.map((item, index) => (
          <div
            key={index}
            className={`mb-1 font-mono ${
              item.type === 'error' 
                ? 'text-red-600' 
                : item.type === 'system' 
                ? 'text-blue-600' 
                : 'text-gray-800'
            }`}
          >
            {item.text}
          </div>
        ))}
      </div>
      
      {showSetup && <SetupChain sendCommand={sendCommand} setShowSetup={setShowSetup} />} {/* Show SetupChain popup */}
    </main>
  );
}

export default App;
