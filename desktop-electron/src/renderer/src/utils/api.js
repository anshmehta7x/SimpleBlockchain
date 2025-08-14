// API utilities for blockchain operations with enhanced debugging
const API_BASE_URL = 'http://127.0.0.1:3001/api';

const apiCall = async (endpoint, options = {}) => {
  const fullUrl = `${API_BASE_URL}${endpoint}`;
  console.log(`🌐 Making API call to: ${fullUrl}`, options);

  try {
    const response = await fetch(fullUrl, {
      headers: {
        'Content-Type': 'application/json',
        ...options.headers
      },
      mode: 'cors', // Explicitly set CORS mode
      ...options
    });

    console.log(`📡 Response status: ${response.status} for ${fullUrl}`);

    if (!response.ok) {
      const errorText = await response.text();
      console.error(`❌ API Error ${response.status}:`, errorText);
      throw new Error(`HTTP ${response.status}: ${response.statusText} - ${errorText}`);
    }

    const data = await response.json();
    console.log(`✅ API call successful for ${fullUrl}:`, data);
    return data;
  } catch (error) {
    console.error(`💥 API call failed for ${fullUrl}:`, error);

    // Check if it's a network error
    if (error.message.includes('Failed to fetch') || error.name === 'TypeError') {
      console.error('🔍 Network error detected. Possible causes:');
      console.error('   1. C++ backend not running on port 3001');
      console.error('   2. CORS issues');
      console.error('   3. Network connectivity problems');

      return {
        success: false,
        message: `Network error: ${error.message}. Check if C++ backend is running on port 3001.`
      };
    }

    return { success: false, message: error.message };
  }
};

export const blockchainAPI = {
  // Get the entire blockchain
  getBlockchain: async () => {
    console.log('🔗 Fetching blockchain...');
    return await apiCall('/blockchain');
  },

  // Get a specific block by index
  getBlock: async (index) => {
    console.log(`🧱 Fetching block ${index}...`);
    return await apiCall(`/block/${index}`);
  },

  // Get all transactions in the pool
  getTransactions: async () => {
    console.log('📝 Fetching transactions...');
    return await apiCall('/transactions');
  },

  // Add a new transaction
  addTransaction: async (sender, receiver, amount) => {
    console.log(`➕ Adding transaction: ${sender} → ${receiver} (${amount})`);
    return await apiCall('/transaction', {
      method: 'POST',
      body: JSON.stringify({ sender, receiver, amount })
    });
  },

  // Edit an existing transaction
  editTransaction: async (hash, sender, receiver, amount) => {
    console.log(`✏️ Editing transaction ${hash}`);
    return await apiCall(`/transaction/${hash}`, {
      method: 'PUT',
      body: JSON.stringify({ sender, receiver, amount })
    });
  },

  // Remove a transaction
  removeTransaction: async (hash) => {
    console.log(`🗑️ Removing transaction ${hash}`);
    return await apiCall(`/transaction/${hash}`, {
      method: 'DELETE'
    });
  },

  // Verify the blockchain
  verifyBlockchain: async () => {
    console.log('🔍 Verifying blockchain...');
    return await apiCall('/verify');
  },

  // Initialize blockchain with custom parameters
  initializeBlockchain: async (difficulty, blockSize) => {
    console.log(`🚀 Initializing blockchain (difficulty: ${difficulty}, blockSize: ${blockSize})`);
    return await apiCall('/initialize', {
      method: 'POST',
      body: JSON.stringify({ difficulty, blockSize })
    });
  },

  // Tamper with a block (for testing)
  tamperWithBlock: async (blockIndex) => {
    console.log(`🔨 Tampering with block ${blockIndex}`);
    return await apiCall('/tamper', {
      method: 'POST',
      body: JSON.stringify({ blockIndex })
    });
  },

  // Save blockchain to file
  saveBlockchain: async (filename) => {
    console.log(`💾 Saving blockchain to ${filename}`);
    return await apiCall('/save', {
      method: 'POST',
      body: JSON.stringify({ filename })
    });
  },

  // Load blockchain from file
  loadBlockchain: async (filename) => {
    console.log(`📂 Loading blockchain from ${filename}`);
    return await apiCall('/load', {
      method: 'POST',
      body: JSON.stringify({ filename })
    });
  },

  // Test connection (utility function)
  testConnection: async () => {
    console.log('🔧 Testing connection to C++ backend...');
    try {
      const result = await apiCall('/blockchain');
      console.log('✅ Connection test passed!');
      return { success: true, message: 'Connection successful' };
    } catch (error) {
      console.error('❌ Connection test failed:', error);
      return { success: false, message: error.message };
    }
  }
};
