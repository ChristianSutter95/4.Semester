const express = require("express"); 
const Chance = require("chance"); 
 
const app = express(); 
const diceThrower = new Chance(); 
 
const throwDice = (label, aDice, times) => { 
    const results = []; 
    for (let i = 0; i < times; i++) { 
        const result = { type: label, result: aDice() }; 
        results.push(result); 
    } 
    return JSON.stringify(results, null, 4); 
} 
app.get("/", (request, response) => { 
    response.send("Dice Thrower App"); 
}); 
 
app.get("/throw", (request, response) => { 
    const dice = request.query.dice ? request.query.dice : "d20"; 
    const times = request.query.times ? request.query.times : 1; 
    response.send(throwDice(dice, () => diceThrower[dice](), times)); 
}); 
 
app.listen(3000, function () { 
    console.log('Dice app listening on port 3000!'); 
}); 