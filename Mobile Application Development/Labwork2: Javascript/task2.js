function cToF(celsius) {
  var cToFahr = (celsius * 1.8 + 32).toFixed(2);
  console.log(
    `${celsius} degree celsius is equal to ${cToFahr} degree fahrenheit.`
  );
}

function fToC(fahrenheit) {
  const fToCel = ((fahrenheit - 32) / 1.8).toFixed(2);
  console.log(
    `${fahrenheit} degree fahrenheit is equal to ${fToCel} degree celsius.`
  );
}

cToF(36)
fToC(55)
