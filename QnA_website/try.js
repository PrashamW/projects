const bcrypt = require('bcrypt')

const x = "Maharshi"
const saltRounds = 10;

const hash_pw = async ()=>{
    const salt = await bcrypt.genSalt(10);
    const encrypted = await bcrypt.hash(x,salt);

    console.log(encrypted);

    const compare  =bcrypt.compare(x,encrypted);
    if(compare){
        console.log("matched");
    }
    else{
        console.log("NO");
    }
}

hash_pw();


