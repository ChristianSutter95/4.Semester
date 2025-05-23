var variable = "John"; 
var obj1 = { }; 
var obj2 = { 
    firstName : "John", 
    lastName : "Smith", 
    age : 50 
}; 

var obj3 = new Object( ); 

obj3.firstName = "John"; 
obj3.lastName = "Smith"; 
obj3.age = 50; 
console.log(obj3); 

// Prototype
function Person( first, last, age) { 
    this.firstName = first; 
    this.lastName = last; 
    this.age = age; 
};
Person.prototype.nationality = "English"; 
Person.prototype.changeLastName = function (name) { 
this.lastName = name; 
};

var student = new Person ( "Jack", "Doe", 25 ); 

student.changeLastName("Sutter");
console.log(student);

// Vererbung
function Person1( name ) { 
    this.name = name; 
} 
Person1.prototype.sayHello = function ( ) { 
 return "Hello, my name is " + this.name; 
}; 
function Student( name, subject ) { 
    Person1.call( this, name ); 
    this.subject = subject; 
} 
Student.prototype.sayHello = function ( ) { 
    return Person1.prototype.sayHello.call( this ) + " and I’m a " + this.subject + " student."; 
};

var student1 = new Student("Chrigi", "Systemtechnik");
console.log(student1.sayHello()); 