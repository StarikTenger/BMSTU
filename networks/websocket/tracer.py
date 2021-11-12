import asyncio
import websockets
from pythonping import ping
from icmplib import multiping, traceroute, resolve

print("TRACER")

response = ping('ya.ru', count = 1)
print(response.__repr__())

async def echo(websocket, path):
    async for message in websocket:
        print(message)
        mesage_parsed = message.split(sep=",")
        resp = ''
        if mesage_parsed[0] == 'ping':
            resp = ping(mesage_parsed[1], count = int(mesage_parsed[2])).__repr__()
        if mesage_parsed[0] == 'trace':
            hops = traceroute(mesage_parsed[1])
            
            resp += 'Distance/TTL    Address    Average round-trip time\n'
            last_distance = 0

            for hop in hops:
                if last_distance + 1 != hop.distance:
                    resp += 'Some gateways are not responding\n'
        
                # See the Hop class for details
                resp += f'{hop.distance}    {hop.address}    {hop.avg_rtt} ms\n'
            
                last_distance = hop.distance

        await websocket.send(resp)

async def main():
    async with websockets.serve(echo, "localhost", 8888):
        await asyncio.Future()  # run forever

asyncio.run(main())