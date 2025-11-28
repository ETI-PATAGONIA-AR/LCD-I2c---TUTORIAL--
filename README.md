En este ejemplo, veremos como implementar un display LCD I2c para hacer un sencillo controlador multifunción experimental.
Para no emplear un encoder, se implemento un potenciómetro el cual la lectura es mapeada y con ello, podemos recorrer el menú a las distintas funciones.
Una de las funciones, es poder visualizar la temperatura con el sensor LM35, y poder programar la acción de un RELAY según la temperatura (hay que mejorar 
esta sección para poder tener un mayor margen de trabajo, o sea, que se acciones entre una Temp MIN y una Temp MAX).
Otra de las funciones, es la de poder tener un control digital de encendido y apagado de luces (6 LED o LAMPARAS DC), que se complementa con otra función 
mas dentro del menú, que es el control del brillo por intermedio de un PWM.
La ultima función del menú, es la visualización de todas las variables / estados en la pantalla...

<img width="837" height="588" alt="LCD_I2c" src="https://github.com/user-attachments/assets/c3700381-099f-436d-abe1-582ebdc6c416" />
