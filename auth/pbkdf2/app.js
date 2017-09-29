const crypto = require('crypto');
let password = "deepintoiot";
let salt = "saltingval";
let iterations = 901;
//let genPass = '';
crypto.pbkdf2(password, salt, iterations, 24, 'sha256', (err, derivedKey) => {
    if (err) throw err;
    //genPass =
    console.log(`PBKDF2${derivedKey.toString('hex')}`);
});