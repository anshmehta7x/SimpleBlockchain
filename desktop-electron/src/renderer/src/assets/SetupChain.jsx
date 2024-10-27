import { useState } from 'react';

export default function SetupChain({ sendCommand, setShowSetup }) {
    const [difficulty, setDifficulty] = useState(0);
    const [blockSize, setBlockSize] = useState(0);

    const handleDifficulty = (e) => {
        setDifficulty(e.target.value);
    };

    const handleBlockSize = (e) => {
        setBlockSize(e.target.value);
    };

    const handleSubmit = () => {
        sendCommand(`${difficulty}`);
        sendCommand( `${blockSize}`); // Send input to C++ process
        setShowSetup(false); // Close popup after submission
    };

    return (
        <div className="fixed inset-0 bg-gray-500 bg-opacity-75 flex items-center justify-center">
            <div className="bg-white p-6 rounded shadow-lg">
                <h1 className="text-xl font-bold mb-4">Setup Chain</h1>
                <input
                    type="number"
                    placeholder="Difficulty"
                    value={difficulty}
                    onChange={handleDifficulty}
                    className="mb-2 p-2 border rounded w-full"
                />
                <input
                    type="number"
                    placeholder="Block Size"
                    value={blockSize}
                    onChange={handleBlockSize}
                    className="mb-4 p-2 border rounded w-full"
                />
                <button onClick={handleSubmit} className="bg-blue-500 text-white px-4 py-2 rounded">
                    Submit
                </button>
            </div>
        </div>
    );
}
