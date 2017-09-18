var mqttClient = require('mqtt');
var mongoClient = require('mongodb').MongoClient;
var assert = require('assert');
var dbUser = "localuser";
var dbPass = "localpassword";
var connectionString = `mongodb://${dbUser}:${dbPass}@ds139954.mlab.com:39954/mqttdatastore`;
var rootTopic = "demo/nodeclient";
var collection, client;
client = mqttClient.connect("mqtt://sungura1-angani-ke-host.africastalking.com:1882");
client.on('connect', function() {
    console.log("Connected successfully to broker");
    client.subscribe('demo/nodeclient/presence');
    mongoClient.connect(connectionString, function(err, db) {
        collection = db.collection('messages');
        assert.equal(null, err);
        console.log("Connected successfully to server");
    });
});
client.on('message', function(topic, message) {
    // message is Buffer
    console.log(message.toString());
    // client.end();
    collection.insertOne({ messages: message.toString() });
});