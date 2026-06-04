/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BotOptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 23:16:08 by adriescr          #+#    #+#             */
/*   Updated: 2026/06/04 17:32:59 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_OPTIONS_HPP
#define BOT_OPTIONS_HPP

// Definición de comandos del bot
struct BotCmd {
	const char *trigger;
	const char *response;
};

// Lista de comandos: añade nuevas entradas aquí. La lista termina con {NULL, NULL}.
static const BotCmd BOT_COMMANDS[] = {
	{ "hola", "Tontería 1: ¡beep boop!" },
	{ "tonteria", "Tontería 2: ¿Por qué el pollo cruzó IRC?" },
	{ NULL, NULL }
};

#endif
