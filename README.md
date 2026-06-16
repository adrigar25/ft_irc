# ft_irc

Servidor IRC escrito en C++ (compatible con C++98). Implementa las funcionalidades
básicas de un servidor IRC: autenticación (PASS/NICK/USER), creación/gestión de
canales, mensajería privada y por canal, modos de canal (básicos), invitaciones,
kick, topic y list. Fue desarrollado como ejercicio y usa `poll` para I/O
no bloqueante.

## Contenido
- Requisitos
- Compilación
- Ejecución
- Conectar con un cliente
- Flujo de autenticación
- Comandos soportados (con ejemplos)
- Modos de canal
- Códigos y respuestas importantes
- Límites y comportamientos a tener en cuenta
- Depuración y logs
- Contribuir

## Requisitos
- Compilador C++ compatible con C++98 (g++ / clang++)
- `make`

## Compilación
Desde la raíz del proyecto (`Programacion/ft_irc`):

```bash
make
# Genera el ejecutable `ircserv`
```

Targets útiles:
- `make` — compila y crea `ircserv`
- `make clean` — elimina objetos
- `make fclean` — elimina ejecutable y objetos
- `make re` — `fclean` + `all`

Nota: Hay un subdirectorio `bot/` con su propio `Makefile`, pero su `main.cpp`
está vacío actualmente.

## Ejecución
Este programa tiene dos formas de ejecución:
1. Especificando puerto y contraseña.
2. Especificando solo la contraseña (usa puerto por defecto 6667).

### La ejecución básica es:

```bash
./ircserv [port] <password>
```

- `[port]`: puerto TCP donde escuchará el servidor (1-65535). Opcional; si se omite se usa `6667`.
- `<password>`: contraseña que deben enviar los clientes con `PASS`.

Ejemplo:

```bash
./ircserv 6667 mysecret	# especificando puerto
```

El servidor imprimirá mensajes de estado en stdout (p. ej. `Server is listening on port 6667...`).

### La ejecución automatica es:

```bash
./ircserv <password>	   # usa puerto por defecto 6667
```

- `<password>`: contraseña que deben enviar los clientes con `PASS`.

Ejemplo:

```bash
./ircserv mysecret		 # usa puerto por defecto 6667
```

El servidor imprimirá mensajes de estado en stdout (p. ej. `Server is listening on port 6667...`).

## Conectar con un cliente
Puedes usar clientes IRC (irssi, weechat, HexChat, etc.) o herramientas de línea
de comandos como `nc` / `telnet`.

Ejemplo rápido con `nc` (en otra terminal):

```bash
nc localhost 6667
PASS mysecret
NICK alice
USER alice 0 * :Alice Example
```

Observaciones:
- Las líneas deben terminar en nueva línea; el servidor asegura CRLF al enviar respuestas.
- El servidor obtiene su nombre de host vía `gethostname()` (si falla usa `localhost`), y lo usa como prefijo en replies.

## Flujo de autenticación
Para que el usuario sea marcado como "autenticado" y pueda ejecutar la mayoría de
comandos, deben cumplirse:
1. Enviar `PASS <password>` con la contraseña correcta del servidor.
2. Enviar `NICK <nick>` para establecer nick.
3. Enviar `USER <username> <mode> <unused> :<realname>`.

Cuando las tres piezas estén presentes, el servidor envía el welcome numeric (`001`) al nick.

Comandos permitidos sin autenticación completa: `PASS`, `NICK`, `USER`, `PING` (y `PONG` lógicamente). El comando `CAP` existe en código pero no está registrado en el dispatcher (puede no estar disponible).

## Comandos soportados (resumen y ejemplos)

- Autenticación:
	- `PASS <password>`
		Ej: `PASS mysecret`
	- `NICK <nick>`
		Ej: `NICK alice`
	- `USER <username> <mode> <unused> :<realname>`
		Ej: `USER alice 0 * :Alice Example`

- Gestión de canales:
	- `JOIN <chan>[,<chan2>...] [keys]`
		Ej: `JOIN #general`
		Ej: `JOIN #secret secretkey`
		- Límite por defecto de canales por usuario: 10.
		- Respuestas numéricas en caso de error: `471` (límite), `473` (invite-only), `474` (banned), `475` (wrong key).
	- `PART <chan>[,chan2] [:message]`
		Ej: `PART #general :Bye`
	- `LIST` — Lista canales públicos (omite canales invite-only/secret).
	- `INVITE <nick> <channel>` — Invitar a usuario (solo operadores).
	- `KICK <channel> <nick> [:reason]` — Expulsar usuario (solo operadores).
		Ej: `KICK #general bob :Spam`
	- `MODE <channel> <modes> [params...]` — Cambiar modos del canal.
		Ej: `MODE #room +i`
		Ej: `MODE #room +k secretkey`
		Ej: `MODE #room +o alice`
		(ver Modos abajo)
	- `TOPIC <channel> [:topic]` — Obtener/establecer el topic. Si no se pasa topic devuelve `331`/`332`.

- Mensajería:
	- `PRIVMSG <target> :<message>` — Enviar mensaje a usuario o canal.
		Ej: `PRIVMSG #general :Hola a todos`
		Ej: `PRIVMSG bob :Hola bob`

- Control de sesión:
	- `PING <token>` — El servidor responde `PONG <token>`.
	- `QUIT [:message]` — Desconexión.

- Comando no reconocido: el servidor responde `421 <cmd> :Unknown command`.

## Modos de canal (soportados)
Los modos manejados por `MODE` (añadir con `+`, quitar con `-`):
- `i` — invite-only (solo invitados pueden entrar).
- `t` — topic protegido (solo ops pueden cambiar).
- `k` — key (clave requerida para entrar). Uso: `MODE #c +k secret`.
- `o` — asignar/remover operador a un nick. Uso: `MODE #c +o nick`.
- `l` — límite de usuarios. Uso: `MODE #c +l 10`.
- `b` — ban/unban de usuarios. Uso: `MODE #c +b nick`.

El mensaje MODE se propaga a todos los usuarios del canal.

## Códigos / replies importantes
El servidor usa una mezcla de mensajes IRC y mensajes "NOTICE"/textos. Algunos numerics que verás:
- `001` — Welcome message (al autenticarse correctamente).
- `353` — NAMES list (lista de usuarios de un canal).
- `366` — End of NAMES list.
- `331` — No topic set.
- `332` — Topic string.
- `401` / `403` / `404` — No such nick/channel / No such channel / Cannot send to channel.
- `431` / `433` — No nickname given / Nickname in use.
- `461` — Not enough parameters.
- `471` — Cannot join channel (+l).
- `473` — Cannot join channel (+i).
- `474` — Cannot join channel (+b).
- `475` — Cannot join channel (+k) or generic join failure.
- `482` — You're not channel operator.
- `441` — They aren't on that channel.
- `421` — Unknown command.
- `405` — You have joined too many channels (límite por usuario).

Observa que algunos handlers devuelven mensajes en texto libre (p. ej. `LIST` devuelve un texto multi-line) en vez de numerics completos.

## Límites y comportamiento importante
- Límite máximo de canales por usuario: 10 (constante en código).
- No hay persistencia: todos los canales/usuarios se pierden al cerrar el servidor.
- No hay TLS/SSL: la conexión es TCP sin cifrado.
- El servidor usa `gethostname()` para el prefijo de mensajería.
- El comando `CAP` tiene implementación, pero actualmente no se registra en el dispatcher (puede no responder).
- El servidor acepta formato de comandos siguiendo la sintaxis clásica de IRC; los handlers hacen stripping de `:` y `\\r`.

## Depuración y logs
- El servidor escribe logs a stdout (creación de server, bind, listen, creación/borrado de canales, errores de send, eventos de I/O).
- Los mensajes enviados a clientes terminan en `\\r\\n` (CRLF) según RFC básico.

## Ejemplo de sesión completa (netcat)
Terminal 1 (arrancar servidor):

```bash
make
./ircserv 6667 mysecret
```

Terminal 2 (cliente con `nc`):

```bash
nc localhost 6667
PASS mysecret
NICK alice
USER alice 0 * :Alice
JOIN #general
PRIVMSG #general :Hola desde netcat
```

Si otro cliente se une como `bob`, ambos recibirán el PRIVMSG en el canal.

## Desarrollo y contribuciones
- Estándar: código compilado con `-std=c++98` y `-Wall -Wextra -Werror`.
- Si quieres añadir comandos o registrar `CAP`, registra el handler en `src/server/ServerCommands.cpp` (llamada a `dispatcher.registerHandler`).
- Para mejoras: soporte TLS, persistencia de canales/usuarios, tests automatizados, validación más estricta de nicks/identidad.

## Archivos relevantes
- Ejecutable generado: `ircserv` (Makefile en la raíz).
- Código fuente: `src/` y headers en `includes/`.
- Punto de entrada: `main.cpp` (`Programacion/ft_irc/main.cpp`).

---

Si quieres, puedo ajustar el README (por ejemplo, añadir salidas de ejemplo exactas, más ejemplos de `MODE`, o soporte de `systemd`), o crear un `CONTRIBUTING.md` separado.

