var str = process.argv[2];
var pos = 0;
var flag = true;

function cur(){  while(/\s/.test(str[pos])) ++pos;  return str[pos]; }
function assign(){ stmt();  cur(); if(pos>=str.length) return;  assign()}
function stmt(){dec();  id();
	if(cur()=='='){  ++pos;  exp();}
	else  flag = false;
	if(cur()==';'){  ++pos; }
	else  flag = false;
}
function dec(){
	if(str.substr(pos,7) == "declare") pos+=7;
	else flag = false;
}
function exp(){  term();  A(); }
function term(){ fact();  B(); }

function A(){
        switch(cur()){
                case '+':
                        ++pos;        term();  A();  break;
                case '-':
                        ++pos;        term();  A();  break;
		case ')':  case '*':  case '/':
		case 'd':  case ';':
			break;
                default:
			if(/[0-9A-Z]/.test(cur())) return;
			if(pos>=str.length) return;
			console.error('A: %s\n',cur());
			flag = false;
                        break;
        }
}
function B(){
        switch(cur()){
                case '*':
                        ++pos;        term();  B();  break;
                case '/':
                        ++pos;        term();  B();  break;
		case ')':  case '+':  case '-':
		case 'd':  case ';':
			break;
                default:
			if( /[0-9A-Z]/.test(cur()) ) return;
			if(pos>=str.length) return;
			console.error('B: %s\n',cur());
			flag = false;
                        break;
        }
}
function fact(){
        switch(cur()){
                case '(':
                        ++pos;
                        exp();
                        if(cur()==')') ++pos;
                        else flag = false;
                        break;
                default:
			if( /[A-Z]/.test(cur()) ){
				id();
			}else if( /[0-9]/.test(cur()) ){
				number();
			}
			else{
				console.error('fact: %s\n',cur());
	                        flag = false;
			}
        }
}
function digit(){
	if(/[0-9]/.test(cur())) ++pos;
	else flag = false;
}
function letter(){
	if(/[A-Z]/.test(cur())) ++pos;
	else flag = false;
}
function number(){
	digit();
	while(/[0-9]/.test(str[pos]) )  digit();
	if(str[pos] == '.'){
		++pos;
		while(/[0-9]/.test(str[pos])) digit();
	}
	if(str[pos] == 'E'){
		++pos;
		if(str[pos] == '+' || str[pos] == '-')  ++pos;
		digit();
		while(/[0-9]/.test(str[pos]) )  digit();
	}
}
function id(){
	letter();
	if(/[0-9]/.test(cur())) digit();
	if(/[A-Z]/.test(cur())) letter();
	/*switch(cur()){	
		case '0': case '1': case '2':
		case '3': case '4': case '5':
		case '6': case '7': case '8':
		case '9':  
	}*/
}
function init(s){ str = s; pos = 0; flag = true;}
function outcome(){  console.log( flag?'Yes':'No');}
function test(s){
	str = s;  pos = 0; flag = true;
	exp();
	console.log( flag?'Yes':'No');

}
module.exports = {'init':init, 'assign':assign, 'outcome':outcome};
