import asyncio
import websockets
import threading

print("FORWARD")

async def send(message, backsocket):
    async with websockets.connect("ws://localhost:8888") as websocket:
        await websocket.send(message)
        resp = await websocket.recv()
        print(resp)
        await backsocket.send(resp)

async def listen(websocket, path):
    async for message in websocket:
        print(message)
        await send(message, websocket)

async def main():
    async with websockets.serve(listen, "localhost", 8080):
        await asyncio.Future()  # run forever

asyncio.run(main())