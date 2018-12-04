let { readFileSync } = require("fs");
let { join } = require("path");

let input = readFileSync(join(__dirname, "input.txt"), "utf-8");
let lines = input.split("\n");

let range = (x, y) => Array.from({ length: y }).map((_, index) => index + x);

let transform = line => {
  let parts = line
    .replace("#", "")
    .replace("@ ", "")
    .replace(":", "")
    .split(" ");
  let num = parseInt(parts[0]);
  let [x, y] = parts[1].split(",").map(val => parseInt(val));
  let [width, height] = parts[2].split("x").map(val => parseInt(val));
  return { num, x, y, width, height };
};

let howManyItemsAreWithinClaimsByX = (x, lines) => {
  let hash = {};
  for (let line of lines) {
    let { x, y, width, height } = transform(line);
    for (let posX of range(x, width)) {
      for (let posY of range(y, height)) {
        let key = `${posX}:${posY}`;
        if (typeof hash[key] === "undefined") {
          hash[key] = 1;
        } else {
          hash[key]++;
        }
      }
    }
  }
  return {
    val: Object.keys(hash).reduce((total, key) => (hash[key] >= x ? total + 1 : total), 0),
    hash
  };
};

let firstClaimToNotOverlap = (hash, lines) => {
  for (let line of lines) {
    let { num, x, y, width, height } = transform(line);
    let doesOverlap = false;

    for (let posX of range(x, width)) {
      for (let posY of range(y, height)) {
        let key = `${posX}:${posY}`;

        if (hash[key] > 1) {
          doesOverlap = true;
        }
      }
    }

    if (!doesOverlap) {
      return num;
    }
  }
};

let { val, hash } = howManyItemsAreWithinClaimsByX(2, lines);

console.log(val); // part one
console.log(firstClaimToNotOverlap(hash, lines)); // part two
