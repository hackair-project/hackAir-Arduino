/*
 * hackAIR Arduino Library
 * Copyright © 2016-2017 hackAir Consortium
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file Create flash script
 * @author Thanasis Georgiou
 *
 * This script will minify the Lua firmware into a series of commands
 * that the ESP can run over serial to recreate the firmware
 */

const fs = require('fs');

// Remove old firmware and open new file
console.log('file.remove(\'init.lua\')');
console.log('file.open(\'init.lua\', \'w+\')');

// Add the helper function to the header
console.log('function __hexwrite(s) for c in s:gmatch(\'..\') do file.write(string.char(tonumber(c, 16))) end end');

// Open the file
const rawContent = fs.readFileSync('./src/init.lua');
const content = rawContent.toString('hex');
const chunks = content.match(/.{1,232}/g);

// Add each chunk as a Serial.println() command
chunks.forEach((chunk) => console.log(
    `__hexwrite("${chunk}")`
));
console.log('file.close()');
