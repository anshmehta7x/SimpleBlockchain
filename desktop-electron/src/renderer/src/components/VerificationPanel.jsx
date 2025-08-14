import { useState, useEffect } from 'react';
import { blockchainAPI } from '../utils/api';


function VerificationPanel() {
  const [verificationResult, setVerificationResult] = useState(null);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState(null);
  const [blockchain, setBlockchain] = useState(null);
  const [tamperResult, setTamperResult] = useState(null);

  const verifyBlockchain = async () => {
    setLoading(true);
    setError(null);
    try {
      const result = await blockchainAPI.verifyBlockchain();
      if (result.success) {
        setVerificationResult(result);
      } else {
        setError(result.message);
      }
    } catch (err) {
      setError('Failed to verify blockchain');
    } finally {
      setLoading(false);
    }
  };

  const fetchBlockchain = async () => {
    try {
      const result = await blockchainAPI.getBlockchain();
      if (result.success) {
        setBlockchain(result);
      }
    } catch (err) {
      console.error('Failed to fetch blockchain:', err);
    }
  };

  const tamperWithBlock = async (blockIndex) => {
    if (window.confirm(`Are you sure you want to tamper with block ${blockIndex}? This is for demonstration purposes only.`)) {
      try {
        const result = await blockchainAPI.tamperWithBlock(blockIndex);
        if (result.success) {
          setTamperResult(result);
          // Refresh blockchain data
          fetchBlockchain();
          // Re-verify after tampering
          setTimeout(verifyBlockchain, 1000);
        } else {
          setError(result.message);
        }
      } catch (err) {
        setError('Failed to tamper with block');
      }
    }
  };

  useEffect(() => {
    verifyBlockchain();
    fetchBlockchain();
  }, []);

  return (
    <div className="p-6 space-y-6 h-full overflow-auto">
      {/* Header */}
      <div>
        <h2 className="text-2xl font-bold">Blockchain Verification</h2>
        <p className="text-gray-600">Verify the integrity of the blockchain</p>
      </div>

      {/* Error Display */}
      {error && (
        <div className="bg-red-50 border border-red-200 rounded-md p-4">
          <div className="text-red-800">{error}</div>
          <button
            onClick={() => setError(null)}
            className="mt-2 text-red-600 hover:text-red-800 text-sm"
          >
            Dismiss
          </button>
        </div>
      )}

      {/* Tamper Result */}
      {tamperResult && (
        <div className="bg-yellow-50 border border-yellow-200 rounded-md p-4">
          <div className="text-yellow-800">{tamperResult.message}</div>
          <button
            onClick={() => setTamperResult(null)}
            className="mt-2 text-yellow-600 hover:text-yellow-800 text-sm"
          >
            Dismiss
          </button>
        </div>
      )}

      {/* Verification Section */}
      <div className="bg-white rounded-lg shadow p-6">
        <div className="flex justify-between items-center mb-4">
          <h3 className="text-xl font-bold">Blockchain Integrity Check</h3>
          <button
            onClick={verifyBlockchain}
            disabled={loading}
            className="bg-blue-500 text-white px-4 py-2 rounded-lg hover:bg-blue-600 disabled:opacity-50"
          >
            {loading ? 'Verifying...' : 'Verify Blockchain'}
          </button>
        </div>

        {verificationResult && (
          <div className={`p-4 rounded-lg ${
            verificationResult.valid
              ? 'bg-green-50 border border-green-200'
              : 'bg-red-50 border border-red-200'
          }`}>
            <div className="flex items-center space-x-2">
              <div className={`text-2xl ${
                verificationResult.valid ? 'text-green-500' : 'text-red-500'
              }`}>
                {verificationResult.valid ? '✅' : '❌'}
              </div>
              <div>
                <div className={`font-semibold ${
                  verificationResult.valid ? 'text-green-800' : 'text-red-800'
                }`}>
                  {verificationResult.valid ? 'Blockchain is Valid' : 'Blockchain is Invalid'}
                </div>
                <div className={`text-sm ${
                  verificationResult.valid ? 'text-green-600' : 'text-red-600'
                }`}>
                  {verificationResult.message}
                </div>
              </div>
            </div>
          </div>
        )}
      </div>

      {/* Educational Section */}
      <div className="bg-white rounded-lg shadow p-6">
        <h3 className="text-xl font-bold mb-4">Understanding Blockchain Verification</h3>
        <div className="space-y-4 text-sm">
          <div className="bg-blue-50 p-4 rounded-lg">
            <h4 className="font-semibold text-blue-800 mb-2">What does verification check?</h4>
            <ul className="list-disc list-inside space-y-1 text-blue-700">
              <li>Each block's hash is correctly calculated</li>
              <li>Each block references the correct previous block hash</li>
              <li>The merkle root matches the transactions in each block</li>
              <li>The proof-of-work difficulty is satisfied for each block</li>
            </ul>
          </div>

          <div className="bg-purple-50 p-4 rounded-lg">
            <h4 className="font-semibold text-purple-800 mb-2">Why is verification important?</h4>
            <ul className="list-disc list-inside space-y-1 text-purple-700">
              <li>Ensures data integrity across the entire chain</li>
              <li>Detects any tampering or corruption</li>
              <li>Maintains consensus in the network</li>
              <li>Builds trust in the blockchain system</li>
            </ul>
          </div>
        </div>
      </div>

      {/* Tampering Demo Section */}
      <div className="bg-white rounded-lg shadow p-6">
        <h3 className="text-xl font-bold mb-4">Tampering Demo</h3>
        <p className="text-gray-600 mb-4">
          For educational purposes, you can tamper with a block to see how it affects blockchain validity.
        </p>

        {blockchain && blockchain.blocks.length > 0 && (
          <div className="space-y-3">
            <h4 className="font-semibold">Select a block to tamper with:</h4>
            <div className="grid grid-cols-1 md:grid-cols-3 gap-3">
              {blockchain.blocks.map((block) => (
                <div key={block.id} className="border rounded-lg p-3">
                  <div className="flex justify-between items-center">
                    <div>
                      <div className="font-semibold">Block #{block.id}</div>
                      <div className="text-sm text-gray-500">
                        {block.transactionCount} transactions
                      </div>
                    </div>
                    <button
                      onClick={() => tamperWithBlock(block.id)}
                      className="bg-red-500 text-white px-3 py-1 rounded text-sm hover:bg-red-600"
                    >
                      Tamper
                    </button>
                  </div>
                </div>
              ))}
            </div>
            <div className="bg-orange-50 border border-orange-200 rounded-md p-3">
              <div className="text-orange-800 text-sm">
                <strong>Note:</strong> Tampering will modify a transaction in the selected block,
                which will invalidate the blockchain. This demonstrates how blockchain technology
                detects unauthorized changes.
              </div>
            </div>
          </div>
        )}
      </div>

      {/* Blockchain Health Summary */}
      {blockchain && (
        <div className="bg-white rounded-lg shadow p-6">
          <h3 className="text-xl font-bold mb-4">Blockchain Health Summary</h3>
          <div className="grid grid-cols-1 md:grid-cols-3 gap-4">
            <div className="bg-gray-50 p-4 rounded-lg">
              <div className="text-2xl font-bold text-gray-700">{blockchain.chainSize}</div>
              <div className="text-gray-600">Total Blocks</div>
            </div>
            <div className="bg-gray-50 p-4 rounded-lg">
              <div className="text-2xl font-bold text-gray-700">
                {blockchain.blocks.reduce((total, block) => total + block.transactionCount, 0)}
              </div>
              <div className="text-gray-600">Total Transactions</div>
            </div>
            <div className="bg-gray-50 p-4 rounded-lg">
              <div className="text-2xl font-bold text-gray-700">
                {verificationResult ? (verificationResult.valid ? 'Valid' : 'Invalid') : 'Unknown'}
              </div>
              <div className="text-gray-600">Chain Status</div>
            </div>
          </div>
        </div>
      )}
    </div>
  );
}

export default VerificationPanel;
