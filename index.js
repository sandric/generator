const express = require('express');
const axios = require('axios');
const path = require('path');
const app = express();
const PORT = 3000;

let generatorIP = null;
let generatorPingTimestamp = null;
let lastCommand = null;

const isPingStalled = () => {
    const difference = Math.abs(Date.now() - generatorPingTimestamp);
    return difference > 5000;
}

app.use(express.static(path.join(__dirname, 'public')));

app.get('/generators/0', (req, res) => {
    res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

app.get('/generators/0/ping', async (req, res) => {
    console.log('ping received');

    generatorIP = req.query.ip;
    generatorPingTimestamp = Date.now();

    console.log(generatorIP);

    let status = 200;
    if(lastCommand == 'ON') status = 201;
    else if(lastCommand == 'OFF') status = 202;

    lastCommand = null

    res.sendStatus(status);
});

app.get('/generators/0/ip', async (req, res) => {
    if(isPingStalled()) generatorIP = null

    res.send(generatorIP)
});

app.get('/generators/0/on', async (req, res) => {
    console.log('Button on clicked');

    lastCommand = 'ON';

    res.sendStatus(200);
});

app.get('/generators/0/off', async (req, res) => {
    console.log('Button off clicked');

    lastCommand = 'OFF';

    res.sendStatus(200);
});

app.listen(PORT, () => {
  console.log(`lil' slutty server is up on http://localhost:${PORT}`);
});
