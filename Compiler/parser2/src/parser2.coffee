#Set up global variables
console.log 'By hsine'
[str,pos,valid] = ['',0,true]
if process?
	str = process.argv[2] if process.argv[2]?
log = (args...) -> args.forEach (e) -> console.log '\t' + e

#Helper functions
init = (s) ->
	[str,pos,valid] = [s,0,true]
	return
peek = () ->
	space()
	str[pos]
isdigit = (c) ->
	'0123456789'.split ''
	.some (e) -> e==c
isletter = (c) ->
	'ABCDEFGHIJKLMNOPQRSTUVWXYZ'.split ''
	.some (l)->
		l==c
isspace = (c) ->
	/\s/.test c
space = () ->
	pos++ while isspace str[pos]
	return

#Symbol
readStr = (s) ->
	space()
	if str.substr(pos, s.length) is s
		pos += s.length
		return
	log "missing symbol #{s}, get #{str[pos]} at #{pos}" if valid
	valid = false
digit = () ->
	if isdigit peek()
		++pos
		return
	log "missing digit, get #{str[pos]} at #{pos}" if valid
	valid = false
digits = () ->
	digit()
	while isdigit peek()
		digit()
	return
letter = () ->
	if isletter peek()
		++pos
		return
	log "missing letter, get #{str[pos]} at #{pos}(missing id?)" if valid
	valid = false
number = () ->
	digits()
	if peek() is '.'
		++pos
		digits()
	if peek() is 'E'
		++pos
		if str[pos] is '+' or str[pos] is '-'
			++pos
		digits()
	return
id = () ->
	letter()
	type = true
	while type
		type = false
		type = 1 if isdigit str[pos]
		type = 2 if isletter str[pos]
		if type is 1
			digit()
		else if type is 2
			letter()
	return

#Syntax
stmts = () ->
	#space()
	if peek() is 'I'
		ifStm()
	else
		assStm()
assStm = () ->
	stmt()
	space();
	unless valid is false || pos >= str.length || peek() is '}' || peek() is '$'
		assStm()
ifStm = () ->
	readStr 'If'
	readStr '('
	id()
	readStr '>'
	digit()
	readStr ')'
	readStr '{'
	assStm()
	readStr '}'
stmt = () ->
	readStr 'declare'
	if valid and /[0-9a-zA-Z]/.test str[pos]
		log("unexpected token #{str[pos]} at #{pos}(missing white space?)")
		valid = false
	id();
	readStr '='
	exp()
	readStr ';'
exp = () ->
	term()
	A()
A = () ->
	switch peek()
		when '+','-'
			++pos
			term()
			A()
			break
term = () ->
	fact()
	B()
B = () ->
	switch peek()
		when '*','/'
			++pos
			term()
			B()
			break
fact = () ->
	if peek() is '('
		readStr '('
		exp()
		readStr ')'
	else if isletter peek()
		id()
	else if isdigit peek()
		number()
	else
		log "unexpected input #{str[pos]} at #{pos}(missing oprand?)" if valid
		valid = false
debug = (label) ->
	console.log label,str.substr pos,str.length

#A simple alias
start = stmts

#Run some test
if str.length > 1
	stmts()
	console.log "Is '#{str}' valid? #{valid}"
else if process?
	fs = require 'fs'
	datas = JSON.parse fs.readFileSync 'datas.json'
	datas.forEach (obj) ->
		console.log "  Test for: #{obj.description}" if obj.description?
		init(obj.code)
		start()
		console.log "    '#{obj.code}' #{valid}"
		if obj.assert? and valid != obj.assert
			console.log 'WARNING!!! THE OUTCOME OF #{obj.code} IS NOT WHAT I EXPECT'