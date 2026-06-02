ircbot
======

Cliente IRC simple en C++ incluido en este repositorio.

Características
- Responde `PING` automáticamente.
- Se une automáticamente a un canal tras recibir `INVITE`.
- Responde con una de dos "tonterías" cuando lo mencionan o le envían un mensaje privado.

Compilar
-------

Desde el directorio `bot/`:

```bash
make ircbot
```

Ejecutar
--------

Uso básico:

```bash
./ircbot <host> <port> <nick>
# ejemplo:
./ircbot localhost 6667 MiBot
```

Modo automático de puerto (tu script de servidor debe escribir el puerto en `.irc_port`):

```bash
./ircbot localhost auto MiBot
```

Notas
- Programa simple, pensado como ejemplo. No gestiona reconexiones avanzadas ni todos los cachos del protocolo.
- Si quieres que el bot se ejecute como servicio, crea un script que arranque el servidor y escriba el puerto en `.irc_port`.
