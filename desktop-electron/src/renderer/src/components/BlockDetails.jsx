import { useState, useEffect } from 'react';

function BlockDetails() {
  const [blockchain, setBlockchain] = useState(null);
  const [selectedBlock, setSelectedBlock] = useState(null);
  const [loading, setLoading] = useState(true);
  const [blockLoading, setBlockLoading] = useState(false);
  const [error, setError] = useState(null);

  const fetchBlockchain = async () => {
    setLoading(true);
    setError(null);
    try {
      const result = await window.api.getBlockchain();
      if (result.success) {
        setBlockchain(result);
        if (result.blocks.length > 0 && !selectedBlock) {
          setSelectedBlock(result.blocks[0]);
        }
      } else {
        setError(result.message);
      }
    } catch (err) {
      setError('Failed to fetch blockchain data');
    } finally {
      setLoading(false);
    }
  };

  const fetchSpecificBlock = async (blockIndex) => {
    setBlockLoading(true);
    try {
      const result = await window.api.getBlock(blockIndex);
      if (result.success) {
        setSelectedBlock(result.block);
      } else {
        setError(result.message);
      }
    } catch (err) {
      setError('Failed to fetch block data');
    } finally {
      setBlockLoading(false);
    }
  };

  useEffect(() => {
    fetchBlockchain();
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
    <div className="flex h-full">
      {/* Block List Sidebar */}
      <div className="w-1/3 border-r bg-white p-4 overflow-y-auto">
        <h2 className="text-lg font-bold mb-4">Blocks</h2>
        <div className="space-y-2">
          {blockchain.blocks.map((block) => (
            <div
              key={block.id}
              onClick={() => fetchSpecificBlock(block.id)}
              className={`p-3 border rounded-lg cursor-pointer transition-colors ${
                selectedBlock?.id === block.id
                  ? 'bg-blue-50 border-blue-200'
                  : 'hover:bg-gray-50'
              }`}
            >
              <div className="font-semibold">Block #{block.id}</div>
              <div className="text-sm text-gray-500">
                {block.transactionCount} transactions
              </div>
              <div className="text-xs font-mono text-gray-400 truncate">
                {block.hash}
              </div>
            </div>
          ))}
        </div>
      </div>

      {/* Block Details */}
      <div className="flex-1 p-6 overflow-y-auto">
        {blockLoading ? (
          <div className="flex justify-center py-8">
            <div className="animate-spin rounded-full h-8 w-8 border-b-2 border-blue-500"></div>
          </div>
        ) : selectedBlock ? (
          <div className="space-y-6">
            {/* Block Header */}
            <div className="bg-white rounded-lg shadow p-6">
              <h2 className="text-2xl font-bold mb-4">Block #{selectedBlock.id}</h2>
              <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
                <div>
                  <label className="block text-sm font-medium text-gray-700 mb-1">
                    Block Hash
                  </label>
                  <div className="p-2 bg-gray-50 rounded font-mono text-sm break-all">
                    {selectedBlock.hash}
                  </div>
                </div>
                <div>
                  <label className="block text-sm font-medium text-gray-700 mb-1">
                    Previous Hash
                  </label>
                  <div className="p-2 bg-gray-50 rounded font-mono text-sm break-all">
                    {selectedBlock.prevHash === "0" ? "Genesis Block" : selectedBlock.prevHash}
                  </div>
                </div>
                <div>
                  <label className="block text-sm font-medium text-gray-700 mb-1">
                    Merkle Root
                  </label>
                  <div className="p-2 bg-gray-50 rounded font-mono text-sm break-all">
                    {selectedBlock.merkleRoot}
                  </div>
                </div>
                <div>
                  <label className="block text-sm font-medium text-gray-700 mb-1">
                    Timestamp
                  </label>
                  <div className="p-2 bg-gray-50 rounded text-sm">
                    {new Date(selectedBlock.timestamp * 1000).toLocaleString()}
                  </div>
                </div>
                <div>
                  <label className="block text-sm font-medium text-gray-700 mb-1">
                    Nonce
                  </label>
                  <div className="p-2 bg-gray-50 rounded text-sm">
                    {selectedBlock.nonce}
                  </div>
                </div>
                <div>
                  <label className="block text-sm font-medium text-gray-700 mb-1">
                    Transaction Count
                  </label>
                  <div className="p-2 bg-gray-50 rounded text-sm">
                    {selectedBlock.transactionCount}
                  </div>
                </div>
              </div>
            </div>

            {/* Transactions */}
            <div className="bg-white rounded-lg shadow">
              <div className="p-6 border-b">
                <h3 className="text-xl font-bold">Transactions in this Block</h3>
              </div>
              <div className="p-6">
                {selectedBlock.transactions.length === 0 ? (
                  <div className="text-center text-gray-500 py-8">
                    No transactions in this block
                  </div>
                ) : (
                  <div className="space-y-4">
                    {selectedBlock.transactions.map((tx, index) => (
                      <div key={index} className="border rounded-lg p-4">
                        <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
                          <div>
                            <label className="block text-sm font-medium text-gray-700 mb-1">
                              Transaction Hash
                            </label>
                            <div className="p-2 bg-gray-50 rounded font-mono text-sm break-all">
                              {tx.hash}
                            </div>
                          </div>
                          <div>
                            <label className="block text-sm font-medium text-gray-700 mb-1">
                              Timestamp
                            </label>
                            <div className="p-2 bg-gray-50 rounded text-sm">
                              {new Date(tx.timestamp * 1000).toLocaleString()}
                            </div>
                          </div>
                          <div>
                            <label className="block text-sm font-medium text-gray-700 mb-1">
                              From
                            </label>
                            <div className="p-2 bg-gray-50 rounded text-sm">
                              {tx.sender}
                            </div>
                          </div>
                          <div>
                            <label className="block text-sm font-medium text-gray-700 mb-1">
                              To
                            </label>
                            <div className="p-2 bg-gray-50 rounded text-sm">
                              {tx.receiver}
                            </div>
                          </div>
                          <div className="md:col-span-2">
                            <label className="block text-sm font-medium text-gray-700 mb-1">
                              Amount
                            </label>
                            <div className="p-2 bg-green-50 rounded text-sm font-semibold text-green-700">
                              {tx.amount}
                            </div>
                          </div>
                        </div>
                      </div>
                    ))}
                  </div>
                )}
              </div>
            </div>
          </div>
        ) : (
          <div className="text-center text-gray-500 py-8">
            Select a block to view its details
          </div>
        )}
      </div>
    </div>
  );
}

export default BlockDetails;