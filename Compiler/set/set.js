var log, autoPrint = false
var str,pos,flag,global={};
// ex: global[_] = {'type':_,'val':_};
var typedef = require('./typedef.js')

function init(s){
	str = s
	pos = 0
	flag = true
}
function info(label){
	var s = "At " + pos + ", get " + str[pos]
	if(label) s = label + s
	console.log(s)
}
function err(label){
	if(flag)
		log(label || 'unknown error')
	flag = false
}
function space(){
	while(/\s/.test(str[pos])) pos++
}
function readStr(s) {
  space();
  if (str.substr(pos, s.length) == s) {
    pos += s.length;
    return;
  }
  err('readStr: read no '+s)
};
function id(){	//return variable name
	space()
	if( !/[A-Z]/.test(str[pos]) )
		err('id: Id must be [A-Z]')
	return str[pos++]
	// var beg = pos
	// while( str[pos] && /[A-Z0-9]/.test(str[pos]) ) pos++
	// var end = pos
	// return str.slice(beg,end)
}
//set literal
function litSet(){
	readStr('{')
	var beg = pos
	var end = str.indexOf('}', pos)
	var csv = str.slice(beg, end)
	pos = end+1	//end is at '}'
	return csv.split(',').map(function(s){return s.trim()}) 
}
function dot(s){
	while(str[pos]=='.'){
		readStr('.')
		if(str.substr(pos,6) == 'unique'){
			readStr('unique')
			s = s.filter(function(v1,i1){
				return !s.some(function(v2,i2){
					return i1>i2 && v1 == v2
				})
			})
		}
		else if(str.substr(pos,4) == 'sort'){
			readStr('sort')
			var mirror = Array.apply(null,s)
			s = mirror.sort()	//because sort() mutates
		}else if(str.substr(pos,3) == 'ave'){
			readStr('ave')
			return s.reduce(function(ac,v){return Number(v)+ac},0)/s.length
		}else{
			err('unknown Set method')
		}
	}
	return s
}
function set(){
	space()
	var s
	if(str[pos]=='{')
		s = litSet()
	else{
		var name = id()
		s = global[name]
		if(s == undefined)
			return err('undeclared varible ' + name)
		s = s.val
	}
	return dot(s)
}
function eset(){
	var lhs = set()
	space()
	var boom = 0;
	while(boom++ <9999)
	switch(str[pos]){
		case '+':
			readStr('+')
			lhs =  lhs.concat(set())
			space()
			break;
		case '-':
			readStr('-')
			var rhs = set()
			lhs =  lhs.filter(function(e){
				return !rhs.some(function(e2){
					return e==e2
				})
			})
			space()
			break;
		case '*':
			readStr('*')
			var rhs = set()
			lhs = lhs.reduce(function(ac,v){
				return ac.concat(rhs.reduce(function(ac2,v2){
					return ac2.concat(v+v2)
				},[]))
			}, [])
			space()
			break;
		default:
			//shorten mutiplication
			if(/[A-Z]/.test(str[pos])){
				rhs = global[id()]
				if(rhs == undefined)
					return err('undeclared variable ' + rhs)
				rhs = rhs.val
				lhs = lhs.reduce(function(ac,v){
					return ac.concat(rhs.reduce(function(ac2,v2){
						return ac2.concat(v+v2)
					},[]))
				}, [])
				continue
			}
			return lhs
	}
	console.log('eset unexpeted flow')
	return []
}
function assign(){
	var type = str.substr(0,str.indexOf(' '))
	if(typedef.names.some(function(n){return n==type}))
		readStr(type)
	else
		type = false
	var name = id()
	type = type || global[name] && global[name].type
	readStr('=')
	var s = eset()
	if(type && !s.every(function(v){return typedef[type].typecheck(v)}) )
		return err('type check error')
		global[name] = {'type':type,'val':s}
	return s;
}
function format(myset){
	//myset is actuall js array
	if(!myset) return //console.log('format get: ' +  myset)
	if({}.toString.call(myset)==='[object Array]')
		return '{' + myset.toString() + '}'
	return myset.toString()
}
function dispatch(){
	//This is where stupid things happene
	if( /\s*Print()\s*/.test(str) ){
		// console.log('type print')
		var name = str.match(/\(.*\)/g)[0].slice(1,-1)
		init(name)
		if(autoPrint)
			return dispatch()
		else
			return !flag || console.log(dispatch())
	}else if( str.search('=')!= -1){
		return format(assign())
	}else if( /\|.*\|/.test(str) ){
		readStr('|')
		return eset().length
	}else if(/\d+[A-Z]/.test(str)){
		while(/\d/.test(str[pos])) pos++
		var m = parseInt(str.slice(0,pos))
		if(isNaN(m)) console.log('multipler m error')
		var s = set();
		if(s.some(function(e){return isNaN(e)}))
			return 'Cannot do this'
		var ret = s.map(function(e){
			return e * m
		})
		return format(ret)
	}
	// console.log('type eset')
	return format(eset())
}
function run(cmd){
	init(cmd)
	if(!log) log = console.log
	var s = dispatch()
	if(autoPrint && flag && s)
		log(s)
}

if(module) module.exports = {
	'run':run,
	'togglePrint':function(){return autoPrint = !autoPrint;},
	'vars': function(){for(k in global)log('%s of type %s = %s',k,global[k].type,format(global[k].val))},
	'forget': function(){global = {}}
};