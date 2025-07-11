#Sistema Bancario en C++ (Consola)

Este es un sistema bancario simple desarrollado en C++ que permite registrar clientes, crear cuentas bancarias, realizar transacciones como depósitos, retiros, transferencias, y guardar/cargar la información desde un archivo.

##Funcionalidades principales

- Registrar nuevos clientes
- Crear cuentas bancarias por cliente
- Consultar saldos
- Realizar depósitos y retiros
- Transferencias entre cuentas
- Ver historial de movimientos
- Guardar y cargar datos desde archivo (`.txt`)

##Tecnologías usadas

- Lenguaje: C++
- Entrada/Salida estándar
- Manejo de archivos con `fstream`
- Vectores (`std::vector`)
- Clases y métodos
- Funciones `friend` para acceso controlado

##Estructura del código

- `Cliente`: clase que representa a una persona con cuentas
- `Cuenta`: clase que gestiona saldo, historial y operaciones
- `main`: menú interactivo para usar el sistema desde consola

##Guardado de datos

El sistema guarda la información de los clientes y sus cuentas en un archivo de texto estructurado (por ejemplo: `banco.txt`) para persistencia entre sesiones.

##Cómo ejecutar

Compilar y ejecutar desde consola con cualquier compilador C++ moderno, por ejemplo:

```bash
g++ banco.cpp -o banco
./banco
