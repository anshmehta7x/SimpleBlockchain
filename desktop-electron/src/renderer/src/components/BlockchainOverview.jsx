import { useState, useEffect } from 'react';
import { blockchainAPI } from '../utils/api';

function BlockchainOverview() {
  const [blockchain, setBlockchain] = useState(null);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState(null);

  const fetchBlockchain = async () => {
    setLoading(true);
    setError(null);
    try {
      const result = await blockchainAPI.getBlockchain();
      if (result.success) {
        setBlockchain(result);
      } else {
        setError(result.message);
      }
    } catch (err) {
      setError('Failed to fetch blockchain data');
    } finally {
      setLoading(false);
    }
  };

  useEffect(() => {
    fetchBlockchain();
    const interval = setInterval(fetchBlockchain, 5000); // Refresh every 5 seconds
    return () => clearInterval(interval);
  }, []);

  if (loading) {
    return (
      <div className="p-6 flex justify-center">
        <div className="animate-spin rounded-full h-12 w-12 border-b-2 border-blue-500"></div>
      </div>
    );
  }

  if (error) {
    return (
      <div className="p-6">
        <div className="bg-red-50 border border-red-200 rounded-md p-4">
          <div className="text-red-800">Error: {error}</div>
          <button
            onClick={fetchBlockchain}
            className="mt-2 bg-red-500 text-white px-3 py-1 rounded text-sm hover:bg-red-600"
          >
            Retry
          </button>
        </div>
      </div>
    );
  }

  return (
    <div className="p-6 space-y-6 overflow-auto h-full">
      {/* Chain Statistics */}
      <div className="bg-white rounded-lg shadow p-6">
        <h2 className="text-xl font-bold mb-4">Blockchain Statistics</h2>
        <div className="grid grid-cols-1 md:grid-cols-3 gap-4">
          <div className="bg-blue-50 p-4 rounded-lg">
            <div className="text-2xl font-bold text-blue-600">{blockchain.chainSize}</div>
            <div className="text-blue-800">Total Blocks</div>
          </div>
          <div className="bg-green-50 p-4 rounded-lg">
            <div className="text-2xl font-bold text-green-600">
              {blockchain.blocks.reduce((total, block) => total + block.transactionCount, 0)}
            </div>
            <div className="text-green-800">Total Transactions</div>
          </div>
          <div className="bg-purple-50 p-4 rounded-lg">
            <div className="text-2xl font-bold text-purple-600">
              {blockchain.blocks[0]?.hash.match(/^0+/)?.[0]?.length || 0}
            </div>
            <div className="text-purple-800">Mining Difficulty</div>
          </div>
        </div>
      </div>

      {/* Block List */}
      <div className="bg-white rounded-lg shadow">
        <div className="p-6 border-b">
          <h2 className="text-xl font-bold">Block Chain</h2>
          <p className="text-gray-600">All blocks in the blockchain</p>
        </div>
        <div className="space-y-4 p-6">
          {blockchain.blocks.map((block, index) => (
            <div key={block.id} className="border rounded-lg p-4 hover:bg-gray-50">
              <div className="flex justify-between items-start mb-2">
                <div>
                  <h3 className="font-semibold text-lg">Block #{block.id}</h3>
                  <p className="text-sm text-gray-500">
                    {new Date(block.timestamp * 1000).toLocaleString()}
                  </p>
                </div>
                <div className="text-right">
                  <div className="text-sm text-gray-500">Nonce: {block.nonce}</div>
                  <div className="text-sm text-gray-500">{block.transactionCount} transactions</div>
                </div>
              </div>

              <div className="space-y-2 text-sm">
                <div>
                  <span className="font-medium">Hash:</span>
                  <span className="ml-2 font-mono text-blue-600 break-all">{block.hash}</span>
                </div>
                {block.prevHash !== "0" && (
                  <div>
                    <span className="font-medium">Previous Hash:</span>
                    <span className="ml-2 font-mono text-gray-600 break-all">{block.prevHash}</span>
                  </div>
                )}
                <div>
                  <span className="font-medium">Merkle Root:</span>
                  <span className="ml-2 font-mono text-green-600 break-all">{block.merkleRoot}</span>
                </div>
              </div>

              {/* Mini Transaction List */}
              {block.transactions.length > 0 && (
                <div className="mt-3 pt-3 border-t">
                  <h4 className="font-medium mb-2">Transactions:</h4>
                  <div className="space-y-1">
                    {block.transactions.slice(0, 3).map((tx, txIndex) => (
                      <div key={txIndex} className="text-xs bg-gray-50 p-2 rounded">
                        <span className="font-medium">{tx.sender}</span> →
                        <span className="font-medium"> {tx.receiver}</span>
                        <span className="text-green-600 ml-2">{tx.amount}</span>
                      </div>
                    ))}
                    {block.transactions.length > 3 && (
                      <div className="text-xs text-gray-500">
                        ... and {block.transactions.length - 3} more
                      </div>
                    )}
                  </div>
                </div>
              )}
            </div>
          ))}
        </div>
      </div>
    </div>
  );
}

export default BlockchainOverview;
