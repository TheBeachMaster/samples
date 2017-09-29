var mqtt = require('mqtt')
var options = {
    username: "africastalking",
    password: "africastalking"
}
var client = mqtt.connect('mqtt://sungura1-angani-ke-host.africastalking.com:1882', options)

client.on('connect', function() {
    client.subscribe('presence')
    client.publish('presence', 'Hello mqtt')
})

client.on('message', function(topic, message) {
    // message is Buffer
    console.log(message.toString())
    client.end()
})