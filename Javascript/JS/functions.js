// Funktion
function hello () { 
    var hello = 'Hallo Welt'; 
    console.log(hello); 
} 
hello();

// anonyme Funktion
let anonymFunction = function(){
    console.log("Hallo Welt");
}

anonymFunction();

// Closure
var greeting = function (x) { 
    return function () { 
    console.log(x); 
    } 
} 
var hello = greeting('Hallo'); 
var world = greeting('Welt'); 
hello(); world(); 

// Müesli Rezept - Imperativ
var base = [ "Milch", "Naturjoghurt ", "Sojamilch" ]; 
var ingredients = [ "Haferflocken", "Buchweizen", "Chiasamen", "Dinkelflocken", 
                    "Gerstenflocken", "Leinsamen", "Mohnsamen", "Sesam", "Weizenflocken" ]; 
var extras = [ "Bananen", "Erdbeeren", "Schokolade", "Haselnüsse", "Honig" ]; 

var random = function (array) { 
    return array[ Math.floor( Math.random() * array.length ) ]; 
} 
console.log("Basis:\n"); 
console.log("* ", random(base), "\n"); 
console.log("Zutaten:\n"); 
var taken = []; 
while (taken.length < 5) { 
    var choice = random(ingredients); 
    if (taken.indexOf(choice) === -1) { 
        taken.push(choice); 
        console.log("* ", choice, "\n"); 
    } 
} 
console.log("Extras:\n"); 
console.log("* ", random(extras), "\n");

// Müesli-Rezept - Funktional
function shuffle(array) { 
    var index = 0; 
    var shuffled = []; 
    array.forEach(function (value) { 
        var random = Math.floor( Math.random() * index++ ); 
        shuffled[index - 1] = shuffled[random]; 
        shuffled[random] = value; 
    }); 
    return shuffled; 
} 
var random_sample = function (n, array) { 
    return shuffle(array).slice(0, n); 
}; 
var pick = function (array) { 
    return function (n) { 
        return random_sample( n, array ) 
            .forEach( function (a) { 
                console.log("* ", a, "\n"); 
            }); 
    }; 
}; 
     
console.log("Basis:\n"); 
pick(base)(1); 
console.log("Zutaten:\n"); 
pick(ingredients)(5); 
console.log("Extras:\n"); 
pick(extras)(2); 