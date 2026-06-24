/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   BotOptions.hpp									 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: agarcia <agarcia@student.42.fr>			+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2026/06/02 23:16:08 by adriescr		  #+#	#+#			 */
/*   Updated: 2026/06/08 17:05:48 by agarcia		  ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#ifndef BOT_OPTIONS_HPP
#define BOT_OPTIONS_HPP

#include <string>
#include <vector>


struct BotCmd {
	const std::string trigger;
	const std::string action;
	const std::string response;
	const bool isOp;
};

// Lista de comandos: añade nuevas entradas aquí. La lista termina con {NULL, NULL}.
static const std::vector<BotCmd> BOT_COMMANDS = {
	{ "unknown", "PRIVMSG", "Comando desconocido. Escribe !help para ver los comandos disponibles.", false },
	{ "hola", "PRIVMSG", "¡Hola! Soy un bot de IRC.", false },
	{ "tonteria", "PRIVMSG", "¡Perro Sanchez es el mejor!", false },
	{ "adios", "PRIVMSG", "¡Adiós! ¡Hasta la próxima!", false },
	{ "kick", "KICK", "¡Fuera de aquí!", true },
	{ "help", "PRIVMSG", "Comandos disponibles: ", false },
	{ "dice", "PRIVMSG", "¡Tira un dado! El resultado es: ", false },
	{ "coin", "PRIVMSG", "¡Lanza una moneda! El resultado es: ", false },
};

#endif
