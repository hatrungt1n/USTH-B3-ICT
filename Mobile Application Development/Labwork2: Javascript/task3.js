function check(inputNum) {
  var randomNum = Math.floor(Math.random() * 10 + 1);

  if (inputNum == randomNum) {
    alert("Good Work");
  } else {
    alert("Not matched");
    // alert("Not matched. The right result is " + randomNum);
  }
}

const inputNum = prompt("Enter your number: ");

check(inputNum);