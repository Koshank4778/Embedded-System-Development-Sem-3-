// Import Express to create the web server.
const express = require("express");

// Create the Express application.
const app = express();

// Set the port used by the website.
const PORT = 3000;

// Enter the Arduino IP address shown in Serial Monitor.
const ARDUINO_IP = "10.152.67.163";

// Serve the files stored inside the public folder.
app.use(express.static("public"));


// Receive room and ON/OFF commands from the webpage.
app.get("/toggle", async (req, res) => {

    // Read the room and state from the webpage request.
    const room = req.query.room;
    const state = req.query.state;

    // List the rooms that can be controlled.
    const allowedRooms = [
        "livingroom",
        "bathroom",
        "closet"
    ];

    // List the valid LED states.
    const allowedStates = [
        "ON",
        "OFF"
    ];

    // Check that the room is valid.
    if (!allowedRooms.includes(room)) {

        return res.status(400).send("Invalid room");
    }

    // Check that the state is valid.
    if (!allowedStates.includes(state)) {

        return res.status(400).send("Invalid state");
    }


    try {

        // Send the room and ON/OFF command to the Arduino.
        const response = await fetch(
            `http://${ARDUINO_IP}/toggle?room=${room}&state=${state}`
        );

        // Read the response from the Arduino.
        const message = await response.text();

        // Send the Arduino response back to the webpage.
        res.send(message);

    }

    catch (error) {

        // Display the connection problem in the terminal.
        console.error("Arduino connection error:", error);

        // Inform the webpage that the Arduino could not be reached.
        res.status(500).send("Arduino connection failed");
    }
});


// Start the Node.js web server.
app.listen(PORT, () => {

    // Display the website address in the terminal.
    console.log(
        `Server running at http://localhost:${PORT}`
    );
});