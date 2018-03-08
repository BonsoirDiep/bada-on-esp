# Easy IOTs with ESP kit: like NodeMCU
***
Design [here](https://badaiots-kz.herokuapp.com/nguoidung/thietke.html) (You need "productKey" signup and request productKey (can send mail to: thandieu13@gmail.com))

Control and monitoring
```text
	bada-on-esp.ino -> badaiots-kz.herokuapp.com/nguoidung/theodoi.html
	bada-on-esp-v2.ino -> badaiots-kz.herokuapp.com/nguoidung/theodoi2.html
```
Note:
```text
	id [ 5, 12*, 13*, 14*, 15*, 16* ]    -> node with type "ao" (~PWM)
	id [ 5*, 12, 13, 14*, 15*, 16* ]     -> node with type "do"
	id [ 17 ]                            -> node with type "ai"
	id [ 2, 4 ]                          -> node with type "one"
	
	(*) different than Arduino Uno
	
	To not be confused
	id [ 5 ]                             -> node with type "ao" (~PWM)
	id [ 12, 13 ]                        -> node with type "do"
	id [ 17 ]                            -> node with type "ai"
	id [ 2, 4 ]                          -> node with type "one"
```
[Nodemcu GPIO] (https://nodemcu.readthedocs.io/en/master/en/modules/gpio/)
```text	
	D3-> 0
	D4-> 2
	D2-> 4
	
	A0-> 17
	
	D1-> 5
	D6-> 12
	D7-> 13
	D5-> 14
	D8-> 15
	D0-> 16
```
