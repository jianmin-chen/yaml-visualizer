import fs from 'fs'
import { parse } from 'yaml'

fs.readFile(process.argv[2], 'utf-8', (err, data) => {
  console.log(parse(data))
})
