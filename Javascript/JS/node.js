const fs = require("fs");    // File System Modul 
const path = require("path");   // Pfad Modul 
let enviroment = JSON.stringify(process.env, null, 4); 
let fileName = path.join(process.cwd(), "environment.json"); 
fs.writeFileSync(fileName, enviroment); 