import pool from '../config/database_connection.js';
import jwt from 'jsonwebtoken';

export const getTags = async (req,res)=>{ // searching tag names
    try{
        var {id} = req.params;
            if(!id) return res.send("Type something");
            var string = '%'+id+'%'
            const getResult = await pool.query("select * from tags where tag_name like $1 order by tag_name limit 100",[string]);
            res.json(getResult.rows);
    }
    catch(err){
        console.log(err+"here2");
    }
}

export const getUserDisplayNames = async (req,res)=>{ // searching user display names
    try{
        var {id} = req.params;
        var string = '%'+id+'%'
        const getResult = await pool.query("select * from users where display_name like $1 order by display_name limit 100",[string]);
        res.json(getResult.rows);
    }
    catch(err){
        console.log(err+"here2");
    }
}

export const getUserDisplayNames2 = async (req,res)=>{
    try{
        const getResult = await pool.query("select * from users order by reputation desc limit 100");
        res.json(getResult.rows);
    }
    catch(err){
        console.log(err+"here2");
    }
}

export const getUserInfo = async (req,res)=>{ // getting user info
    try {
        const {id} = req.params;
        const getResult = await pool.query("select * from users where id = $1",[id]);
        res.json(getResult.rows)
        console.log(getResult.rows)
    } catch (error) {
        console.log(error)
    }
}

export const logout = async(req,res)=>{ // logout
    try {
        // console.log(req.cookies);
        const refreshToken = req.cookies.refreshToken;
        
        const getUser = await pool.query("select * from users where refreshToken = $1",[refreshToken]);
        const user = getUser.rows;
        
        await pool.query("update users set refreshToken = null where id = $1",[user[0].id]);

        res.clearCookie('refreshToken');
        res.sendStatus(200);
        console.log("done");

    } catch (error) {
        console.log(error)
    }
}

export const login = async (req,res)=>{ // login
    try {
        const {username,password} = req.body
        const user = await pool.query("select * from users where username = $1",[username]);
        const row = user.rows
        if(row[0].password !== password) res.status(400).send({msg:"Wrong username - password combination"});
        const id = row[0].id

        const accessToken = jwt.sign({id}, process.env.ACCESS_TOKEN_SECRET,{
            expiresIn: '30s'
        });

        const refreshToken = jwt.sign({id}, process.env.REFRESH_TOKEN_SECRET,{
            expiresIn: '1d'
        });

        await pool.query("update users set refreshToken = $1 where id = $2",[refreshToken,id]);

        res.cookie('refreshToken', refreshToken,{
            httpOnly: true,
            maxAge:24*60*60*1000,
        });
        res.json({ accessToken });

    } catch (error) {
        console.log(error)
    }
}

export const refreshToken = async (req,res)=>{
    try {
    const cookies = req.cookies;

    if(!cookies) return res.sendStatus(401);
    const refreshToken = cookies.refreshToken;

    const getUser = await pool.query("select * from users where refreshToken = $1",[refreshToken])
    if(!getUser) return res.sendStatus(403);

    const Newuser = getUser.rows
    const user = Newuser[0];

    jwt.verify(
        refreshToken,
        process.env.REFRESH_TOKEN_SECRET,
        (err,decoded)=>{
            if(err) {console.log(err); return res.sendStatus(403);}
            const accessToken = jwt.sign({"id":user.id},process.env.ACCESS_TOKEN_SECRET,{
                expiresIn :'60s'
            });
            // console.log(user.id)
            res.json({accessToken})
            // console.log("Done")
        }
    )
    }
    catch (error) {
        console.log(error)
    }
}