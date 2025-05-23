let sayHelloSync = () => "Hello";
let greeting = sayHelloSync();
console.log(greeting); // 'hello world’ 

let rd = () => Math.random() * 5000;
let sayHello = (v) => new Promise(resolve => {
    var r = rd();
    setTimeout(() => resolve("Hello World " + v + ", t: " + r), r);
});
let sayHelloErr = () => new Promise((resolve, reject) => {
    setTimeout(() => resolve("Hello"), rd() / 5);
    setTimeout(() => reject("Error"), rd() / 5)
});
let addExclamation = (message) => new Promise(
    resolve => setTimeout(() => resolve(message + "!!!"), 1000)
);

var greetingPromise = sayHello(2);
greetingPromise
    .then((greeting) => greeting + '!!!!')
    .then((greeting) => console.log(greeting)); // hello world!!!! 

var greetingPromise = sayHello(3);
greetingPromise
    .then((greeting) => addExclamation(greeting))
    .then((greeting) => console.log(greeting)); // 'hello world!!!!’ 

var greetingPromise = sayHello(4);
greetingPromise
    .then(addExclamation)
    .then((greeting) => console.log(greeting)); // 'hello world!!!!’ 