var regularExpression= /(?<=\[).*?(?=\])/g;
var getTheValueWithIndex= "[John Smith][David Miller]".match(regularExpression);
console.log("The first value without bracket="+getTheValueWithIndex[0]);
console.log("The second value without bracket="+getTheValueWithIndex[1]);