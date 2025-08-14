import { useState, useEffect } from 'react';
import BlockchainOverview from './components/BlockchainOverview';
import BlockDetails from './components/BlockDetails';
import TransactionManager from './components/TransactionManager';
import VerificationPanel from './components/VerificationPanel';

function App() {
  const [activeTab, setActiveTab] = useState('overview');
  const [serverStatus, setServerStatus] = useState('starting');
  const [connectionError, setConnectionError] = useState(null);

  useEffect(() => {
    // Start the C++ process when the app loads
    const initializeServer = async () => {
      try {
        await window.api.startCpp();
        // Wait a bit for the server to start up
        setTimeout(async () => {
          try {
            const result = await window.api.getBlockchain();
            if (result.success) {
              setServerStatus('connected');
            } else {
              setServerStatus('error');
              setConnectionError('Failed to connect to blockchain server');
            }
          } catch (error) {
            setServerStatus('error');
            setConnectionError('Failed to connect to blockchain server');
          }
        }, 3000);
      } catch (error) {
        setServerStatus('error');
        setConnectionError('Failed to start blockchain server');
      }
    };

    initializeServer();

    // Cleanup function
    return () => {
      window.api.stopCpp();
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
        </div>
      </div>
    );
  }

  if (serverStatus === 'error') {
    return (
      <div className="flex items-center justify-center h-screen">
        <div className="text-center bg-red-50 p-8 rounded-lg">
          <div className="text-red-500 text-6xl mb-4">⚠️</div>
          <h2 className="text-xl font-bold text-red-700 mb-2">Server Error</h2>
          <p className="text-red-600">{connectionError}</p>
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
