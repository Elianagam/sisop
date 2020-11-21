# Lab: shell

### Búsqueda en $PATH

La diferencia entre execve y exec es que la primera recibe un array con las variables de entorno para poder ejecutar el programa, al contrario de las ultimas que toma las variables por entono default del actual proceso

---

### Comandos built-in

El que se podria implementar sin built-in seria PWD ya que para un resultado igual se podria hacer uno de ```ècho $PWD``` 

---

### Variables de entorno adicionales

En el caso de execve la funcion agrega, ademas de las variables que se le pasan, SHLVL, _ y PWD explicitamente.
Al contrario de setenv que solo agrega la variable que se le pasa por paramentro.

Una posible implementacion seria agregar todos los valores que se agregarian explicitamente con el execve y tambien los que se quiera setear diferente a ellos.

---

### Procesos en segundo plano

Se hace un fork en exec_cmd en el case BACK y en la llamada al hijo se hace una llamada recursiva a exec_cmd y al contrario del caso EXEC no se hace un wait en el padre, para que el promt aparezca justo debajo del comando ejecutado.

---

### Flujo estándar

En las shell  cuando se quiere combinar la salida stderr y stdout en la salida normal para poder así manipular los errores.
La diferencia es que cuando se hace 2>out.txt se escribe en un archivo los errores, en cambio al usar 2>&1 se escribe en salida estandar, que fue redireccionado al archivo por lo que el archivo contiene la salida y los erroeres.

---

### Tuberías simples (pipes)

Un mecanismo de tuberias es tal se conectan los procesos para que la entrada de uno sea la salida del otro. Permiten la comunicacion y sincronizacion entre procesos.
Los casos de error pueden ser porque el array para la creacion de la tuberia es invalido o porque hay demasiados FD abiertos.

---

### Pseudo-variables

* Variable $$: Expande el Id del proceso del script.
* Variable $?: Expande el status de lo ultimo proceso que se haya ejecutado.

---


