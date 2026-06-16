/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotOptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 23:16:08 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/08 17:05:48 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_OPTIONS_HPP
#define BOT_OPTIONS_HPP

#include <string>


struct BotCmd {
	const std::string trigger;
	const std::string action;
	const std::string response;
};

// Lista de comandos: añade nuevas entradas aquí. La lista termina con {NULL, NULL}.
static const BotCmd BOT_COMMANDS[] = {
	{ "unknown", "PRIVMSG", "Comando desconocido. Escribe !help para ver los comandos disponibles." },
	{ "hola", "PRIVMSG", "¡Hola! Soy un bot de IRC." },
	{ "tonteria", "PRIVMSG", "¡Perro Sanchez es el mejor!" },
	{ "adios", "PRIVMSG", "¡Adiós! ¡Hasta la próxima!" },
	{ "kick", "KICK", "¡Fuera de aquí!" },
	{ "help", "PRIVMSG", "Comandos disponibles: "},
	{ "dice", "PRIVMSG", "¡Tira un dado! El resultado es: "},
	{ "coin", "PRIVMSG", "¡Lanza una moneda! El resultado es: "},
};

#endif
