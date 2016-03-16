module.exports = {
	'names':['int','char'],
	'int':{
		'typecheck':function(s){return !isNaN(s)}
	},
	'char':{
		'typecheck':function(s){return /[a-z]/.test(s)}
	}
}