from random import sample
from operator import mul
cityCode = {'A':10, 'B':11, 'C':12, 'D':13, 'E':14, 'G':16, 'H':17, 'K':19, 'M':21, 'N':22, 'P':23, 'T':27, 'U':28, 'V':29, 'W':32, 'X':30}
rule = [1,9,8,7,6,5,4,3,2,1]
def generateID(city,gender):
	c = cityCode[city]
	a = [c/10, c%10, gender] + sample(range(0,10),7)
	s = sum(map( mul, a, rule))
	check = 10 - ( s % 10)	# (check + s) mod 10 = 0
	check %= 10
	a.append(check)
	return city + reduce(lambda acc,x: acc+str(x), a[2:], '')

def code_city_pairs():
	yield 'D', 'Tainan'		# yes I'm from Tainan
	yield 'A', 'Taipei'
	yield 'B', 'Taichung'
	yield 'C', 'Keelung'
	yield 'E', 'Kaohsiung'
	yield 'G', 'Ilan'
	yield 'H', 'Taoyuan'
	yield 'K', 'Miaoli'
	yield 'M', 'Nantou'
	yield 'N', 'Changhua'
	yield 'P', 'Yunlin'
	yield 'T', 'Pingtung '
	yield 'U', 'Hualien'
	yield 'V', 'Taitung'
	yield 'W', 'Kinmen'
	yield 'X', 'Penghu'