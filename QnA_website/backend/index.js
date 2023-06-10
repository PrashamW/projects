import express from 'express';
import cors from 'cors';
import cookieParser from 'cookie-parser';
import dotenv from 'dotenv';
import router from './routes/index.js'

dotenv.config();
const app = express();

app.use(cors({origin:'http://localhost:3000',credentials:true}));
app.use(cookieParser());
app.use(express.json()); 
app.use(express.urlencoded({extended:false}));
app.use(router);


app.listen(5000,()=>{
    console.log("server running on port 5000");
})