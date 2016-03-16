[str,pos,ans] = [process.argv[2], 0, true]

cur = -> str[pos]
exp = -> A term()
term = -> B fact()
A = (v) ->
  switch cur()
    when '+' then ++pos;  A v+term()
    when '-' then ++pos;  A v-term()
    else v
B = (v) ->
  switch cur()
    when '*' then ++pos;  B v*fact()
    when '/' then ++pos;  B v/fact()
    else v
fact = ->
  switch cur()
    when '(' then ++pos; v=exp();  (if cur() is ')' then ++pos;v else ans=false)
    else (if isNaN cur() then ans=false else (pos++; Number str[pos-1]))

console.log(exp())
console.log(ans)
