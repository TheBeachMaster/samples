var mqttClient = require('mqtt');
var mongoClient = require('mongodb').MongoClient;
var assert = require('assert');
var dbUser = "localuser";
var dbPass = "localpassword";
var connectionString = `mongodb://${dbUser}:${dbPass}@ds139954.mlab.com:39954/mqttdatastore`;
var collection, client;
var options = {
    username: "kapua-broker",
    password: "kapua-password"
};
client = mqttClient.connect("mqtt://sungura1-angani-ke-host.africastalking.com:1883", options);
client.on('connect', function() {
    console.log("Connected successfully to broker");
    mongoClient.connect(connectionString, function(err, db) {
        collection = db.collection('messages');
        assert.equal(null, err);
        console.log("Connected successfully to server");
    });
    client.subscribe('kapua-sys/ESP8266/sensors');
});
client.on('message', function(topic, message) {
    // message is Buffer
    console.log(message.toString());
    var data = message.toString();
    // client.end();
    collection.insertOne({ messages: data, time: Date() });
});