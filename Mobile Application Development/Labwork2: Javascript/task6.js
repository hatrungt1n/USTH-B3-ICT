function happy_number(num) {
  var m, n;
  var c = [];

  while (num != 1 && c[num] !== true) {
    c[num] = true;
    m = 0;
    while (num > 0) {
      n = num % 10;
      m += n * n;
      num = (num - n) / 10;
    }
    num = m;
  }
  return num == 1;
}

var cnt = 10;
var num = 1;
var f10 = "";
while (cnt-- > 0) {
  while (!happy_number(num)) num++;
  f10 = f10 + (num + ", ");

  num++;
}
console.log("First 10 happy numbers are: " + f10);
