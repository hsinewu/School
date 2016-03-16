[str,pos,ans] = [process.argv[2], 0, true]

cur = -> str[pos]
exp = -> A term()
term = -> B fact()
A = (op1) ->
	switch cur()
		when '+'
			++pos;
			A op1+term() + '+'
		when '-'
			++pos;
			A op1+term() + '-'
		else op1
B = (op1) ->
	switch cur()
		when '*'
			++pos;
			B op1+fact() + '*'
		when '/'
			++pos;
			B op1+fact() + '/'
		else op1
fact = ->
	switch cur()
		when '('
			++pos
			v=exp()
			if cur() is ')'
				++pos
				v
			else
				ans=false
		else
			if isNaN cur() then ans=false
			else str[pos++]

console.log(exp())
console.log(ans)
