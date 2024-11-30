# need python3, python3 asyncpg_example.py
import asyncio
import asyncpg

async def run():
    conn = await asyncpg.connect(user='interma', password='111111',database='postgres', host='127.0.0.1')
    values = await conn.fetch('''SELECT * FROM t2''')
    print(values)
    await conn.close()

loop = asyncio.get_event_loop()
loop.run_until_complete(run())
