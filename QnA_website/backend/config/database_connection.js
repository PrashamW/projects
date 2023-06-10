// const Pool = require('pg').Pool;

import Pool from 'pg'
const newPool = Pool.Pool

const pool = new newPool({
    user:"prasham",
    password:"prashampune2003",
    host:"localhost",
    port:"5432",
    database:"cqadb"
});

export default pool;