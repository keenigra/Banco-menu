#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <fstream>

using namespace std;

void limpiarBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}


class Cliente;
void guardarDatos(const vector<Cliente>& clientes, const string& archivo);
void cargarDatos(vector<Cliente>& clientes, const string& archivo);


class Cuenta {
private:
    string numeroCuenta;
    double saldo;
    vector<string> historial;

public:
    Cuenta(string numero) {
        numeroCuenta = numero;
        saldo = 0.0;
        historial.push_back("Cuenta creada.");
    }

    void depositar(double monto) {
        saldo += monto;
        historial.push_back("Deposito: Q" + to_string(monto));
    }

    void retirar(double monto) {
        if (monto <= saldo) {
            saldo -= monto;
            historial.push_back("Retiro: Q" + to_string(monto));
        }
        else {
            cout << "Fondos insuficientes.\n";
            historial.push_back("Intento fallido de retiro: Q" + to_string(monto));

        }
    }

    void transferirA(Cuenta& destino, double monto) {
        if (monto <= saldo) {
            saldo -= monto;
            destino.saldo += monto;
            historial.push_back("Transferencia enviada a " + destino.getNumero() + ": Q" + to_string(monto));
            destino.historial.push_back("Transferencia recibida de " + numeroCuenta + ": Q" + to_string(monto));
        }
        else {
            cout << "Fondos insuficientes para transferir.\n";
            historial.push_back("Intento fallido de transferencia a " + destino.getNumero() + ": Q" + to_string(monto));
        }
    }

    void mostrarSaldo() const {
        cout << "Cuenta: " << numeroCuenta << ", Saldo: Q" << saldo << endl;
    }

    void mostrarHistorial() const {
        cout << "Historial de cuenta " << numeroCuenta << ":\n";
        for (const string& registro : historial) {
            cout << "- " << registro << endl;
        }
    }

    string getNumero() const {
        return numeroCuenta;
    }

    
    friend void guardarDatos(const vector<Cliente>& clientes, const string& archivo);
    friend void cargarDatos(vector<Cliente>& clientes, const string& archivo);
};


class Cliente {
private:
    string nombre;
    vector<Cuenta> cuentas;

public:
    Cliente(string nombreCliente) {
        nombre = nombreCliente;
    }

    void agregarCuenta(Cuenta c) {
        cuentas.push_back(c);
    }

    void mostrarCuentas() const {
        cout << "Cuentas de " << nombre << ":\n";
        for (const auto& cuenta : cuentas) {
            cuenta.mostrarSaldo();
        }
    }

    string getNombre() const {
        return nombre;
    }

    vector<Cuenta>& getCuentasRef() {
        return cuentas;
    }

    const vector<Cuenta>& getCuentasConst() const {
        return cuentas;
    }

    friend void guardarDatos(const vector<Cliente>& clientes, const string& archivo);
    friend void cargarDatos(vector<Cliente>& clientes, const string& archivo);
};


void guardarDatos(const vector<Cliente>& clientes, const string& archivo) {
    ofstream out(archivo);
    for (const auto& cliente : clientes) {
        out << "CLIENTE " << cliente.getNombre() << endl;
        for (const auto& cuenta : cliente.getCuentasConst()) {
            out << "CUENTA " << cuenta.getNumero() << " " << cuenta.saldo << endl;
            for (const auto& h : cuenta.historial) {
                out << "HISTORIAL " << h << endl;
            }
            out << "FINCUENTA" << endl;
        }
        out << "FINCLIENTE" << endl;
    }
}


void cargarDatos(vector<Cliente>& clientes, const string& archivo) {
    ifstream in(archivo);
    if (!in) return;

    string linea;
    Cliente* clienteActual = nullptr;
    Cuenta* cuentaActual = nullptr;

    while (getline(in, linea)) {
        if (linea.find("CLIENTE ") == 0) {
            string nombre = linea.substr(8);
            clientes.push_back(Cliente(nombre));
            clienteActual = &clientes.back();
        } else if (linea.find("CUENTA ") == 0 && clienteActual) {
            size_t pos1 = linea.find(' ', 7);
            string numero = linea.substr(7, pos1 - 7);
            double saldo = stod(linea.substr(pos1 + 1));
            clienteActual->agregarCuenta(Cuenta(numero));
            cuentaActual = &clienteActual->getCuentasRef().back();
            cuentaActual->saldo = saldo;
            cuentaActual->historial.clear();
        } else if (linea.find("HISTORIAL ") == 0 && cuentaActual) {
            cuentaActual->historial.push_back(linea.substr(10));
        } else if (linea == "FINCUENTA") {
            cuentaActual = nullptr;
        } else if (linea == "FINCLIENTE") {
            clienteActual = nullptr;
        }
    }
}


int main() {
    vector<Cliente> clientes;
    cargarDatos(clientes, "banco.txt");

    int opcion;
    do {
        cout << "\n         ===== BANCO CPP =====\n";
        cout << "1. Registrar cliente\n";
        cout << "2. Crear cuenta para cliente\n";
        cout << "3. Mostrar cuentas\n";
        cout << "4. Salir\n";
        cout << "5. Depositar en cuenta\n";
        cout << "6. Retirar de cuenta\n";
        cout << "7. Transferir entre cuentas\n";
        cout << "8. Ver historial de cuenta\n";
        cout << "9. Guardar datos\n";
        cout << "10. Cargar datos\n";
        cout << "Opcion: ";
        cin >> opcion;
        limpiarBuffer();

        if (opcion == 1) {
            string nombre;
            cout << "Nombre del cliente: ";
            getline(cin, nombre);
            clientes.push_back(Cliente(nombre));
            cout << "Cliente agregado.\n";
        }
        else if (opcion == 2) {
            string nombre, numero;
            cout << "Nombre del cliente: ";
            getline(cin, nombre);
            cout << "Numero de cuenta: ";
            getline(cin, numero);

            bool encontrado = false;
            for (auto& cliente : clientes) {
                if (cliente.getNombre() == nombre) {
                    cliente.agregarCuenta(Cuenta(numero));
                    cout << "Cuenta agregada.\n";
                    encontrado = true;
                    break;
                }
            }
            if (!encontrado) cout << "Cliente no encontrado.\n";
        }
        else if (opcion == 3) {
            if (clientes.empty()) {
                cout << "No hay clientes registrados.\n";
            }
            else {
                for (const auto& cliente : clientes) {
                    cliente.mostrarCuentas();
                }
            }
        }
        else if (opcion == 5) {
            string nombreCliente, numeroCuenta;
            double monto;
            cout << "Nombre del cliente: ";
            getline(cin, nombreCliente);
            cout << "Numero de cuenta: ";
            getline(cin, numeroCuenta);
            cout << "Monto a depositar: Q";
            cin >> monto;
            limpiarBuffer();

            bool realizado = false;
            for (auto& cliente : clientes) {
                if (cliente.getNombre() == nombreCliente) {
                    for (Cuenta& cuenta : cliente.getCuentasRef()) {
                        if (cuenta.getNumero() == numeroCuenta) {
                            cuenta.depositar(monto);
                            cout << "Deposito realizado.\n";
                            realizado = true;
                            break;
                        }
                    }
                }
            }
            if (!realizado) cout << "Cuenta o cliente no encontrados.\n";
        }
        else if (opcion == 6) {
            string nombreCliente, numeroCuenta;
            double monto;
            cout << "Nombre del cliente: ";
            getline(cin, nombreCliente);
            cout << "Numero de cuenta: ";
            getline(cin, numeroCuenta);
            cout << "Monto a retirar: Q";
            cin >> monto;
            limpiarBuffer();

            bool realizado = false;
            for (auto& cliente : clientes) {
                if (cliente.getNombre() == nombreCliente) {
                    for (Cuenta& cuenta : cliente.getCuentasRef()) {
                        if (cuenta.getNumero() == numeroCuenta) {
                            cuenta.retirar(monto);
                            realizado = true;
                            break;
                        }
                    }
                }
            }
            if (!realizado) cout << "Cuenta o cliente no encontrados.\n";
        }
        else if (opcion == 7) {
            string clienteOrigen, cuentaOrigen, clienteDestino, cuentaDestino;
            double monto;
            cout << "Cliente origen: ";
            getline(cin, clienteOrigen);
            cout << "Cuenta origen: ";
            getline(cin, cuentaOrigen);
            cout << "Cliente destino: ";
            getline(cin, clienteDestino);
            cout << "Cuenta destino: ";
            getline(cin, cuentaDestino);
            cout << "Monto a transferir: Q";
            cin >> monto;
            limpiarBuffer();

            Cuenta* origen = nullptr;
            Cuenta* destino = nullptr;

            for (auto& cliente : clientes) {
                if (cliente.getNombre() == clienteOrigen) {
                    for (Cuenta& cuenta : cliente.getCuentasRef()) {
                        if (cuenta.getNumero() == cuentaOrigen) {
                            origen = &cuenta;
                        }
                    }
                }
                if (cliente.getNombre() == clienteDestino) {
                    for (Cuenta& cuenta : cliente.getCuentasRef()) {
                        if (cuenta.getNumero() == cuentaDestino) {
                            destino = &cuenta;
                        }
                    }
                }
            }

            if (origen && destino) {
                origen->transferirA(*destino, monto);
                cout << "Transferencia realizada.\n";
            }
            else {
                cout << "Cuenta origen o destino no encontrada.\n";
            }
        }
        else if (opcion == 8) {
            string nombreCliente, numeroCuenta;
            cout << "Nombre del cliente: ";
            getline(cin, nombreCliente);
            cout << "Numero de cuenta: ";
            getline(cin, numeroCuenta);

            bool encontrado = false;
            for (auto& cliente : clientes) {
                if (cliente.getNombre() == nombreCliente) {
                    for (Cuenta& cuenta : cliente.getCuentasRef()) {
                        if (cuenta.getNumero() == numeroCuenta) {
                            cuenta.mostrarHistorial();
                            encontrado = true;
                            break;
                        }
                    }
                }
            }
            if (!encontrado) cout << "Cuenta o cliente no encontrados.\n";
        }
        else if (opcion == 9) {
            string archivo;
            cout << "Nombre del archivo para guardar: ";
            getline(cin, archivo);
            guardarDatos(clientes, archivo);
            cout << "Datos guardados.\n";
        }
        else if (opcion == 10) {
            string archivo;
            cout << "Nombre del archivo para cargar: ";
            getline(cin, archivo);
            cargarDatos(clientes, archivo);
            cout << "Datos cargados.\n";
        }

    } while (opcion != 4);

    guardarDatos(clientes, "banco.txt");

    cout << "Datos guardados al salir.\n";
    return 0;
}
