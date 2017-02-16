--- hackAIR ESP Firmware
-- Handles WiFi setup and network communications
-- @version 0.1.0
-- @author Thanasis Georgiou

--- The server URL used for sending data to the hackAIR project
server_URL = "http://hackair.draxis.gr:8000/sensors/arduino/measurements"
auth_token = ""

function send_to_hackAIR(args)
    -- Parse args
    local pm25, pm10, battery, tamper, error = string.match(args, "([^,]+),([^,]+),([^,]+),([^,]+),([^,]*)")
    local requestJson = '{"reading":{"PM2.5_AirPollutantValue":"' .. pm25 .. '","PM10_AirPollutantValue":"' .. pm10 ..'"},"battery":"' .. battery ..'","tamper":"' .. tamper ..'","error":"' .. error .. '"}'
    http.post(server_URL, 'Content-Type: application/json\r\nHTTP Accept header: application/vnd.hackair.v1+json\r\nAuthorization: '..auth_token..'\r\n', requestJson,
        function(code, data)
            uart.write(0, tostring(code))
        end
    )
end

--- Handle UART commands
-- The first word (string until first space) is the command, the rest is
-- parameters. Each command is terminated with a \n.
-- @param UART input, must be parsed
function handle_uart(data)
    -- Parse parameters
    local com, args = string.match(data, "([^%s]+)%s([^%s]+)")
    -- print("command: "..com)
    -- if args ~= nil then print("args: "..args) end
    
    -- Execute commands
    if com == 'e+raw' then -- Enter raw mode (lua console)
        uart.on("data")
        uart.setup(0, 115200, 8, uart.PARITY_NONE, uart.STOPBITS_1, 1)
    elseif com == 'e+restart' then -- Restart ESP
        node.restart()
    elseif com == 'e+send' then -- Send data to server
        send_to_hackAIR(args)
    elseif com == 'e+clearap' then -- Clear saved access points
        wifi.sta.config('', '')
        node.restart()
    elseif com == 'e+token' then -- Grab the token for authorization
	    auth_token = args
    elseif com == 'e+isready' then -- Is the ESP ready and connected?
        if wifi.sta.getip() ~= nil then
            uart.write(0, tostring(1))
        else
            uart.write(0, tostring(0))
        end
    end
end

-- Start listening to UART for commands
uart.setup(0, 115200, 8, uart.PARITY_NONE, uart.STOPBITS_1, 0)
uart.on('data', "\n", handle_uart, 0)

-- Create AP for configuration
wifi.ap.config({ssid = "hackAIR-"..node.chipid(), pwd = "hackAIR-"..node.chipid()})
wifi.setmode(wifi.STATIONAP)

-- Manual mode
enduser_setup.manual(true)
enduser_setup.start(
    function()
        -- WiFi information is stored in wifi.sta.config so we don't have
        -- to do anything special here.
        
        -- Print debug info
        print("Connected to wifi as:" .. wifi.sta.getip())
 
        -- Stop portal
        enduser_setup.stop()
        wifi.setmode(wifi.NULLMODE)
        wifi.setmode(wifi.STATION)
        wifi.sta.connect()
    end,
    function(err, str)
        print("enduser_setup: Err #" .. err .. ": " .. str)
    end)

wifiConfig = nil
