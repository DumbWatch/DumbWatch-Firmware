/*
 * This file is part of DumbWatch
 * Copyright (c) 2021 Jesse Buhagiar
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 *	Entry point as defined in `gcc_startup_nrf52.S`
 */
extern "C" void _start(void)
{
	 // Spin forever
	for(;;)
	{
		__asm__ volatile("wfi"); // Put the M4 core into low power mode
	}
}