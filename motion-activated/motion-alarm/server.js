// Save this as server.js and run with: node server.js
const express = require("express");
const app = express();
const port = 3000;

app.get("/", (req, res) => {
  const distance = req.query.distance;
  console.log("Received distance:", distance);
  res.send("Distance logged: " + distance + " cm");
});

app.listen(port, () => {
  console.log(`Server running at http://192.168.0.123:${port}`);
});
