//This script run in nodeJS
var set = require('./set.js')
var readline = require('readline');
var rl = readline.createInterface({input:process.stdin,output:process.stdout});
var fs;

function ps(){ return '$' + new Date().toLocaleTimeString() + '>'}
function f(cmd){
	cmd = cmd.trim()
	if(cmd.charAt(0) == '.'){
		var argv = cmd.slice(1).split(' ')
		switch(argv[0]){
			case 'a':
			case 'auto':
				console.log('Auto print ' + (set.togglePrint()? 'on':'off'))
				break;
			case 'forget':
				set.forget()
				break;
			case 'h':
			case 'help':
				console.log('.help .h .auto .a .forget .variable .var .exit')
				break;
			case 'var':
			case 'variable':
				set.vars()
				break;
			case 'load':
				fs = fs || require('fs')
				console.log('Loading file "%s"...', argv[1])
				fs.readFileSync(argv[1]).toString().split('\n').forEach(function(line,idx){
					set.run(line)
				})
				console.log()
				break;
			case 'exit':
				process.exit()
			default:
				console.log('unknown command')
		}
	}else
		set.run(cmd)
	rl.question(ps(),f)
}
console.log()
console.log('type .help for more information')
console.log('Interpreter v1.4.0 2015')
rl.question(ps(), f);
