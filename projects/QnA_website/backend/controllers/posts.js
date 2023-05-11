import pool from '../config/database_connection.js';

const queryMaker = (array,option)=>{

    var arr = [];

    array.forEach(el => {
        arr.push(el.tag_name);
    });

    // console.log(arr)

    const string = "select * from posts where ";
    var newString;
    var i;
    for(i=0;i<arr.length;i++){
        if(i == 0){
            newString = string.concat(`tags like '%${arr[i]}%' and `);
        }
        else{
            newString = newString.concat(`tags like '%${arr[i]}%' and `);
        }
    }
    const length = newString.length;
    newString = newString.slice(0,length-5);
    if(option === 'time'){
        newString = newString.concat(" order by creation_date asc");
    }
    else if(option === 'upvotes'){
        newString = newString.concat(" order by score desc");
    }
    return newString;
}

export const getPostById = async (req,res)=>{
    try {
        const {searchBy,array,option} = req.body;
        var getPosts;
        // console.log(array);
        if(searchBy === 'tag'){
            const string = queryMaker(array,option);
            // console.log(string)
            getPosts = await pool.query(string);
        }
        else{
            if(option === 'time'){
                getPosts = await pool.query("select * from posts where owner_user_id = $1 order by creation_date asc",[array]);
            }
            else if(option === 'upvotes'){
                getPosts = await pool.query("select * from posts where owner_user_id = $1 order by score desc",[array]);
            }
            else{
                getPosts = await pool.query("select * from posts where owner_user_id = $1",[array]);
            }
        }
        if(getPosts.rowCount === 0){
            console.log("here")
            return res.sendStatus(500);
        }
        res.json(getPosts.rows);
        
    } catch (error) {
        console.log(error)
    }
}

export const getTop = async (req,res)=>{
    try {
        const getTop = await pool.query("select * from posts where post_type_id = 1 order by score desc limit 10");
        // console.log(getTop);
        res.json(getTop.rows);
    } catch (error) {
        console.log(error)
    }
}

export const getPostByTag = async ()=>{
    try {
        
    } catch (error) {
        console.log(error);
    }
}
