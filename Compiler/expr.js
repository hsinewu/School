var str = process.argv[2];
var pos = 0;
var flag = true;

function peek(){  return str[pos]; }
function exp(){  term();  A(); }
function term(){ fact();  B(); }
function A(){
	switch(peek()){
		case '+':
			++pos;	term();  A();  break;
		case '-':
			++pos;	term();  A();  break;
		default:
			break;
	}
}
function B(){
	switch(peek()){
		case '*':
			++pos;	term();  B();  break;
		case '/':
			++pos;	term();  B();  break;
		default:
			break;
	}
}
function fact(){
	switch(peek()){
		case 'i':
			++pos;
			if(peek()=='d') ++pos;
			else flag = false;
			break;
		case '(':
			++pos;
			exp();
			if(peek()==')') ++pos;
			else flag = false;
			break;
		default:
			flag = false;
	}
}
exp();
console.log( flag?'Yes':'No');
