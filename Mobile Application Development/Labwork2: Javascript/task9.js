const secondLowest = arr => arr.sort((a, b) => a - b)[1]

const secondLargest = arr => arr.sort((a, b) => b - a)[1]

let arr = [10, 20, 30, 40, 50]

console.log(`second lowest: ${secondLowest(arr)}\nSecond largest: ${secondLargest(arr)}`)