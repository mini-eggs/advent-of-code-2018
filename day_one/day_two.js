let fs = require("fs");
let path = require("path");

let data = fs.readFileSync(path.join(__dirname, "input.txt")).toString();
let lines = data.split("\n").map(val => parseInt(val));

let sum = 0;
let hash = {};

async function work() {
  for (let line of lines) {
    sum = sum + line;
    if (typeof hash[sum] === "undefined") {
      hash[sum] = 1;
    } else {
      hash[sum]++;
    }

    if (hash[sum] === 2) {
      return sum;
    }
  }

  return await work();
}

work().then(num => console.log(num));
