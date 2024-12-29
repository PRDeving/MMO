import express from 'express'
import bodyParser from 'body-parser'
import { createClient } from 'redis'
import sha256 from 'sha256'

const redis = await createClient()
  .on('error', err => console.log('Redis Client Error', err))
  .connect();

const app = express()
app.use(bodyParser.json())

const salt = 'salty'
// Mock for user:password:salty
const T = '043c610169b0adb08e0c354785d745325a31df527c1ce52dc526413918e2d1f3'
const USER_ID = sha256('user')

app.post('/login', async (req, res) => {
    const {username, password} = req.body
    console.log('Login request', username, password)

    const auth = sha256(`${username}:${password}:${salt}`)
    if (auth === T) {
        const session = sha256(`${auth}:${Date.now()}`)
        await redis.set(session, USER_ID)
        res.send({
            session,
            host: "127.0.0.1",
            port: 6379
        })
    } else {
        res.status(401).send('Unauthorized')
    }
})

app.listen(3000, () => console.log('Gateway Server running on port 3000'))