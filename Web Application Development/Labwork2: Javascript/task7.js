let revNum = 0;
let num = 8375;
let lastDigit;
let num2 = num;

while (num2 != 0) {
  lastDigit = num2 % 10;
  revNum = revNum * 10 + lastDigit;
  num2 = Math.floor(num2 / 10);
}

console.log(`The reverse number of ${num} is ${revNum}`);
