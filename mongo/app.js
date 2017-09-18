let mongo = require('mongodb').MongoClient,
    assert = require('assert');
let user = "localuser",
    password = "localpassword";
let url = `mongodb://${user}:${password}@ds139954.mlab.com:39954/mqttdatastore`;

let insertData = function(db, callback) {
    let collection = db.collection('messages');
    collection.insertMany([{ a: 1 }, { a: 2 }, { a: 3 }], function(err, result) {
        assert.equal(err, null);
        assert.equal(3, result.result.n);
        assert.equal(3, result.ops.length);
        console.log(`Inserted 3 documents into ${collection}`);
        callback(result);
    });
}
mongo.connect(url, function(err, db) {
    assert.equal(err, null);
    console.log('Connected successfully');
    insertData(db, function() { db.close(); });
});
// mongo.connect(url, function(err, db) {
//     assert.equal(null, err);
//     console.log("Connected successfully to server");

//     db.close();
// });