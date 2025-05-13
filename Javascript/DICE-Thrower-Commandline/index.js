const argv = require("yargs").argv  // Load command line parses 
const Chance = require("chance");   // Load randomness library 
const dice = new Chance(); 
const throwDice = (label, aDice, times) => { 
    for (let i = 0; i < times; i++) { 
        console.log(`${label}: ${aDice()}`); 
    } 
} 
if (argv.d4) throwDice("d4", () => dice.d4(), argv.d4); 
if (argv.d6) throwDice("d6", () => dice.d6(), argv.d6); 
if (argv.d8) throwDice("d8", () => dice.d8(), argv.d8); 
if (argv.d10) throwDice("d10", () => dice.d10(), argv.d10); 
if (argv.d12) throwDice("d12", () => dice.d12(), argv.d12); 
if (argv.d20) throwDice("d20", () => dice.d20(), argv.d20); 