

function hello() {                  // Funktion
    console.log('Hallo Welt');
} // hello();
/*-----------------------------------------------------*/
var hello = function () {           // Anonyme Funktion
    console.log('Hallo Welt');
} // hello();
/*-----------------------------------------------------*/
var greeting = function (x) {       // Closure
    return function () {
        console.log(x);
    }
}
var hello = greeting('Hallo'); // hello();
/*-----------------------------------------------------*/
(function () { var x = "Hello!!"; })(); // Self-Invoking



var obj1 = {}; // leeres Objekt
var obj2 = {
    firstName: "John",
    age: 50
};
// obj2 und obj3 genau gleich
var obj3 = new Object();
obj3.firstName = "John";
obj3.age = 50;
/*-----------------------------------------------------*/
obj2.firstName = "Jack";                // Zugriff
obj2.nationality = "swiss"; // neue Property
delete obj2.age;    // Property löschen
/*-----------------------------------------------------*/
console.log(obj2);                      // Ausgaben
console.log(obj2.firstName);
console.log(obj2["firstName"]);

// Schnelle Variante        // Built-In Konstruktor
var x1 = {}; new Object();
var x2 = ""; new String();
var x3 = 0; new Number();
var x4 = false; new Boolean();
var x5 = []; new Array();
var x6 = /( )/; new RegExp();
var x7 = function () { }; new Function();

// Verschatelung und Methoden


var obj = {
    first: "John",
    last: "Smith",
    details: {
        nationality: "Swiss",
        age: 50
    },
    fullName: function () {
        return this.first + " " + this.last;
    }
};

obj.details.color;                  // Ausgabe: orange
console.log(obj["details"]["age"]); // Ausgabe: 50

console.log(obj.fullName());       // Ausgabe: John Smith 
console.log(obj.fullName);
// Ausgabe: function() {return this.first + " " + this.last;}


function Person(first, last, age) { // Object constructor
    this.firstName = first;         // function
    this.lastName = last;
    this.age = age;
}
/*-----------------------------------------------------*/
Person.prototype.nationality = "English";
Person.prototype.changeLastName = function (name) {
    this.lastName = name;
};
/*-----------------------------------------------------*/
var student = new Person("Jack", "Doe", 25);
student.changeLastName("Miller");

// Vererbung

function Person(name) {
    this.name = name;
}
Person.prototype.sayHello = function () {
    return "Hello, my name is " + this.name;
};
function Student(name, subject) {
    Person.call(this, name);
    this.subject = subject;
}
Student.prototype.sayHello = function () {
    return Person.prototype.sayHello.call(this) +
        " and I’m a " + this.subject + " student.";
};


class Shape {
    constructor(id, x, y) {
        this.id = id;
        this.move(x, y);
    }
    move(x, y) {
        this.x = x;
        this.y = y;
    }
}

class Base {
    constructor(id) {
        this.id = id;
    }
}
class Person extends Base {
    constructor(name, id) {
        super(id);
        this.name = name;
    }
}

let double = a => a + a; // 1 Var.
let printHelloWorld = () => console.log("Hello World"); // 0 Var.
let add = (a, b) => a + b; // 2 Var.
let add2 = (a, b) => { // 2. Var. mit bestimmten Rückgabewert
    let value = a + b;
    return value;
}
let a = 2;
let addTwo = b => b + a; // Var. a von ausserhalb sichtbar

console.log(double(5)); // Aufruf









import React from "react";
class SimpleClassComponent extends React.Component {
    render() {
        return (<div>Hello, {this.props.greet}</div>);
    }
}
//export default SimpleClassComponent;

function SimpleFunctionalComponent(props) {
    return (<div>Hello, {props.greet}</div>);
}
export default SimpleFunctionalComponent;
<SimpleComponent greet={"World"} /> 


const [state, setState] = useState("InitialState"); 

