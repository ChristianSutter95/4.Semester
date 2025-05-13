// Var vs let -> var
/*
var people = ["Frodo", "Aragon", "Legoals", "Gimli"] 
var sandwiches = ["Cheese", "Chicken", "Tomato", "Cheese", "Cucumber"] 
var count = people.length; 
var enoughFood = count < sandwiches.length; 
if (enoughFood) { 
    var count = sandwiches.length; 
    console.log("We have " + count + " sandwiches for everyone. Plenty for all!"); 
    } 
console.log("We have " + count + " people and " + sandwiches.length + 
    " sandwiches!"); 
*/
// var vs. let -> let
let people = ["Frodo", "Aragon", "Legoals", "Gimli"] 
let sandwiches = ["Cheese", "Chicken", "Tomato", "Cheese", "Cucumber"] 
let count = people.length; 
let enoughFood = count < sandwiches.length; 
if (enoughFood) { 
    let count = sandwiches.length; // accidentally overriding the count letiable 
    console.log("We have " + count + " sandwiches for everyone. Plenty for all!"); 
} 
// our count letiable is no longer accurate 
console.log("We have " + count + " people and " + sandwiches.length + " sandwiches!"); 

// Promises
let rd = () =>  Math.random() * 5000; 
let sayHello = (v) => new Promise(resolve => { 
    var r=rd();  
    setTimeout(() => resolve("Hello World " + v + ", t: " + r), r); 
}); 
let sayHelloErr = () => new Promise((resolve, reject) => { 
    setTimeout(()=> resolve("Hello"), rd()/5); 
    setTimeout(() => reject("Error"), rd()/5) 
}); 
let addExclamation = (message) => new Promise( 
    resolve => setTimeout(() => resolve(message + "!!!"), 1000) 
); 

let greetingPromise = sayHello(0); 
greetingPromise.then(function (greeting) { 
console.log(greeting); // 'hello world’ 
}); 
console.log("Waiting..."); 