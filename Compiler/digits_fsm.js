var state = 0;
var str = process.argv[2];
var pos = 0;
var flag = true;

function f0(c){
	if(/[0-9]/.test(c)){
		console.log('%s is digit:in 0',c);
		state = 1;
		return;
	}
	flag = false;
}
function f1(c){
	if(/[0-9]/.test(c)){
		console.log('%s is digit',c);
		state = 1;
		return;
	}
	if(c=='.'){
		console.log('is a dot');
		state = 2;
		return;
	}
	if(c=='E'){
		console.log('is an E');
		state = 4;
		return;
	}
	flag == false;
}

function f2(c){
	if(/[0-9]/.test(c)){
		state = 3;
		return;
	}
	flag = false;
}

function f3(c){
	if(/[0-9]/.test(c)) return state = 3;
	if(c=='E') return state = 4;
	flag = false;
}

function f4(c){
	if(/[0-9]/.test(c)) return state = 6;
	if(/[+-]/.test(c)) return state = 5;
	flag = false;
}

function f5(c){
	if(/[0-9]/.test(c)) return state = 6;
	flag = false;
}

function f6(c){
	if(/[0-9]/.test(c)) return state = 6;
	flag = false;
}
var ftable = [f0,f1,f2,f3,f4,f5,f6];
while(pos < str.length)
	ftable[state](str[pos++]);

var ans = false;
if(state == 1 || state == 3 || state == 6) 
	ans = true;
console.log("Is %s number? ",str,flag && ans);
