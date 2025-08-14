import { useState, useEffect } from 'react';

function TransactionManager() {
  const [transactions, setTransactions] = useState([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState(null);
  const [editingTx, setEditingTx] = useState(null);
  const [showAddForm, setShowAddForm] = useState(false);
  
  // Form state
  const [formData, setFormData] = useState({
    sender: '',
    receiver: '',
    amount: ''
  });

  const fetchTransactions = async () => {
    setLoading(true);
    setError(null);
    try {
      const result = await window.api.getTransactions();
      if (result.success) {
        setTransactions(result.transactions);
      } else {
        setError(result.message);
      }
    } catch (err) {
      setError('Failed to fetch transactions');
    } finally {
      setLoading(false);
    }
  };

  const handleAddTransaction = async (e) => {
    e.preventDefault();
    try {
      const result = await window.api.addTransaction(
        formData.sender,
        formData.receiver,
        parseFloat(formData.amount)
      );
      if (result.success) {
        setFormData({ sender: '', receiver: '', amount: '' });
        setShowAddForm(false);
        fetchTransactions();
      } else {
        setError(result.message);
      }
    } catch (err) {
      setError('Failed to add transaction');
    }
  };

  const handleEditTransaction = async (e) => {
    e.preventDefault();
    try {
      const result = await window.api.editTransaction(
        editingTx.hash,
        formData.sender,
        formData.receiver,
        parseFloat(formData.amount)
      );
      if (result.success) {
        setEditingTx(null);
        setFormData({ sender: '', receiver: '', amount: '' });
        fetchTransactions();
      } else {
        setError(result.message);
      }
    } catch (err) {
      setError('Failed to edit transaction');
    }
  };

  const handleRemoveTransaction = async (hash) => {
    if (window.confirm('Are you sure you want to remove this transaction?')) {
      try {
        const result = await window.api.removeTransaction(hash);
        if (result.success) {
          fetchTransactions();
        } else {
          setError(result.message);
        }
      } catch (err) {
        setError('Failed to remove transaction');
      }
    }
  };

  const startEdit = (tx) => {
    setEditingTx(tx);
    setFormData({
      sender: tx.sender,
      receiver: tx.receiver,
      amount: tx.amount.toString()
    });
    setShowAddForm(false);
  };

  const cancelEdit = () => {
    setEditingTx(null);
    setFormData({ sender: '', receiver: '', amount: '' });
  };

  const startAdd = () => {
    setShowAddForm(true);
    setEditingTx(null);
    setFormData({ sender: '', receiver: '', amount: '' });
  };

  useEffect(() => {
    fetchTransactions();
    const interval = setInterval(fetchTransactions, 5000);
    return () => clearInterval(interval);
  }, []);

  if (loading) {
    return (
      <div className="p-6 flex justify-center">
        <div className="animate-spin rounded-full h-12 w-12 border-b-2 border-blue-500"></div>
      </div>
    );
  }

  return (
    <div className="p-6 space-y-6 h-full overflow-auto">
      {/* Header with Add Button */}
      <div className="flex justify-between items-center">
        <div>
          <h2 className="text-2xl font-bold">Transaction Manager</h2>
          <p className="text-gray-600">Manage transactions in the pool</p>
        </div>
        <button
          onClick={startAdd}
          className="bg-blue-500 text-white px-4 py-2 rounded-lg hover:bg-blue-600"
        >
          Add Transaction
        </button>
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

      {/* Add/Edit Form */}
      {(showAddForm || editingTx) && (
        <div className="bg-white rounded-lg shadow p-6">
          <h3 className="text-lg font-bold mb-4">
            {editingTx ? 'Edit Transaction' : 'Add New Transaction'}
          </h3>
          <form onSubmit={editingTx ? handleEditTransaction : handleAddTransaction}>
            <div className="grid grid-cols-1 md:grid-cols-3 gap-4 mb-4">
              <div>
                <label className="block text-sm font-medium text-gray-700 mb-1">
                  Sender
                </label>
                <input
                  type="text"
                  value={formData.sender}
                  onChange={(e) => setFormData({ ...formData, sender: e.target.value })}
                  className="w-full p-2 border border-gray-300 rounded-md"
                  required
                />
              </div>
              <div>
                <label className="block text-sm font-medium text-gray-700 mb-1">
                  Receiver
                </label>
                <input
                  type="text"
                  value={formData.receiver}
                  onChange={(e) => setFormData({ ...formData, receiver: e.target.value })}
                  className="w-full p-2 border border-gray-300 rounded-md"
                  required
                />
              </div>
              <div>
                <label className="block text-sm font-medium text-gray-700 mb-1">
                  Amount
                </label>
                <input
                  type="number"
                  step="0.01"
                  min="0"
                  value={formData.amount}
                  onChange={(e) => setFormData({ ...formData, amount: e.target.value })}
                  className="w-full p-2 border border-gray-300 rounded-md"
                  required
                />
              </div>
            </div>
            <div className="flex space-x-2">
              <button
                type="submit"
                className="bg-green-500 text-white px-4 py-2 rounded hover:bg-green-600"
              >
                {editingTx ? 'Update' : 'Add'} Transaction
              </button>
              <button
                type="button"
                onClick={editingTx ? cancelEdit : () => setShowAddForm(false)}
                className="bg-gray-500 text-white px-4 py-2 rounded hover:bg-gray-600"
              >
                Cancel
              </button>
            </div>
          </form>
        </div>
      )}

      {/* Transaction Pool */}
      <div className="bg-white rounded-lg shadow">
        <div className="p-6 border-b">
          <h3 className="text-xl font-bold">Transaction Pool</h3>
          <p className="text-gray-600">
            {transactions.length} transaction{transactions.length !== 1 ? 's' : ''} waiting to be mined
          </p>
        </div>
        <div className="p-6">
          {transactions.length === 0 ? (
            <div className="text-center text-gray-500 py-8">
              No transactions in the pool
            </div>
          ) : (
            <div className="space-y-4">
              {transactions.map((tx, index) => (
                <div key={index} className="border rounded-lg p-4">
                  <div className="flex justify-between items-start mb-4">
                    <div className="flex-1">
                      <div className="grid grid-cols-1 md:grid-cols-3 gap-4">
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
                        <div>
                          <label className="block text-sm font-medium text-gray-700 mb-1">
                            Amount
                          </label>
                          <div className="p-2 bg-green-50 rounded text-sm font-semibold text-green-700">
                            {tx.amount}
                          </div>
                        </div>
                      </div>
                      <div className="mt-4">
                        <label className="block text-sm font-medium text-gray-700 mb-1">
                          Transaction Hash
                        </label>
                        <div className="p-2 bg-gray-50 rounded font-mono text-xs break-all">
                          {tx.hash}
                        </div>
                      </div>
                      <div className="mt-2">
                        <label className="block text-sm font-medium text-gray-700 mb-1">
                          Timestamp
                        </label>
                        <div className="text-sm text-gray-500">
                          {new Date(tx.timestamp * 1000).toLocaleString()}
                        </div>
                      </div>
                    </div>
                    <div className="ml-4 flex flex-col space-y-2">
                      <button
                        onClick={() => startEdit(tx)}
                        className="bg-yellow-500 text-white px-3 py-1 rounded text-sm hover:bg-yellow-600"
                      >
                        Edit
                      </button>
                      <button
                        onClick={() => handleRemoveTransaction(tx.hash)}
                        className="bg-red-500 text-white px-3 py-1 rounded text-sm hover:bg-red-600"
                      >
                        Remove
                      </button>
                    </div>
                  </div>
                </div>
              ))}
            </div>
          )}
        </div>
      </div>
    </div>
  );
}

export default TransactionManager;