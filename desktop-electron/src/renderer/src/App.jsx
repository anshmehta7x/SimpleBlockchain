import { useState, useEffect } from 'react';
import BlockchainOverview from './components/BlockchainOverview';
import BlockDetails from './components/BlockDetails';
import TransactionManager from './components/TransactionManager';
import VerificationPanel from './components/VerificationPanel';

function App() {
  const [activeTab, setActiveTab] = useState('overview');
  const [serverStatus, setServerStatus] = useState('starting');
  const [connectionError, setConnectionError] = useState(null);
  const [connectionAttempt, setConnectionAttempt] = useState(0);

  useEffect(() => {
    // The C++ process is started automatically by the main process
    // Just try to connect with retry logic
    const initializeConnection = async () => {
      try {
        console.log('Waiting for C++ backend to start...');
        
        // Try to connect with retry logic
        const maxRetries = 15; // Increased to 15 attempts
        const retryDelay = 1000; // 1 second
        
        const tryConnection = async (attempt) => {
          try {
            console.log(`Connection attempt ${attempt}/${maxRetries}`);
            setConnectionAttempt(attempt);
            const result = await window.api.getBlockchain();
            if (result.success) {
              console.log('✅ Successfully connected to blockchain server');
              setServerStatus('connected');
              return true;
            } else {
              console.log('❌ Connection failed:', result.message);
              if (attempt >= maxRetries) {
                setServerStatus('error');
                setConnectionError(`Failed to connect after ${maxRetries} attempts: ${result.message || 'Unknown error'}`);
                return false;
              }
              
              // Retry after delay
              setTimeout(() => tryConnection(attempt + 1), retryDelay);
              return false;
            }
          } catch (error) {
            console.log(`❌ Connection error on attempt ${attempt}:`, error.message);
            if (attempt >= maxRetries) {
              setServerStatus('error');
              setConnectionError(`Failed to connect after ${maxRetries} attempts: ${error.message}`);
              return false;
            }
            
            // Retry after delay
            setTimeout(() => tryConnection(attempt + 1), retryDelay);
            return false;
          }
        };
        
        // Start connection attempts after a short delay
        setTimeout(() => tryConnection(1), 3000); // Increased initial delay to 3 seconds
        
      } catch (error) {
        console.error('Connection initialization error:', error);
        setServerStatus('error');
        setConnectionError(`Failed to initialize connection: ${error.message}`);
      }
    };

    initializeConnection();

    // Cleanup function
    return () => {
      if (window.api && window.api.stopCpp) {
        window.api.stopCpp();
      }
    };
  }, []);

  const tabs = [
    { id: 'overview', label: 'Blockchain Overview', component: BlockchainOverview },
    { id: 'blocks', label: 'Block Details', component: BlockDetails },
    { id: 'transactions', label: 'Transaction Manager', component: TransactionManager },
    { id: 'verification', label: 'Verification', component: VerificationPanel }
  ];

  const ActiveComponent = tabs.find(tab => tab.id === activeTab)?.component;

  if (serverStatus === 'starting') {
    return (
      <div className="flex items-center justify-center h-screen">
        <div className="text-center">
          <div className="animate-spin rounded-full h-32 w-32 border-b-2 border-blue-500 mx-auto"></div>
          <p className="mt-4 text-gray-600">Starting blockchain server...</p>
          {connectionAttempt > 0 && (
            <p className="mt-2 text-sm text-gray-500">
              Connection attempt {connectionAttempt}/15
            </p>
          )}
        </div>
      </div>
    );
  }

  if (serverStatus === 'error') {
    return (
      <div className="flex items-center justify-center h-screen">
        <div className="text-center bg-red-50 p-8 rounded-lg max-w-lg">
          <div className="text-red-500 text-6xl mb-4">⚠️</div>
          <h2 className="text-xl font-bold text-red-700 mb-2">Server Error</h2>
          <p className="text-red-600 mb-4">{connectionError}</p>
          <div className="text-sm text-gray-600 mb-4">
            <p>Troubleshooting tips:</p>
            <ul className="text-left list-disc list-inside mt-2">
              <li>Make sure the C++ backend is built: <code>npm run build:cpp</code></li>
              <li>Check if port 3001 is available</li>
              <li>Try running the backend manually: <code>./SimpleBlockchain/build/simpleblockchain --port 3001</code></li>
            </ul>
          </div>
          <button 
            onClick={() => window.location.reload()} 
            className="mt-4 bg-red-500 text-white px-4 py-2 rounded hover:bg-red-600"
          >
            Retry
          </button>
        </div>
      </div>
    );
  }

  return (
    <div className="flex flex-col h-screen bg-gray-100">
      {/* Header */}
      <header className="bg-blue-600 text-white p-4 shadow-lg">
        <h1 className="text-2xl font-bold">Educational Blockchain Tool</h1>
        <p className="text-blue-100">Learn and explore blockchain technology</p>
      </header>

      {/* Navigation Tabs */}
      <nav className="bg-white shadow-sm border-b">
        <div className="flex space-x-0">
          {tabs.map((tab) => (
            <button
              key={tab.id}
              onClick={() => setActiveTab(tab.id)}
              className={`px-6 py-3 font-medium text-sm border-b-2 transition-colors ${
                activeTab === tab.id
                  ? 'border-blue-500 text-blue-600 bg-blue-50'
                  : 'border-transparent text-gray-500 hover:text-gray-700 hover:border-gray-300'
              }`}
            >
              {tab.label}
            </button>
          ))}
        </div>
      </nav>

      {/* Main Content */}
      <main className="flex-1 overflow-hidden">
        {ActiveComponent && <ActiveComponent />}
      </main>

      {/* Status Bar */}
      <footer className="bg-gray-800 text-white p-2 text-sm">
        <div className="flex justify-between items-center">
          <span>Status: {serverStatus === 'connected' ? '🟢 Connected' : '🔴 Disconnected'}</span>
          <span>Educational Blockchain Tool v1.0</span>
        </div>
      </footer>
    </div>
  );
}

export default App;
