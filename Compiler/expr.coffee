[str,pos,ans] = [process.argv[2], 0, true];

cur = -> str[pos];
exp = -> term(); A();
term = -> fact(); B();
A = ->
  switch cur()
    when '+','-' then ++pos;  term();  A()
B = ->
  switch cur()
    when '*','/' then ++pos;  fact();  B()
fact = ->
  switch cur()
    when 'i' then ++pos;
    when '(' then ++pos; exp();  if cur() is 'i' then ++pos else ans=false;
exp();
console.log(ans);
